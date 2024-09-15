import re
from abc import ABC
from dataclasses import dataclass
from enum import Enum
from pathlib import Path
from typing import Counter, Dict, List, Optional, Set, Tuple

from .logging import Level, log
from .options import (
    CANONICALIZE_BOOLEANS,
    CANONICALIZE_LANGUAGES,
    CANONICALIZE_NUMBERS,
    CANONICALIZE_URI,
    CWD,
    TRANSLATE_DATATYPES,
)


class ServerCrashedException(Exception):
    pass


class ServerErrorException(Exception):
    pass


class ExecutionStats:
    def __init__(self, name: str):
        self.name = name
        self.correct = 0
        self.skipped: Counter[str] = Counter()
        self.error = 0

    @property
    def total(self):
        return self.correct + sum(self.skipped.values()) + self.error

    def __iadd__(self, other: object):
        assert isinstance(other, ExecutionStats)
        self.correct += other.correct
        self.skipped += other.skipped
        self.error += other.error
        return self

    def log(self):
        width = 45
        col_width = 20
        number_width = 3

        def center(string: str):
            return f"{string:^{width}}"

        log(Level.SUMMARY, f"{f' {self.name} ':-^{width}}")
        log(Level.SUMMARY, "")

        log(Level.SUMMARY, center(f"{'CORRECT':{col_width}}: {self.correct        :>{number_width}}"))
        log(Level.SUMMARY, center(f"{'ERROR'  :{col_width}}: {self.error          :>{number_width}}"))
        log(Level.SUMMARY, center(f"{'SKIPPED':{col_width}}: {sum(self.skipped.values()):>{number_width}}"))

        for reason, count in sorted(self.skipped.items()):
            log(Level.SUMMARY, center(f"  {reason:{col_width-2}}: {count:>{number_width}}"))
        log(Level.SUMMARY, center(f"{'TOTAL':{col_width}}: {self.total:>{number_width}}"))
        log(Level.SUMMARY, "")


XML_SCHEMA = "http://www.w3.org/2001/XMLSchema#"

NUMERIC_PATTERN = re.compile(r"[+-]?(\d+|\.\d+|\d+\.|\d+\.\d+)([eE][+-]?\d+|\d*)")


def remove_prefix(text: str, prefix: str) -> str:
    if text.startswith(prefix):
        return text[len(prefix) :]
    return text


class Value:
    class Type(Enum):
        LITERAL = 1
        IRI = 2
        BNODE = 3
        UNKNOWN = 4

    def __init__(self, value: str, type_: Type, datatype: str, language: str):
        assert isinstance(value, str)
        assert isinstance(type_, Value.Type)
        assert isinstance(datatype, str)
        assert isinstance(language, str)

        if datatype.startswith(XML_SCHEMA):
            datatype = "xsd:" + remove_prefix(datatype, XML_SCHEMA)  # datatype.removeprefix(XML_SCHEMA)

        if CANONICALIZE_NUMBERS and NUMERIC_PATTERN.fullmatch(value):
            value = self.canonicalize_number(value)

        if CANONICALIZE_URI and type_ == Value.Type.IRI:
            value = self.canonicalize_uri(value)

        if type_ == Value.Type.BNODE:
            value = remove_prefix(value, "_:")  # value.removeprefix("_:")

        datatype = TRANSLATE_DATATYPES.get(datatype, datatype)

        if CANONICALIZE_LANGUAGES:
            language = language.lower()

        if CANONICALIZE_BOOLEANS and datatype == "xsd:boolean":
            if value == "0":
                value = "false"
            elif value == "1":
                value = "true"

        self.value = value
        self.type = type_
        self.datatype = datatype
        self.language = language

    def canonicalize_number(self, number_str: str):
        number = float(number_str)
        if number.is_integer():
            return str(int(number))
        return format(number, ".15g")  # Limit precision to avoid unexpected behavior

    def canonicalize_uri(self, uri_str: str):
        if len(uri_str) > 1 and uri_str[0] == "<" and uri_str[-1] == ">":
            return uri_str[1:-1]
        return uri_str

    def __str__(self):
        strings: List[str] = [f"{self.type.name}: "]

        strings.append(f'"{self.value}"')

        if self.datatype:
            if self.datatype.startswith("xsd:"):
                strings.append(f"^^{self.datatype}")
            else:
                strings.append(f"^^<{self.datatype}>")

        if self.language:
            strings.append(f"@{self.language}")

        return "".join(strings)

    def __repr__(self):
        return self.value

    def __eq__(self, other: object):
        if not isinstance(other, Value):
            return False

        if Value.Type.UNKNOWN in (self.type, other.type):
            return self.value == other.value

        return self.value == other.value and self.datatype == other.datatype and self.language == other.language

    def __lt__(self, other: object):
        if not isinstance(other, Value):
            return False

        left = (self.type.value, self.datatype, self.language, self.value)
        right = (other.type.value, other.datatype, other.language, other.value)

        return left < right

    def __gt__(self, other: object):
        return not self == other and not self < other

    def __hash__(self):
        return hash((self.value, self.type.value, self.datatype, self.language))


Binding = Dict[str, Value]
Bindings = List[Binding]


@dataclass()
class Result:
    variables: Set[str]
    bindings: Bindings

    def sort(self):
        self.bindings.sort(key=lambda binding: [binding.get(var, None) for var in sorted(self.variables)])


Triple = Tuple[Value, Value, Value]


class Test(ABC):
    def __init__(self, *, query: Path, graph_output: bool = False):
        self.query = query
        self.graph_output = graph_output


class BadTest(Test):
    def __str__(self):
        return f"BadTest: {self.query.relative_to(CWD)}"


class QueryTest(Test):
    def __init__(
        self,
        *,
        query: Path,
        expected: Path,
        data: Optional[Path] = None,
        named_graphs: Optional[List[Path]] = None,
        prefixes: Optional[Path] = None,
        graph_output: bool = False,
    ):
        if named_graphs is None:
            named_graphs = []
        super().__init__(query=query, graph_output=graph_output)
        self.expected = expected
        self.data = data
        self.named_graphs = named_graphs
        self.prefixes = prefixes

    def __str__(self):
        strings: list[str] = ["QueryTest:"]

        query = self.query.relative_to(CWD)
        strings.append(str(query))

        data = "None" if self.data is None else self.data.name
        strings.append(data)

        strings.append(self.expected.name)

        return " ".join(strings)


class TestSuite:
    def __init__(self, name: str, tests: List[Test]):
        self.name = name
        self.tests = tests
