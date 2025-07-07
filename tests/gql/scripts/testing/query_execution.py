from __future__ import annotations

import math
from collections import defaultdict
from io import SEEK_END
from itertools import zip_longest
from pathlib import Path
from subprocess import Popen

import colorama
from tqdm import tqdm

from .file_handler import (
    get_results_from_csv,
    get_results_from_csv_file,
    query_has_order_by,
)
from .logging import Level, log
from .options import IGNORED_TESTS
from .options import OUTPUT_COLUMN_WIDTH as CW
from .options import TEST_SUITE_DIR
from .server_utils import create_db, execute_query, kill_server, start_server
from .types import (
    BadTest,
    ExecutionStats,
    QueryTest,
    Result,
    ServerCrashedException,
    Test,
    TestSuite,
)


def calculate_epsilon(expected: float) -> float:
    if expected == 0:
        return 1e-6

    decimal_part = str(expected).rsplit(".", maxsplit=1)[0]
    position = len(decimal_part.rstrip("0"))

    return 1 ** (-position)


def is_approx_equal(received: str | None, expected: str | None) -> bool:
    if received is None or expected is None:
        return received == expected

    try:
        expected_float = float(expected)
        received_float = float(received)
        if math.isnan(abs(received_float - expected_float)):
            return received == expected
        if not ("." in expected or "E" in expected):
            return received == expected
        epsilon = calculate_epsilon(expected_float)

        # if abs(received_float - expected_float) > epsilon:
        #     print("EPSILON:", epsilon)
        #     print("DIFF:", abs(received_float - expected_float))
        return abs(received_float - expected_float) <= epsilon
    except ValueError:
        return received == expected


def are_results_approx_equal(received: Result, expected: Result) -> bool:
    if received.variables != expected.variables:
        return False

    for received_binding, expected_binding in zip_longest(received.bindings, expected.bindings):
        if received_binding is None or expected_binding is None:
            if received_binding != expected_binding:
                return False
            continue

        for received_value, expected_value in zip_longest(received_binding, expected_binding):
            if not is_approx_equal(received_value, expected_value):
                return False

    return True


def print_difference_between_outputs(received_result: Result, expected_result: Result):
    log(Level.OUTPUT, f"{' RECEIVED ':^{CW}}{' EXPECTED ':^{CW}}")
    log(Level.OUTPUT, f"{' VARS ':-^{2*CW}}")

    var_str = f"{received_result.variables!s:^{CW}}{expected_result.variables!s:^{CW}}"
    if received_result.variables != expected_result.variables:
        var_str = f"{colorama.Fore.RED}{var_str}{colorama.Fore.RESET}"
    log(Level.OUTPUT, var_str)

    match = 0
    for received_binding, expected_binding in zip_longest(received_result.bindings, expected_result.bindings):
        match += 1
        log(Level.OUTPUT, f"{f' BINDING {match:>2} ':-^{2*CW}}")

        if received_binding is None:
            received_binding = [None]
        elif len(received_binding) == 0:
            received_binding = ["Empty Binding"]

        if expected_binding is None:
            expected_binding = [None]
        elif len(expected_binding) == 0:
            expected_binding = ["Empty Binding"]

        for received, expected in zip_longest(received_binding, expected_binding):
            is_equal = is_approx_equal(received, expected)
            line = f"{received!s:^{CW}}{expected!s:^{CW}}"
            if not is_equal:
                line = f"{colorama.Fore.RED}{line}{colorama.Fore.RESET}"

            log(Level.OUTPUT, line)


def skip_tests(tests: list[Test], stats: ExecutionStats) -> list[Test]:
    ret: list[Test] = []

    for test in tests:
        if str(test.query.relative_to(TEST_SUITE_DIR)) in IGNORED_TESTS:
            stats.skipped["ignored"] += 1
            log(Level.SKIPPED, "ignored", str(test))
            continue

        if test.data.suffix not in (".gql"):
            stats.skipped["non-gql database"] += 1
            log(Level.SKIPPED, "non-gql database", str(test))
            continue

        ret.append(test)

    return ret


def execute_query_test(server: Popen[bytes] | None, test: QueryTest, stats: ExecutionStats) -> None:
    log(Level.DEBUG, str(test))

    csv_result = execute_query(
        server,
        test,
        stats,
    )

    if isinstance(csv_result, int):
        stats.error += 1
        log(Level.ERROR, str(test), f"Server returned error: {csv_result}")
        return

    assert isinstance(csv_result, str)

    received = get_results_from_csv(csv_result)
    expected = get_results_from_csv_file(test.expected)

    if not query_has_order_by(test.query):
        received.sort()
        expected.sort()

    if are_results_approx_equal(received, expected):
        log(Level.CORRECT, str(test))
        stats.correct += 1
    else:
        log(Level.ERROR, str(test))
        print_difference_between_outputs(received, expected)
        stats.error += 1


def execute_bad_test(server: Popen[bytes] | None, test: BadTest, stats: ExecutionStats) -> None:
    log(Level.DEBUG, str(test))

    csv_result = execute_query(
        server,
        test,
        stats,
    )

    if isinstance(csv_result, str):
        stats.error += 1
        log(Level.ERROR, str(test), "Expected error but server did not return error")
        return

    assert isinstance(csv_result, int)

    log(Level.CORRECT, str(test))
    stats.correct += 1


def execute_tests(
    *,
    executable: Path,
    test_suite: TestSuite,
    client_only: bool = False,
    progress_bar: tqdm | None = None,  # type:ignore
):
    """
    If client_only is True, then no database will be created and no server will be started.
    This can be used to manually start the server with a debugger, and then execute the test script.
    """
    stats = ExecutionStats(test_suite.name)
    if progress_bar:
        progress_bar.set_description(test_suite.name)

    tests_by_data: defaultdict[Path, list[Test]] = defaultdict(list)
    for test in skip_tests(test_suite.tests, stats):
        tests_by_data[test.data].append(test)

    if progress_bar:
        progress_bar.update(sum(stats.skipped.values()))

    for data, tests_ in tests_by_data.items():
        server = None
        database = None
        log_file = None

        if not client_only:
            database = create_db(executable, data)
            server, log_file = start_server(executable, database)

        for test in tests_:
            try:
                if isinstance(test, QueryTest):
                    execute_query_test(server, test, stats)
                elif isinstance(test, BadTest):
                    execute_bad_test(server, test, stats)
            except ServerCrashedException:
                if not client_only:
                    assert server is not None  # server has been started
                    assert log_file is not None  # log_file has been set
                    assert database is not None  # database has been set
                    assert server.poll() is not None  # server process has terminated

                    log_file.flush()
                    log_file.seek(0)
                    for line in log_file:
                        log(Level.SERVER_LOG, "SERVER:", line.strip())
                    log_file.seek(0, SEEK_END)

                    server, log_file = start_server(executable, database)  # restart server
            except Exception as exc:
                if not client_only:
                    assert server is not None  # server has been started
                    kill_server(server)
                raise exc

            if progress_bar:
                progress_bar.update(1)

        if not client_only:
            assert server is not None  # server has been started
            kill_server(server)

    stats.log()
    return stats
