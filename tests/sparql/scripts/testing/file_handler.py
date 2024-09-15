from __future__ import annotations

import csv
import json
import re
import sys
from pathlib import Path
from typing import Any, Dict, List, Optional, Set, Tuple
from xml.etree import ElementTree

import rdflib
import rdflib.collection
import rdflib.term
import rdflib.util

from . import options
from .logging import Level, log
from .options import CWD, INTERNAL_TESTS, ROOT_MANIFESTS, TEST_SUITE_DIR
from .types import (
    BadTest,
    Binding,
    Bindings,
    QueryTest,
    Result,
    Test,
    TestSuite,
    Triple,
    Value,
)

# RDF IRIs
IRIS = {
    "RDF": rdflib.URIRef("http://www.w3.org/1999/02/22-rdf-syntax-ns#"),
    "RDFS": rdflib.URIRef("ttp://www.w3.org/2000/01/rdf-schema#"),
    "MF": rdflib.URIRef("http://www.w3.org/2001/sw/DataAccess/tests/test-manifest#"),
    "QT": rdflib.URIRef("http://www.w3.org/2001/sw/DataAccess/tests/test-query#"),
}

# XML name spaces
XMLNS = {
    "rs": "http://www.w3.org/2001/sw/DataAccess/tests/result-set#",
    "rdf": "http://www.w3.org/1999/02/22-rdf-syntax-ns#",
    "xs": "http://www.w3.org/2001/XMLSchema#",
    "xsd": "http://www.w3.org/2001/XMLSchema#",
    "sparql": "http://www.w3.org/2005/sparql-results#",
    "xml": "http://www.w3.org/XML/1998/namespace",
}


def apply_ns(namespace: str, val: str) -> str:
    namespace = XMLNS[namespace]
    return f"{{{namespace}}}{val}"


def node_to_path(uri: rdflib.term.Node) -> Path:
    assert isinstance(uri, rdflib.URIRef)

    protocol, path = str(uri).split("://", maxsplit=1)
    assert protocol == "file"
    path = Path(path)
    assert path.is_file()
    return path


def get_tests_from_root_manifest(root_manifest: Path) -> TestSuite:
    log(Level.BEGIN, f"ROOT MANIFEST: {root_manifest.relative_to(CWD)}")

    test_list: List[Test] = []

    for manifest in get_manifests_from_root_manifest(root_manifest):
        log(Level.BEGIN, f"MANIFEST: {manifest.relative_to(CWD)}")

        for test in get_tests_from_manifest(manifest):
            log(Level.DEBUG, str(test))
            test_list.append(test)

        log(Level.END, f"MANIFEST: {manifest.relative_to(CWD)}")

    log(Level.END, f"ROOT MANIFEST: {root_manifest.relative_to(CWD)}")
    name = str(root_manifest.relative_to(TEST_SUITE_DIR))
    return TestSuite(name, test_list)


def get_manifests_from_root_manifest(path: Path) -> List[Path]:
    graph = rdflib.Graph()
    graph.parse(path)

    main_node = next(graph.subjects(predicate=IRIS["RDF"] + "type", object=IRIS["MF"] + "Manifest"))

    paths_bnode = next(graph.objects(subject=main_node, predicate=IRIS["MF"] + "include"))
    assert isinstance(paths_bnode, rdflib.BNode)

    path_nodes = rdflib.collection.Collection(graph, paths_bnode)
    assert len(path_nodes) > 0

    path_list: List[Path] = []
    for path_node in path_nodes:
        path = node_to_path(path_node)
        path_list.append(path)

    return path_list


def get_tests_from_manifest(path: Path) -> List[Test]:
    graph = rdflib.Graph()
    graph.parse(path)

    tests: List[Test] = []
    for test_node in graph.subjects(predicate=IRIS["RDF"] + "type", object=IRIS["MF"] + "QueryEvaluationTest"):
        action_node = next(graph.objects(subject=test_node, predicate=IRIS["MF"] + "action"))
        assert isinstance(action_node, rdflib.BNode)

        query_node = next(graph.objects(subject=action_node, predicate=IRIS["QT"] + "query"))
        query = node_to_path(query_node)

        default_graph_node = next(graph.objects(subject=action_node, predicate=IRIS["QT"] + "data"), None)
        data = None if default_graph_node is None else node_to_path(default_graph_node)

        named_graph: List[Path] = []
        for named_graph_node in graph.objects(subject=action_node, predicate=IRIS["QT"] + "graphData"):
            named_graph.append(node_to_path(named_graph_node))

        expected_node = next(graph.objects(subject=test_node, predicate=IRIS["MF"] + "result"))
        expected = node_to_path(expected_node)

        graph_output = False
        if query_has_keywords(query, ["describe", "construct"]):
            graph_output = True

        tests.append(
            QueryTest(query=query, data=data, named_graphs=named_graph, expected=expected, graph_output=graph_output)
        )

    return tests


def get_official_tests() -> List[TestSuite]:
    test_suites: List[TestSuite] = []
    for root_manifest in ROOT_MANIFESTS:
        test_suite = get_tests_from_root_manifest(root_manifest)
        test_suites.append(test_suite)

    return test_suites


def get_internal_tests() -> TestSuite:
    log(Level.BEGIN, "GET INTERNAL TESTS")

    test_list: List[Test] = []
    for test_dir in (TEST_SUITE_DIR / "internal").iterdir():
        if test_dir.name not in INTERNAL_TESTS:
            continue

        log(Level.BEGIN, f"{test_dir.name}")

        test_name = test_dir.name
        data = test_dir / f"{test_name}.ttl"
        prefixes = test_dir / f"{test_name}_prefixes.txt"
        if not prefixes.is_file():
            prefixes = None

        for query in test_dir.glob("good_queries/**/*.rq"):
            if query_has_keywords(query, ["describe", "construct"]):
                expected = query.with_suffix(".ttl")
                if not expected.is_file():
                    print(f"File not found: {expected}")
                    sys.exit(1)
                graph_output = True
            else:
                expected = query.with_suffix(".json")
                if not expected.is_file():
                    print(f"File not found: {expected}")
                    sys.exit(1)
                graph_output = False

            test = QueryTest(query=query, data=data, prefixes=prefixes, expected=expected, graph_output=graph_output)
            log(Level.DEBUG, str(test))
            test_list.append(test)

        for query in test_dir.glob("bad_queries/**/*.rq"):
            test = BadTest(query=query)
            log(Level.DEBUG, str(test))
            test_list.append(test)

        log(Level.END, f"{test_dir.name}")

    log(Level.END, "GET INTERNAL TESTS")
    return TestSuite("Internal tests", test_list)


def get_select_result_from_srx(path: Path) -> Result:
    root_ele = ElementTree.parse(path).getroot()

    head_ele = root_ele.find("sparql:head", XMLNS)
    assert isinstance(head_ele, ElementTree.Element)

    results_ele = root_ele.find("sparql:results", XMLNS)
    assert isinstance(results_ele, ElementTree.Element)

    variables: Set[str] = set()
    for variable_ele in head_ele:
        variable = variable_ele.get("name")
        assert variable is not None
        variables.add(variable)

    bindings: Bindings = []
    for result_ele in results_ele:
        binding: Binding = {}
        for binding_ele in result_ele:
            variable = binding_ele.get("name")
            assert variable is not None

            if len(binding_ele) == 0:
                # unbound variable
                continue
            value_ele = binding_ele[0]

            value = value_ele.text or ""

            if value_ele.tag == apply_ns("sparql", "uri"):
                type_ = Value.Type.IRI
            elif value_ele.tag == apply_ns("sparql", "literal"):
                type_ = Value.Type.LITERAL
            elif value_ele.tag == apply_ns("sparql", "bnode"):
                type_ = Value.Type.BNODE
            else:
                raise Exception(f"Unknown type: {value_ele.tag}")

            datatype = value_ele.get("datatype", "")
            language = value_ele.get(apply_ns("xml", "lang"), "")

            binding[variable] = Value(value, type_, datatype, language)
        bindings.append(binding)

    return Result(variables, bindings)


def get_select_result_from_rdf(path: Path) -> Result:
    tree = ElementTree.parse(path)
    root = tree.getroot()
    result_set_ele = root.find("rs:ResultSet", XMLNS)

    # result_set_ele = ElementTree.parse(path).getroot().find("rs:ResultSet", XMLNS)
    assert isinstance(result_set_ele, ElementTree.Element)

    variables: Set[str] = set()
    for variable_ele in result_set_ele.findall("rs:resultVariable", XMLNS):
        assert variable_ele.text is not None
        variables.add(variable_ele.text)

    idx_bindings: List[Tuple[int, Binding]] = []
    for result_ele in result_set_ele.findall("rs:solution", XMLNS):
        binding: Binding = {}
        for binding_ele in result_ele.findall("rs:binding", XMLNS):
            variable_ele = binding_ele.find("rs:variable", XMLNS)
            assert variable_ele is not None

            variable = variable_ele.text
            assert variable is not None

            value_ele = binding_ele.find("rs:value", XMLNS)
            assert value_ele is not None

            language = ""
            datatype = ""
            attrs = value_ele.attrib
            if (apply_ns("rdf", "nodeID")) in attrs:
                attr = apply_ns("rdf", "nodeID")
                value = attrs[attr]
                type_ = Value.Type.BNODE
            elif (apply_ns("rdf", "resource")) in attrs:
                attr = apply_ns("rdf", "resource")
                value = attrs[attr]
                type_ = Value.Type.IRI
            elif (apply_ns("rdf", "datatype")) in attrs:
                attr = apply_ns("rdf", "datatype")
                value = value_ele.text
                type_ = Value.Type.LITERAL
                datatype = attrs[attr]
            elif (apply_ns("xml", "lang")) in attrs:
                attr = apply_ns("xml", "lang")
                value = value_ele.text
                type_ = Value.Type.LITERAL
                language = attrs[attr]
            elif len(attrs) == 0:
                value = value_ele.text
                type_ = Value.Type.LITERAL
            else:
                raise Exception("Unknown value type")

            assert value is not None
            binding[variable] = Value(value, type_, datatype, language)

        idx_ele = result_ele.find("rs:index", XMLNS)
        if idx_ele is not None:
            assert idx_ele.text is not None
            idx = int(idx_ele.text)
        else:
            idx = -1
        idx_bindings.append((idx, binding))

    idx_bindings.sort()
    bindings: Bindings = [result for _, result in idx_bindings]

    return Result(variables, bindings)


def get_select_result_from_ttl(path: Path) -> Result:
    graph = rdflib.Graph(base=None)
    graph.parse(path, publicID=None)

    rs_prefix = "http://www.w3.org/2001/sw/DataAccess/tests/result-set#"
    solution_node = rdflib.URIRef(rs_prefix + "solution")
    binding_node = rdflib.URIRef(rs_prefix + "binding")
    index_node = rdflib.URIRef(rs_prefix + "index")
    variable_node = rdflib.URIRef(rs_prefix + "variable")
    value_node = rdflib.URIRef(rs_prefix + "value")
    result_variable_node = rdflib.URIRef(rs_prefix + "resultVariable")

    results: List[rdflib.term.Node] = []
    for _, binding_obj in graph.subject_objects(predicate=solution_node):
        results.append(binding_obj)

    variables: Set[str] = set()
    for _, binding_obj in graph.subject_objects(predicate=result_variable_node):
        variables.add(str(binding_obj))

    idx_bindings: List[Tuple[int, Binding]] = []
    ordered = False
    for result in results:
        binding: Binding = {}
        for binding_obj in graph.objects(subject=result, predicate=binding_node):
            var_obj = graph.objects(subject=binding_obj, predicate=variable_node).__next__()
            val_obj = graph.objects(subject=binding_obj, predicate=value_node).__next__()

            language = ""
            datatype = ""
            if isinstance(val_obj, rdflib.BNode):
                type_ = Value.Type.BNODE
                value = str(val_obj)
            elif isinstance(val_obj, rdflib.URIRef):
                type_ = Value.Type.IRI
                value = str(val_obj)
            elif isinstance(val_obj, rdflib.Literal):
                type_ = Value.Type.LITERAL
                value = str(val_obj)
                if val_obj.language is not None:
                    language = val_obj.language
                if val_obj.datatype is not None:
                    datatype = str(val_obj.datatype)
            else:
                raise Exception(f"Unknown value: {str(val_obj)}")

            binding[str(var_obj)] = Value(value, type_, datatype, language)

        idx = next(graph.objects(subject=result, predicate=index_node), None)
        if idx is not None:
            ordered = True
            idx_bindings.append((int(str(idx)), binding))
        else:
            assert not ordered
            idx_bindings.append((0, binding))

    if ordered:
        idx_bindings.sort()

    bindings: Bindings = [binding for _, binding in idx_bindings]

    return Result(variables, bindings)


def get_select_result_from_json(loaded_json: Any) -> Result:
    type_from_string = {
        "literal": Value.Type.LITERAL,
        "uri": Value.Type.IRI,
        "bnode": Value.Type.BNODE,
    }

    variables: Set[str] = set(loaded_json["head"]["vars"])
    bindings: Bindings = []

    result: Dict[str, Dict[str, str]]
    for result in loaded_json["results"]["bindings"]:
        binding: Binding = {}

        for variable, value_dict in result.items():
            value = value_dict["value"]
            type_ = type_from_string[value_dict["type"]]
            datatype = value_dict.get("datatype", "")
            language = value_dict.get("xml:lang", "")

            binding[variable] = Value(value, type_, datatype, language)

        bindings.append(binding)
    return Result(variables, bindings)


def get_select_result_from_srj(path: Path) -> Result:
    with open(path, encoding="utf-8") as file:
        loaded_json = json.load(file)
    return get_select_result_from_json(loaded_json)


def get_select_result_from_csv(path: Path) -> Result:
    with open(path, encoding="utf-8") as file:
        reader = csv.reader(file)
        variables = next(reader)
        bindings: Bindings = []
        for line in reader:
            binding: Binding = {}
            for var, val in zip(variables, line):
                if val:
                    binding[var] = Value(val, Value.Type.UNKNOWN, "", "")
            bindings.append(binding)
        return Result(set(variables), bindings)


def get_select_result_from_tsv(path: Path) -> Result:
    with open(path, encoding="utf-8") as file:
        result_data = csv.reader(file, delimiter="\t", quoting=csv.QUOTE_NONE)

        # Remove ? prefixes
        variables = [variable[1:] for variable in next(result_data)]

        bindings: Bindings = []
        for line in result_data:
            binding: Binding = {}
            for variable, value_str in zip(variables, line):
                if value_str:
                    value_node = rdflib.util.from_n3(value_str)
                    language = ""
                    datatype = ""
                    if isinstance(value_node, rdflib.BNode):
                        type_ = Value.Type.BNODE
                        value_str = str(value_node)
                    elif isinstance(value_node, rdflib.URIRef):
                        type_ = Value.Type.IRI
                        value_str = str(value_node)
                    elif isinstance(value_node, rdflib.Literal):
                        type_ = Value.Type.LITERAL
                        value_str = str(value_node)
                        if value_node.language is not None:
                            language = value_node.language
                        if value_node.datatype is not None:
                            datatype = str(value_node.datatype)
                    else:
                        raise Exception(f"Unknown value: {str(value_node)}")
                    binding[variable] = Value(value_str, type_, datatype, language)
            bindings.append(binding)
        return Result(set(variables), bindings)


def get_ask_result_from_ttl(path: Path) -> bool:
    graph = rdflib.Graph(base=None)
    graph.parse(path, publicID=None)

    boolean_node = rdflib.URIRef(XMLNS["rs"] + "boolean")

    _, value = next(graph.subject_objects(predicate=boolean_node))
    value = str(value)

    if value == "true":
        return True
    elif value == "false":
        return False
    else:
        raise Exception(f"Invalid ASK query result: {value}")


def get_ask_result_from_srx(path: Path) -> bool:
    boolean = ElementTree.parse(path).getroot().find("sparql:boolean", XMLNS)

    if boolean is None or boolean.text is None:
        raise Exception(f"Invalid ASK query result: {path}")

    if boolean.text == "true":
        return True
    if boolean.text == "false":
        return False

    raise Exception(f"Invalid ASK query result: {path}")


def get_ask_result_from_json(loaded_json: Any) -> bool:
    if "boolean" in loaded_json:
        return loaded_json["boolean"]

    raise Exception(f"Invalid ASK query result: {loaded_json}")


def get_ask_result_from_srj(path: Path) -> bool:
    with path.open(encoding="utf-8") as file:
        loaded_json = json.load(file)
    return get_ask_result_from_json(loaded_json)


def node_to_value(node: rdflib.term.Node):
    language = ""
    datatype = ""
    if isinstance(node, rdflib.BNode):
        type_ = Value.Type.BNODE
        value = str(node)
    elif isinstance(node, rdflib.URIRef):
        type_ = Value.Type.IRI
        value = str(node)
    elif isinstance(node, rdflib.Literal):
        type_ = Value.Type.LITERAL
        value = str(node)
        if node.language is not None:
            language = node.language
        if node.datatype is not None:
            datatype = str(node.datatype)
    else:
        raise Exception(f"Unknown value: {str(node)}")

    return Value(value, type_, datatype, language)


def get_graph_result_from_ttl(data_or_path: str | Path) -> List[Triple]:
    graph = rdflib.Graph(base=None)

    if isinstance(data_or_path, str):
        graph.parse(data=data_or_path, publicID=None)
    else:
        graph.parse(source=data_or_path, publicID=None)

    triples: List[Triple] = []
    for sub, pred, obj in graph:
        triple: Triple = (node_to_value(sub), node_to_value(pred), node_to_value(obj))
        triples.append(triple)

    return triples


def get_select_result_from_file(path: Path) -> Result:
    file_handlers = {
        ".srx": get_select_result_from_srx,
        ".rdf": get_select_result_from_rdf,
        ".ttl": get_select_result_from_ttl,
        ".csv": get_select_result_from_csv,
        ".tsv": get_select_result_from_tsv,
        ".srj": get_select_result_from_srj,
        ".json": get_select_result_from_srj,
    }

    if path.suffix in file_handlers:
        return file_handlers[path.suffix](path)

    print(f"Unsupported file format: {path}")
    sys.exit(1)


def get_ask_result_from_file(path: Path) -> bool:
    file_handlers = {
        ".srx": get_ask_result_from_srx,
        ".ttl": get_ask_result_from_ttl,
        ".srj": get_ask_result_from_srj,
        ".json": get_ask_result_from_srj,
    }

    if path.suffix in file_handlers:
        return file_handlers[path.suffix](path)

    print(f"Unsupported file format: {path}")
    sys.exit(1)


def query_has_order_by(path: Path) -> bool:
    with path.open(encoding="utf-8") as file:
        query_string = " ".join(file.readlines()).lower()
    match = re.search(r"order by", query_string)
    return match is not None


def query_has_not_supported_keyword(path: Path) -> Optional[str]:
    return query_has_keywords(path, options.NOT_SUPPORTED_KEYWORDS)


def query_has_keywords(query_path: Path, keywords: List[str]) -> Optional[str]:
    with query_path.open(encoding="utf-8") as file:
        lines = [" "]
        for line in file.readlines():
            line = line.strip()
            if not line:
                continue
            if line[0] == "#":
                continue
            line = line.lower()
            lines.append(line)
        query_string = " ".join(lines)

    for keyword in keywords:
        keyword = keyword.lower()
        match = re.search(f"[ {{]{keyword}[ {{]", query_string)
        if match is not None:
            return keyword
    return None
