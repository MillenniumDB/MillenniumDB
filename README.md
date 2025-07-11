# MillenniumDB

MillenniumDB is a **graph oriented database management system** developed by the [Millennium Institute for Foundational Research on Data (IMFD)](https://imfd.cl/).

The main objective of this project is to create a fully functional and easy-to-extend DBMS that serves as the basis for testing new techniques and algorithms related to databases and graphs. We support multiple graph models and query languages:

- RDF, with a fairly complete support for the SPARQL 1.1 standard. See [this document](https://github.com/MillenniumDB/MillenniumDB/wiki/SPARQL-Implementation-Status) for more info.
- We have two models for property graphs:

  - Property Graphs with a single edge label and directed edges only, with a custom Cypher-like language (we call this Quad Model or QM in the docs).

  - Property Graphs specified by the GQL Standard (undirected edges, 0-N edge labels), with an early implementation of GQL, still missing a lot of functionality and optimizations.

This project is still in active development and is not production ready yet, some functionality is missing and there may be bugs.

To learn more about MillenniumDB and how to use it, see our [Wiki](https://github.com/MillenniumDB/MillenniumDB/wiki).
