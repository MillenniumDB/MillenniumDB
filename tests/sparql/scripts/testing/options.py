from pathlib import Path
from typing import Dict, List

# Host and port the server will listen on
HOST = "127.0.0.1"
PORT = 8080

# Time between checks for server initialization
SLEEP_DELAY = 0.01

# Maximum time in seconds that the server will wait for a query
TIMEOUT = 60

# Assume that the script is run from the root directory
CWD = Path().cwd()
ROOT_TEST_DIR = CWD / "tests/sparql"
TESTING_DBS_DIR = ROOT_TEST_DIR / "tmp/dbs"
TEST_SUITE_DIR = ROOT_TEST_DIR / "test_suites"
SERVER_LOGS_DIR = ROOT_TEST_DIR / "tmp/server-logs"

# Executables
CREATE_DB_EXECUTABLE = CWD / "build/Debug/bin/mdb-import"
SERVER_EXECUTABLE = CWD / "build/Debug/bin/mdb-server"

# Empty database used for some tests
EMPTY_DB_DATA = TEST_SUITE_DIR / "empty_db.ttl"

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

# Tests with queries including the following keywords will be ignored
NOT_SUPPORTED_KEYWORDS = ["from", "graph"]

# Official root manifests to run
ROOT_MANIFESTS: List[Path] = [
    TEST_SUITE_DIR / "sparql10/manifest-evaluation.ttl",
    TEST_SUITE_DIR / "sparql11/manifest-sparql11-query.ttl",
    TEST_SUITE_DIR / "sparql11/manifest-sparql11-results.ttl",
    # TEST_SUITE_DIR / "sparql11/manifest-sparql11-fed.ttl",
]

# Internal test directories to run
INTERNAL_TESTS: List[str] = [
    "agg01",
    "agg-numeric2",
    "misc",
    "bind_official",
    "compression-old",
    "construct",
    "datetime",
    "describe",
    "escape-unicode",
    "example",
    "existence",
    "ill-typed",
    "minus",
    "nested",
    "optional",
    "paths",
    "union",
    "print",
    "schemes",
    # "service",
]

# If the following list is not empty, only those tests will be executed
d = TEST_SUITE_DIR / "internal/sparql/nested"
ONLY_SPECIFIC_TESTS: List[Dict[str, Path]] = [
    # {
    #     "query": d / "good_queries/simple-1.rq",
    #     "data": d / "nested.ttl",
    #     "expected": d / "good_queries/simple-1.json",
    # }
]

# Tests with the following query files fill be ignored
IGNORED_TESTS = {
    # Property paths
    "sparql11/property-path/pp10.rq",
    # "sparql11/property-path/pp11.rq",  # We dont have a duplicated result
    "sparql11/property-path/pp14.rq",  # This ignores 2 tests, both of which fail
    # unfixed nullable has a different semantic
    # (we don't return all the database)
    # "sparql11/property-path/path-p2.rq",  # We dont have a duplicated result
    "sparql11/property-path/nps_inverse.rq",
    "sparql11/property-path/nps_direct_and_inverse.rq",
    # Exact lexical representation of numeric types
    "sparql10/distinct/distinct-1.rq",  # This ignores 4 tests, 2 that fail and 2 that pass
    "sparql10/expr-builtin/q-str-1.rq",
    "sparql10/expr-builtin/q-str-2.rq",
    "sparql10/expr-builtin/sameTerm.rq",
    "sparql10/expr-builtin/sameTerm-eq.rq",
    "sparql10/expr-builtin/sameTerm-not-eq.rq",
    "sparql10/expr-equals/query-eq-graph-1.rq",
    "sparql10/expr-equals/query-eq-graph-2.rq",
    "sparql10/open-world/open-eq-01.rq",
    # Case sensitivity of language tags
    "sparql10/expr-builtin/q-lang-3.rq",
    # The following test has two different output file for the same query file.
    # It tests bracket simplification in queries with FILTER inside OPTIONAL.
    # By design, only one can pass.
    "sparql10/optional-filter/expr-5.rq",  # This ignores 2 tests, 1 that fails and 1 that passes
}

# The canonicalization of numbers changes the syntax in which the number is represented.
# for example, if CANONICALIZE_NUMBER = True, then 1e2 = 1E2 is 100, 02 is 2, and such.
# Types, if present, will still be compared.
CANONICALIZE_NUMBERS = True

# URLS are changed from <url> to url
CANONICALIZE_URI = True

# Make all languages lower case because language comparison is case insensitive
CANONICALIZE_LANGUAGES = True

# Convert "0"^^xsd:boolean and "1"^^xsd:boolean to "true"^^xsd:boolean and "false"^^xsd:boolean, respectively
CANONICALIZE_BOOLEANS = True

# Translate datatypes
TRANSLATE_DATATYPES = {
    "xsd:negativeInteger": "xsd:integer",
    "xsd:string": "",
}
