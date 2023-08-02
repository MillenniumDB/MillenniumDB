from __future__ import annotations

import copy
import itertools
from io import SEEK_END
from itertools import zip_longest
from pathlib import Path
from subprocess import Popen
from typing import (
    Counter,
    DefaultDict,
    Dict,
    Iterator,
    List,
    NamedTuple,
    NoReturn,
    Optional,
    Tuple,
)

import colorama
from testing.file_handler import query_has_not_supported_keyword
from tqdm import tqdm

from .file_handler import (
    get_ask_result_from_file,
    get_ask_result_from_json,
    get_graph_result_from_ttl,
    get_select_result_from_file,
    get_select_result_from_json,
    query_has_order_by,
)
from .logging import Level, log
from .options import IGNORED_TESTS
from .options import OUTPUT_COLUMN_WIDTH as CW
from .options import TEST_SUITE_DIR
from .server_utils import (
    create_db,
    execute_bad_query,
    execute_query,
    kill_server,
    start_server,
)
from .types import (
    BadTest,
    Binding,
    Bindings,
    ExecutionStats,
    QueryTest,
    Result,
    ServerCrashedException,
    Test,
    TestSuite,
    Triple,
    Value,
)


def print_difference_between_outputs(received: Result, expected: Result):
    log(Level.OUTPUT, f"{' RECEIVED ':^{CW}}{' EXPECTED ':^{CW}}")
    log(Level.OUTPUT, f"{' VARS ':-^{2*CW}}")

    var_str = f"{received.variables!s:^{CW}}{expected.variables!s:^{CW}}"
    if received.variables != expected.variables:
        var_str = f"{colorama.Fore.RED}{var_str}{colorama.Fore.RESET}"
    log(Level.OUTPUT, var_str)

    variables = list(set(received.variables) | set(expected.variables))

    match = 0
    for rec, exp in zip_longest(received.bindings, expected.bindings):
        match += 1
        log(Level.OUTPUT, f"{f' BINDING {match:>2} ':-^{2*CW}}")
        for var in variables:
            rec_var = rec.get(var, None) if rec else None
            exp_var = exp.get(var, None) if exp else None

            var_width = len(var) + 2 if len(var) % 2 == 0 else len(var) + 3

            line = f"{rec_var!s:^{CW-var_width//2}}{var:^{var_width}}{exp_var!s:^{CW-var_width//2}}"

            if rec_var != exp_var:
                line = f"{colorama.Fore.RED}{line}{colorama.Fore.RESET}"

            log(Level.OUTPUT, line)


def print_describe_differences(received: List[str], expected: List[str]):
    log(Level.OUTPUT, f"{' RECEIVED ':^{CW}}{' EXPECTED ':^{CW}}")

    for rec, exp in zip_longest(received, expected):
        rec = rec or "None"
        exp = exp or "None"
        string = f"{rec:^{CW}}{exp:^{CW}}"
        if rec != exp:
            string = f"{colorama.Fore.RED}{string}{colorama.Fore.RESET}"
        log(Level.OUTPUT, string)


def print_graph_differences(received: List[Triple], expected: List[Triple]):
    log(Level.OUTPUT, f"{' RECEIVED ':^{2*CW}}")
    log(Level.OUTPUT, f"{' EXPECTED ':^{2*CW}}")

    triple_count = -0
    for rec, exp in zip_longest(received, expected):
        triple_count += 1
        rec = rec or ("None",) * 3
        exp = exp or ("None",) * 3
        rec_str_list: List[str] = []
        exp_str_list: List[str] = []
        for i in range(3):
            rec_val = rec[i]
            exp_val = exp[i]

            rec_str = str(rec_val)
            exp_str = str(exp_val)
            if rec_val != exp_val:
                rec_str = f"{colorama.Fore.RED}{rec_str}{colorama.Fore.RESET}"
                exp_str = f"{colorama.Fore.RED}{exp_str}{colorama.Fore.RESET}"

            rec_str_list.append(rec_str)
            exp_str_list.append(exp_str)

        rec_str = " ".join(rec_str_list)
        exp_str = " ".join(exp_str_list)

        triple_count_str = f" {triple_count} "
        log(Level.OUTPUT, f"{triple_count_str:-^{2*CW}}")
        log(Level.OUTPUT, rec_str)
        log(Level.OUTPUT, exp_str)


def skip_tests(tests: List[Test], stats: ExecutionStats) -> List[Test]:
    ret: List[Test] = []

    for test in tests:
        if str(test.query.relative_to(TEST_SUITE_DIR)) in IGNORED_TESTS:
            stats.skipped["ignored"] += 1
            log(Level.SKIPPED, "ignored", str(test))
            continue

        if key_word := query_has_not_supported_keyword(test.query):
            stats.skipped[f"kw: {key_word}"] += 1
            log(Level.SKIPPED, f"kw: {key_word}", str(test))
            continue

        if isinstance(test, QueryTest):
            if test.named_graphs:
                stats.skipped["has named graph"] += 1
                log(Level.SKIPPED, "has named graph", str(test))
                continue
            if test.data and test.data.suffix not in (".ttl", ".nt", ".n3"):
                stats.skipped["non-ttl database"] += 1
                log(Level.SKIPPED, "non-ttl database", str(test))
                continue

        elif isinstance(test, BadTest):
            pass

        else:
            raise TypeError(f"Invalid test type: {type(test)}")

        ret.append(test)

    return ret


def execute_query_test(server: Optional[Popen[bytes]], test: QueryTest, stats: ExecutionStats) -> None:
    log(Level.DEBUG, str(test))

    json_result = execute_query(server, test, stats)

    if json_result is None:
        return

    assert isinstance(json_result, dict)

    if "results" in json_result:
        received = get_select_result_from_json(json_result)
        expected = get_select_result_from_file(test.expected)

        for potential_received in iterate_result_permutations(received, expected):
            if not query_has_order_by(test.query):
                potential_received.sort()
                expected.sort()

            if potential_received == expected:
                log(Level.CORRECT, str(test))
                stats.correct += 1
                return

        log(Level.ERROR, str(test))
        print_difference_between_outputs(received, expected)
        stats.error += 1

    elif "boolean" in json_result:
        received = get_ask_result_from_json(json_result)
        expected = get_ask_result_from_file(test.expected)
        if received == expected:
            log(Level.CORRECT, str(test))
            stats.correct += 1
        else:
            log(Level.ERROR, str(test))
            log(Level.OUTPUT, f"ASK Result: got {received}, expected {expected}")
            stats.error += 1


def execute_graph_output_test(server: Optional[Popen[bytes]], test: QueryTest, stats: ExecutionStats) -> None:
    log(Level.DEBUG, str(test))

    execute_result = execute_query(server, test, stats)

    if execute_result is None:
        return

    assert isinstance(execute_result, str)

    received_triples = get_graph_result_from_ttl(execute_result)
    expected_triples = get_graph_result_from_ttl(test.expected)

    expected_triples.sort()

    for potential_received in iterate_triples_permutations(received_triples, expected_triples):
        potential_received.sort()

        if potential_received == expected_triples:
            log(Level.CORRECT, str(test))
            stats.correct += 1
            return

    log(Level.ERROR, str(test))
    stats.error += 1
    print_graph_differences(received_triples, expected_triples)


def execute_bad_test(server: Optional[Popen[bytes]], test: BadTest, stats: ExecutionStats):
    log(Level.DEBUG, str(test))

    returned_error = execute_bad_query(server, test, stats)

    if returned_error:
        log(Level.CORRECT, str(test))
        stats.correct += 1
    else:
        log(Level.ERROR, str(test))
        stats.error += 1


def execute_tests(test_suite: TestSuite, *, client_only: bool = False, progress_bar: tqdm[NoReturn] | None = None):
    """
    If client_only is True, then no database will be created and no server will be started.
    This can be used to manually start the server with a debugger, and then execute the test script.
    """
    stats = ExecutionStats(test_suite.name)
    if progress_bar:
        progress_bar.set_description(test_suite.name)

    Key = NamedTuple("Key", [("data", Optional[Path]), ("prefixes", Optional[Path])])

    tests_by_key = DefaultDict[Key, List[Test]](list)
    for test in skip_tests(test_suite.tests, stats):
        data = getattr(test, "data", None)
        prefixes = getattr(test, "prefixes", None)
        key = Key(data, prefixes)
        tests_by_key[key].append(test)

    if progress_bar:
        progress_bar.update(sum(stats.skipped.values()))

    for (data, prefixes), tests_ in tests_by_key.items():
        server = None
        data_path = None
        log_file = None

        if not client_only:
            data_path = create_db(data, prefixes)
            server, log_file = start_server(data_path)

        for test in tests_:
            try:
                if isinstance(test, QueryTest):
                    if test.graph_output:
                        execute_graph_output_test(server, test, stats)
                    else:
                        execute_query_test(server, test, stats)
                elif isinstance(test, BadTest):
                    execute_bad_test(server, test, stats)
                else:
                    raise TypeError(f"Unknown test type: {type(test)}")

            except ServerCrashedException:
                if not client_only:
                    assert server is not None  # server has been started
                    assert log_file is not None
                    assert data_path is not None  # data_path has been set
                    assert server.poll() is not None  # server process has terminated

                    log_file.flush()
                    log_file.seek(0)
                    for line in log_file:
                        log(Level.SERVER_LOG, "SERVER:", line.strip())
                    log_file.seek(0, SEEK_END)

                    server, log_file = start_server(data_path)  # restart server

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


BNodeCounts = List[Tuple[str, int]]
PotentialMappings = List[Tuple[List[str], List[str]]]
Mapping = Dict[str, str]


def count_blank_nodes_in_result(result: Result) -> BNodeCounts:
    blank_nodes = Counter[str]()
    for binding in result.bindings:
        for value in binding.values():
            if value.type == Value.Type.BNODE:
                blank_nodes[value.value] += 1
    return sorted(blank_nodes.items(), key=lambda x: (-x[1], x[0]))


def count_blank_nodes_in_triples(triples: List[Triple]) -> BNodeCounts:
    blank_nodes = Counter[str]()
    for triple in triples:
        for value in triple:
            if value.type == Value.Type.BNODE:
                blank_nodes[value.value] += 1
    return sorted(blank_nodes.items(), key=lambda x: (-x[1], x[0]))


def generate_potential_mappings(bnodes_1: BNodeCounts, bnodes_2: BNodeCounts) -> Optional[PotentialMappings]:
    if len(bnodes_1) != len(bnodes_2):
        return None

    potential_mappings: PotentialMappings = []

    prev_count = -1

    current_keys: List[str] = []
    current_values: List[str] = []

    for (node1, count1), (node2, count2) in zip(bnodes_1, bnodes_2):
        if count1 != count2:
            return None

        if prev_count == count1:
            current_keys.append(node1)
            current_values.append(node2)
        else:
            if len(current_keys) > 0:
                potential_mappings.append((current_keys, current_values))
                current_keys = []
                current_values = []
            current_keys.append(node1)
            current_values.append(node2)
            prev_count = count1

    if len(current_keys) > 0:
        potential_mappings.append((current_keys, current_values))

    return potential_mappings


def iterate_mapping_permutations(
    potential_mappings: PotentialMappings, potential_mapping: Mapping | None = None
) -> Iterator[Mapping]:
    if potential_mapping is None:
        potential_mapping = {}
    # Base case
    if len(potential_mappings) == 0:
        yield potential_mapping
        return

    group = potential_mappings.pop()

    for perm in itertools.permutations(group[1]):
        # Choose the current permutation
        for key, value in zip(group[0], perm):
            potential_mapping[key] = value

        # Try the chosen permutation
        for mapping in iterate_mapping_permutations(potential_mappings, potential_mapping):
            yield mapping

        # Undo the chosen permutation
        for key, value in zip(group[0], perm):
            del potential_mapping[key]

    potential_mappings.append(group)


def replace_nodes_in_result(result: Result, mapping: Mapping):
    new_bindings: Bindings = []

    for binding in result.bindings:
        new_binding: Binding = {}
        for var, value in binding.items():
            value_copy = copy.copy(value)

            if value.type == Value.Type.BNODE:
                value_copy.value = mapping.get(value.value, value.value)

            new_binding[var] = value_copy
        new_bindings.append(new_binding)

    return Result(result.variables.copy(), new_bindings)


def replace_nodes_in_triples(triples: List[Triple], mapping: Mapping):
    new_triples: List[Triple] = []

    for triple in triples:
        values: List[Value] = []
        for value in triple:
            value_copy = copy.copy(value)

            if value.type == Value.Type.BNODE:
                value_copy.value = mapping.get(value.value, value.value)

            values.append(value_copy)
        new_triples.append(tuple(values))

    return new_triples


def iterate_result_permutations(received: Result, expected: Result):
    received_counts = count_blank_nodes_in_result(received)
    expected_counts = count_blank_nodes_in_result(expected)

    if not received_counts or not expected_counts:
        yield received
        return

    potential_mappings = generate_potential_mappings(received_counts, expected_counts)

    if not potential_mappings:
        return

    for potential_mapping in iterate_mapping_permutations(potential_mappings):
        yield replace_nodes_in_result(received, potential_mapping)


def iterate_triples_permutations(received: List[Triple], expected: List[Triple]):
    received_counts = count_blank_nodes_in_triples(received)
    expected_counts = count_blank_nodes_in_triples(expected)

    if not received_counts or not expected_counts:
        yield received
        return

    potential_mappings = generate_potential_mappings(received_counts, expected_counts)

    if not potential_mappings:
        return

    for potential_mapping in iterate_mapping_permutations(potential_mappings):
        yield replace_nodes_in_triples(received, potential_mapping)
