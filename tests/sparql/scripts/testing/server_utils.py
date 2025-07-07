from __future__ import annotations

import json
import socket
import subprocess
import sys
import time
import warnings
from http.client import RemoteDisconnected
from io import TextIOWrapper
from pathlib import Path
from subprocess import Popen
from typing import Any, Dict, Optional
from urllib.error import URLError

from SPARQLWrapper import SPARQLWrapper
from SPARQLWrapper.SPARQLExceptions import EndPointInternalError, QueryBadFormed

from .logging import Level, log
from .options import (
    CWD,
    EMPTY_DB_DATA,
    HOST,
    PORT,
    SERVER_LOGS_DIR,
    SLEEP_DELAY,
    TEST_SUITE_DIR,
    TESTING_DBS_DIR,
    TIMEOUT,
)
from .types import (
    BadTest,
    ExecutionStats,
    ServerCrashedException,
    ServerErrorException,
    Test,
)


def create_db(executable: Path, rdf_file: Optional[Path], prefixes_file: Optional[Path] = None):
    if rdf_file is None:
        rdf_file = EMPTY_DB_DATA

    if not rdf_file.is_file():
        log(Level.ERROR, f"File not found {rdf_file}")
        sys.exit(1)

    db_dir = TESTING_DBS_DIR / rdf_file.relative_to(TEST_SUITE_DIR)
    db_dir = db_dir.parent / rdf_file.stem

    if db_dir.exists():
        log(Level.WARNING, f'Database "{db_dir.relative_to(TESTING_DBS_DIR)}" already exists')
    else:
        cmd: list[str] = [str(executable), "import", str(rdf_file), str(db_dir)]
        if prefixes_file is not None:
            if not prefixes_file.is_file():
                log(Level.ERROR, f"File not found {prefixes_file}")
                sys.exit(1)
            cmd.extend(["--prefixes", str(prefixes_file)])

        try:
            subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            log(Level.DEBUG, f'Database "{db_dir}" created')
        except subprocess.CalledProcessError as exc:
            raise exc

    return db_dir


__log_file: TextIOWrapper | None = None


def start_server(executable: Path, db_dir: Path):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    address = (HOST, PORT)

    # Check if port is already in use
    if sock.connect_ex(address) == 0:
        log(Level.ERROR, f"Port {PORT} is already in use")
        sys.exit(1)

    global __log_file

    if __log_file is not None:
        __log_file.close()
        __log_file = None

    log_path = SERVER_LOGS_DIR / db_dir.relative_to(TESTING_DBS_DIR).with_suffix(".log")
    log_path.parent.mkdir(parents=True, exist_ok=True)
    log_path.touch()

    __log_file = log_path.open(mode="a+", encoding="utf-8")

    cmd: list[str] = [str(executable), "server", str(db_dir), "--timeout", str(TIMEOUT), "--port", str(PORT)]
    server_process = subprocess.Popen(cmd, stdout=__log_file, stderr=__log_file)

    # Wait for server initialization
    while sock.connect_ex(address) != 0:
        # We have to close and recreate the socket because it is not specified
        # what happens if connect fails. On Linux the socket can continue to be
        # used. On macOS, however, the socket is in an unusable state.
        sock.close()
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        time.sleep(SLEEP_DELAY)

    log(Level.BEGIN, f'SERVER STARTED: "{db_dir.relative_to(CWD)}"')
    return server_process, __log_file


def kill_server(server_process: Popen[bytes]):
    server_process.kill()
    server_process.wait()
    log(Level.END, "SERVER KILLED")


def execute_query(
    server: Optional[Popen[bytes]], test: Test, stats: ExecutionStats, bad_query: bool = False
) -> Dict[Any, Any] | str | None:
    with test.query.open(encoding="utf-8") as file:
        query_str = file.read()
    log(Level.DEBUG, f"query_str: {query_str}")
    sparql_wrapper = SPARQLWrapper(f"http://{HOST}:{PORT}/sparql")

    if test.graph_output:
        sparql_wrapper.setReturnFormat("turtle")
    else:
        sparql_wrapper.setReturnFormat("json")

    sparql_wrapper.setMethod("POST")
    sparql_wrapper.setRequestMethod("postdirectly")
    sparql_wrapper.setQuery(query_str)

    try:
        response = sparql_wrapper.query()
    except QueryBadFormed as exc:
        if server and server.poll() is not None:
            stats.error += 1
            log(Level.ERROR, str(test), "Server crashed")
            raise ServerCrashedException from exc
        if bad_query:
            raise ServerErrorException from exc
        else:
            stats.error += 1
            log(Level.ERROR, str(test), "Server returned error")
            return None
    except EndPointInternalError as exc:
        if server and server.poll() is not None:
            stats.error += 1
            log(Level.ERROR, str(test), "Server crashed")
            raise ServerCrashedException from exc
        stats.error += 1
        log(Level.ERROR, str(test), "Server returned internal error")
        return None
    except RemoteDisconnected as exc:
        if server and server.poll() is not None:
            stats.error += 1
            log(Level.ERROR, str(test), "Server crashed")
            raise ServerCrashedException from exc
        stats.error += 1
        log(Level.ERROR, str(test), "Server closed connection")
        return None
    except URLError as exc:
        stats.error += 1
        log(Level.ERROR, str(test), "Server crashed")
        raise ServerCrashedException from exc
    except ConnectionResetError as exc:
        stats.error += 1
        log(Level.ERROR, str(test), "Server crashed")
        raise ServerCrashedException from exc

    try:
        response = response.convert()
        if isinstance(response, dict):
            return response
        if isinstance(response, bytes):
            return response.decode()
    except json.decoder.JSONDecodeError as exc:
        stats.error += 1
        log(Level.ERROR, str(test), "Returned invalid JSON")
        return None

    raise ValueError("Did not receive a dict or bytes from SPARQLWrapper")


def execute_bad_query(server: Optional[Popen[bytes]], test: BadTest, stats: ExecutionStats):
    """Returns True if the query throws an error, False otherwise"""
    try:
        with warnings.catch_warnings():
            warnings.simplefilter("ignore")
            execute_query(server, test, stats, bad_query=True)
        return False
    except ServerErrorException:
        return True
