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
CREATE_DB_EXECUTABLE = CWD / "build/Debug/bin/mdb-import"
SERVER_EXECUTABLE = CWD / "build/Debug/bin/mdb-server"

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
    "aggs",
    "dates",
    "is",
    "simple",
    "paths",
    "bad_queries",
    "escape-unicode",
    "property_types_queries",
    "regex",
]

# Tests with the following query files fill be ignored
IGNORED_TESTS: set[str] = set()
