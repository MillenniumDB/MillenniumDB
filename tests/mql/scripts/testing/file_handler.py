from __future__ import annotations

import csv
import re
import sys
from pathlib import Path

from .logging import Level, log
from .options import TEST_SUITE_DIR, TEST_SUITES
from .types import BadTest, Bindings, QueryTest, Result, Test, TestSuite


def get_test_suites() -> list[TestSuite]:
    log(Level.BEGIN, "GET TESTS")

    test_suites: list[TestSuite] = []

    for test_dir in (TEST_SUITE_DIR).iterdir():
        if test_dir.name not in TEST_SUITES:
            continue

        log(Level.BEGIN, f"{test_dir.name}")

        test_suite = test_dir.name
        data = test_dir / f"{test_suite}.qm"

        if not data.is_file():
            print(f'Could not find database file "{data}"')
            sys.exit(1)

        test_list: list[Test] = []

        for query in test_dir.glob("**/*.mql"):
            expected = query.with_suffix(".csv")
            has_bad_suffix = len(query.suffixes) >= 2 and query.suffixes[-2] == ".bad"

            if expected.is_file():
                test = QueryTest(query=query, data=data, expected=expected)
                if has_bad_suffix:
                    print(f'Query should not have a ".bad.mql" suffix because "{expected}" exists')
                    sys.exit(1)
            elif has_bad_suffix:
                test = BadTest(query=query, data=data)
            else:
                print(f'Query does not have a ".bad.mql" suffix and "{expected}" does not exist')
                sys.exit(1)

            log(Level.DEBUG, str(test))
            test_list.append(test)

        test_suites.append(TestSuite(name=test_suite, tests=test_list))

        log(Level.END, test_suite)

    test_suites.sort(key=lambda ts: ts.name)

    log(Level.END, "GET TESTS")
    return test_suites


def get_results_from_csv(string: str) -> Result:
    result_data = csv.reader(string.splitlines())

    variables = next(result_data)
    bindings: Bindings = list(result_data)

    return Result(variables, bindings)


def get_results_from_csv_file(path: Path) -> Result:
    with open(path, encoding="utf-8") as file:
        result_string = file.read()

    return get_results_from_csv(result_string)


def query_has_order_by(path: Path) -> bool:
    with path.open(encoding="utf-8") as file:
        query_string = " ".join(file.readlines()).lower()
    match = re.search(" order by ", query_string)
    return match is not None
