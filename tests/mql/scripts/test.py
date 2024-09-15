#!/usr/bin/python3
import argparse
import sys
from pathlib import Path

from testing import options
from testing.file_handler import get_test_suites
from testing.logging import print_summary
from testing.query_execution import execute_tests  # type: ignore
from testing.types import ExecutionStats
from tqdm import tqdm


def run_all_tests(server_executable: Path, create_db_executable: Path):
    stats = ExecutionStats("All Tests")

    test_suites = get_test_suites()

    test_count = sum(len(ts.tests) for ts in test_suites)

    progress_bar = tqdm(
        total=test_count, leave=False, dynamic_ncols=True, bar_format="{l_bar}{bar}| {n_fmt}/{total_fmt}"
    )

    for test_suite in test_suites:
        stats += execute_tests(
            test_suite=test_suite,
            server_executable=server_executable,
            create_db_executable=create_db_executable,
            progress_bar=progress_bar,
            client_only=False,
        )

    progress_bar.close()

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

    sys.exit(run_all_tests(args.server_executable, args.create_db_executable))
