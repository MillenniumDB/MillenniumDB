#!/usr/bin/python3
from pathlib import Path
from typing import List

from testing.file_handler import (
    get_internal_tests,
    get_official_tests,
    query_has_keywords,
)
from testing.logging import print_summary
from testing.options import ONLY_SPECIFIC_TESTS
from testing.query_execution import execute_tests
from testing.types import ExecutionStats, QueryTest, Test, TestSuite
from tqdm import tqdm


def run_all_tests():
    stats = ExecutionStats("All Tests")

    test_suites: List[TestSuite] = []

    test_suites.append(get_internal_tests())
    test_suites.extend(get_official_tests())

    test_count = sum(len(ts.tests) for ts in test_suites)

    progress_bar = tqdm(
        total=test_count, leave=False, dynamic_ncols=True, bar_format="{l_bar}{bar}| {n_fmt}/{total_fmt}"
    )
    for test_suite in test_suites:
        stats += execute_tests(test_suite, progress_bar=progress_bar)
    progress_bar.close()

    stats.log()
    print_summary()


def run_specific_tests():
    query_tests: list[Test] = []
    for test in ONLY_SPECIFIC_TESTS:
        query = Path(test["query"])
        data = Path(test["data"])
        expected = Path(test["expected"])
        query_test = QueryTest(query=query, data=data, expected=expected)

        if query_has_keywords(query, ["describe", "construct"]):
            query_test.graph_output = True

        query_tests.append(query_test)

    execute_tests(TestSuite("Specific Tests", query_tests))
    print_summary()


RUN_CUSTOM_FUNCTION = False


def run_custom_tests():
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

    stats += execute_tests(TestSuite("Custom Tests", tests), client_only=False)

    stats.log()
    print_summary()


if __name__ == "__main__":
    if ONLY_SPECIFIC_TESTS:
        run_specific_tests()

    elif RUN_CUSTOM_FUNCTION:
        run_custom_tests()

    else:
        run_all_tests()
