from __future__ import annotations

from pathlib import Path

# Host and port the server will listen on
HOST = "127.0.0.1"
PORT = 8080

# Time between checks for server initialization
SLEEP_DELAY = 0.01

# Maximum time in seconds that the server will wait for a query
TIMEOUT = 60

# Assume that the script is run from the root directory
CWD = Path().cwd()
ROOT_TEST_DIR = CWD / "tests/mql"
TEST_SUITE_DIR = ROOT_TEST_DIR / "test_suites"
TESTING_DBS_DIR = ROOT_TEST_DIR / "tmp/dbs"
SERVER_LOGS_DIR = ROOT_TEST_DIR / "tmp/server-logs"

# Executables
CREATE_DB_EXECUTABLE = CWD / "build/Debug/bin/create_db_mql"
SERVER_EXECUTABLE = CWD / "build/Debug/bin/server_mql"
QUERY_EXECUTABLE = CWD / "build/Debug/bin/query_mql"

# Empty database used for some tests
EMPTY_DB_DATA = TEST_SUITE_DIR / "empty_db.qm"

# Width of each column of test outputs
OUTPUT_COLUMN_WIDTH = 50

LOGGING_LEVELS = {
    "SUMMARY": True,
    "ERROR": True,
    "WARNING": True,
    "OUTPUT": True,
    "CORRECT": False,
    "BEGIN": False,
    "END": False,
    "SKIPPED": False,
    "DEBUG": False,
    "SERVER_LOG": False,
}

# Test suites to run
TEST_SUITES: list[str] = [
    "simple",
    "paths",
]

# Tests with the following query files fill be ignored
IGNORED_TESTS: set[str] = {
    # Path order has to be fixed
    "simple/queries/path-1.mql",
}
