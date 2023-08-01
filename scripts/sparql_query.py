#!/usr/bin/python3

import json
import sys

from SPARQLWrapper import SPARQLWrapper
from SPARQLWrapper.SPARQLExceptions import QueryBadFormed


def send_query(file: str):
    with open(file, "r", encoding="utf-8") as f:
        query = f.read()

    print(query)
    print("-----------------\n")

    sparql = SPARQLWrapper("http://localhost:8080/sparql")

    sparql.setMethod("POST")
    sparql.setRequestMethod("postdirectly")
    sparql.setQuery(query)

    assert sparql.queryType is not None

    if sparql.queryType in ("ASK", "SELECT"):
        sparql.setReturnFormat("json")
    elif sparql.queryType in ("DESCRIBE", "CONSTRUCT"):
        sparql.setReturnFormat("turtle")
    else:
        raise ValueError(f"Unknown queryType: {sparql.queryType}")

    try:
        response = sparql.queryAndConvert()
    except QueryBadFormed:
        print("QueryBadFormed")
        sys.exit(1)

    if isinstance(response, dict):
        # JSON response
        print(json.dumps(response, indent=2))

        if "results" in response:
            # SELECT
            count = len(response["results"]["bindings"])
        elif "boolean" in response:
            # ASK
            count = 1
        else:
            raise ValueError(f"Unknown response format: {response}")

    elif isinstance(response, bytes):
        # TURTLE DESCRIBE response
        response = response.decode()
        count = 0
        for line in response.splitlines():
            print(line)
            count += 1

    else:
        raise ValueError(f"Unknown response format: {response}")

    print("Results:", count)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Please pass query filepath")
        sys.exit(1)

    send_query(sys.argv[1])
