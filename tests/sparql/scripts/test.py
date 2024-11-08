#!/usr/bin/python3
import argparse
import sys
from pathlib import Path
from typing import List

from testing import options
from testing.file_handler import (
    get_internal_tests,
    get_official_tests,
    query_has_keywords,
)
from testing.logging import print_summary
from testing.query_execution import execute_tests
from testing.types import ExecutionStats, QueryTest, Test, TestSuite
from tqdm import tqdm


def run_all_tests(server_executable: Path, create_db_executable: Path):
    stats = ExecutionStats("All Tests")

    test_suites: List[TestSuite] = []

    test_suites.append(get_internal_tests())
    test_suites.extend(get_official_tests())

    test_count = sum(len(ts.tests) for ts in test_suites)

    progress_bar = tqdm(
        total=test_count, leave=False, dynamic_ncols=True, bar_format="{l_bar}{bar}| {n_fmt}/{total_fmt}"
    )
    for test_suite in test_suites:
        stats += execute_tests(
            test_suite,
            server_executable=server_executable,
            create_db_executable=create_db_executable,
            progress_bar=progress_bar,
        )
    progress_bar.close()

    stats.log()
    print_summary()
    return stats.error != 0


def run_specific_tests(server_executable: Path, create_db_executable: Path):
    query_tests: list[Test] = []
    for test in options.ONLY_SPECIFIC_TESTS:
        query = Path(test["query"])
        data = Path(test["data"])
        expected = Path(test["expected"])
        query_test = QueryTest(query=query, data=data, expected=expected)

        if query_has_keywords(query, ["describe", "construct"]):
            query_test.graph_output = True

        query_tests.append(query_test)

    stats = execute_tests(
        TestSuite("Specific Tests", query_tests),
        server_executable=server_executable,
        create_db_executable=create_db_executable,
    )
    print_summary()
    return stats.error != 0


RUN_CUSTOM_FUNCTION = False


def run_custom_tests(server_executable: Path, create_db_executable: Path):
    """Use this function to do custom testing for debugging, etc"""
    stats = ExecutionStats("Custom Tests")

    cwd = Path().cwd()

    pref = Path("tests/sparql10/distinct")
    queries = [pref / "no-distinct-1.rq"]
    data = cwd / pref / Path("data-all.ttl")
    expected = cwd / pref / Path("no-distinct-all.srx")

    tests: list[Test] = []
    for query in queries:
        query = cwd / Path(query)
        test = QueryTest(query=query, data=data, expected=expected)
        tests.append(test)

    stats += execute_tests(
        TestSuite("Custom Tests", tests),
        server_executable=server_executable,
        create_db_executable=create_db_executable,
        client_only=False,
    )

    stats.log()
    print_summary()
    return stats.error != 0


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--server-executable", type=str, help="Custom server executable", default=options.SERVER_EXECUTABLE
    )
    parser.add_argument(
        "--create-db-executable", type=str, help="Custom create-db executable", default=options.CREATE_DB_EXECUTABLE
    )
    args = parser.parse_args()

    if options.ONLY_SPECIFIC_TESTS:
        sys.exit(run_specific_tests(args.server_executable, args.create_db_executable))

    elif RUN_CUSTOM_FUNCTION:
        sys.exit(run_custom_tests(args.server_executable, args.create_db_executable))

    else:
        sys.exit(run_all_tests(args.server_executable, args.create_db_executable))
