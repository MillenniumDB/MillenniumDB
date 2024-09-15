from __future__ import annotations

from abc import ABC
from dataclasses import dataclass
from pathlib import Path
from typing import Counter, List

from .logging import Level, log
from .options import CWD


class ServerCrashedException(Exception):
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


Binding = List[str]
Bindings = List[Binding]


@dataclass()
class Result:
    variables: List[str]
    bindings: Bindings

    def sort(self):
        self.bindings.sort()


class Test(ABC):
    def __init__(self, query: Path, data: Path):
        self.query = query
        self.data = data

    def __str__(self):
        return f"Test: {self.query} {self.data}"


class BadTest(Test):
    def __str__(self):
        query = self.query.relative_to(CWD)
        database = self.data.name

        return f"BadTest: {query} {database}"


class QueryTest(Test):
    def __init__(
        self,
        *,
        query: Path,
        expected: Path,
        data: Path,
    ):
        super().__init__(query, data)
        self.expected = expected

    def __str__(self):
        query = self.query.relative_to(CWD)
        database = self.data.name
        expected = self.expected.name

        return f"QueryTest: {query} {database} {expected}"


class TestSuite:
    def __init__(self, name: str, tests: list[Test]):
        self.name = name
        self.tests = tests
