# Text Search Index

The Text Search Index lets users create indexes based on an RDF predicate or MQL property to efficiently search and retrieve nodes from the database.

The index support updates, so each time a node is inserted, modified or removed the index is automatically updated if needed.

## Create a Text Search Index

In SPARQL the Text Search Index can be created via an update request and requires a name `string`, a predicate `iri` and optionally the normalization and/or tokenization type keywords:

```text
# Create with the default settings
CREATE TEXT INDEX "example" ON <https://www.w3.org/2000/01/rdf-schema#label>

# Create with a custom normalization and tokenization. PREFIX statement is also allowed
PREFIX rdfs: <https://www.w3.org/2000/01/rdf-schema#>
CREATE TEXT INDEX "example" ON rdfs:label WITH NORMALIZE <normalization-type> TOKENIZE <tokenization-type>
```

In MQL the Text SearchIndex is created as a regular query and requires a name `string`, a property `identifier` and optionally the normalization and/or tokenization type keywords:

```text
# Create with the default settings
CREATE TEXT INDEX "example" ON label

# Create with a custom normalization and tokenization
CREATE TEXT INDEX "example" ON label WITH NORMALIZE <normalization-type> TOKENIZE <tokenization-type>
```

### Normalization types

Given the string to normalize `"El Pingüino"`, the following table demonstrates the available normalization types:

| Type                      | Description                                | Output          |
| ------------------------- | ------------------------------------------ | --------------- |
| `IDENTITY`                | Keep the same string unchanged             | `"El Pingüino"` |
| `NFKD_CASEFOLD (DEFAULT)` | Apply NFKD, remove diacritics and casefold | `"el pinguino"` |

### Tokenization Types

Given the string to tokenize `Spider-Man 3 (2007)`, the following table demonstrates the available tokenization types:

| Type                       | Description                                 | Output                           |
| -------------------------- | ------------------------------------------- | -------------------------------- |
| `IDENTITY`                 | Keep the entire string as one token         | `["Spider-Man 3 (2007)"]`        |
| `WS_SPLIT_PUNCT (DEFAULT)` | Split on whitespace and punctuation         | `["Spider", "Man", "3", "2007"]` |
| `WS_RM_PUNCT`              | Split on whitespace and remove punctuations | `["SpiderMan", "3", "2007"]`     |
| `WS_KEEP_PUNCT`            | Split on whitespace and keep punctuations   | `["Spider-Man", "3", "(2007)"]`  |

## Querying a Text Search Index

If you have no previous information about the database maybe you want to know which Text Search Indexes are available. Both SPARQL and MQL provide the following query to get each name of the available Text Search Indexes and their metadata:

```text
SHOW TEXT INDEXES
```

For querying in SPARQL we have the reserved IRI `http://millenniumdb.com/function/textSearch` (or for short `mdbfn:textSearch`) to instantiate a Text Search Index Scan. It receives as arguments:

- `(str)` Text Search Index name
- `(str)` Query string
- `(str)` Query mode
- `(var, optional)` Variable where the matched text is projected

For example:

```sparql
PREFIX mdbfn: <http://millenniumdb.com/function/>
SELECT ?subject ?matchedText
WHERE {
    ?subject mdbfn:textSearch ("example" "Spider" "PREFIX" ?matchedText) .
}
```

On the other hand, for querying in MQL we have the expression `TEXT_SEARCH` to instantiate a Text Search Index Scan. It receives as arguments:

- `(str)` Text Search Index name
- `(str)` Query string
- `(keyword)` Query mode
- `(var)` Variable where the found nodes are projected
- `(var, optional)` Variable where the matched text is projected

For example:

```txt
MATCH (?node)
WHERE TEXT_SEARCH("example", "Spider", "PREFIX", ?node, ?matchedText)
RETURN ?node, ?matchedText
```

The available query modes are:

- `MATCH`: The query string must match the whole text
- `PREFIX`: The query string can match a prefix of the text
