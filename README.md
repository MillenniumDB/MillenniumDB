MillenniumDB
================================================================================
MillenniumDB is a graph oriented database management system developed by the [Millennium Institute for Foundational Research on Data (IMFD)](https://imfd.cl/).

The main objective of this project is to create a fully functional and easy-to-extend DBMS that serves as the basis for testing new techniques and algorithms related to databases and graphs. We support multiple graph models, RDF/SPARQL support is fairly complete and we have a custom language to work with Property Graphs. We are working to add support for GQL in the near future.

This project is still in active development and is not production ready yet, some functionality is missing and there may be bugs.


Table of Contents
================================================================================
- [Graph Models](#graph-models)
    - [RDF](#rdf-model)
    - [Property Graphs](#property-graph-model)
- [Project Build](#project-build)
- [Using MillenniumDB](#using-millenniumdb)
- [Example](#example)
- [Docker](#docker)


# [Graph Models](#millenniumdb)
We support two different graph models, each graph model has its corresponding query language (i.e. once you import an RDF graph, you must use SPARQL to query it, our property graph query language will not work in that graph).

## [RDF Model](#millenniumdb)

We can import RDF graphs in `.ttl`, `.n3` or `.nt` formats and we support most of  [SPARQL](https://www.w3.org/TR/sparql11-query/), with exceptions explained below.

Currently Unsupported SPARQL Features
--------------------------------------------------------------------------------
- Updates other that [INSERT DATA](https://www.w3.org/TR/2013/REC-sparql11-update-20130321/#insertData) and [DELETE DATA](https://www.w3.org/TR/2013/REC-sparql11-update-20130321/#deleteData)
- Named graphs
- The `FROM` clause
- The `GRAPH` keyword
- Regular expression flags other than `i`
- Property paths with Negated Property Sets

Deviations from the SPARQL Specification
--------------------------------------------------------------------------------
- **Language tag** (`@`) handling is **case sensitive** for `JOIN`s and related operators, but in **expressions** it is **case insensitive**.
- We do **not** store the exact **lexical representation** of numeric datatypes, only the **numeric value**. For example, `"01"^^xsd:integer` and `"1"^^xsd:integer` are identical in MillenniumDB.
    - This implies that expressions that work with the lexical representation may result in a different value. For example `STR(1e0)` should be `"1e0"` according to the standard, but MillenniumDB will evaluate it as `"1.0E0"`.
- We do not differentiate between `"0"^^xsd:boolean` and `false` / `"false"^^xsd:boolean` or between `"1"^^xsd:boolean` and `true` / `"true"^^xsd:boolean`.
- Our implementation uses **ECMAScript** regular expressions, not **Perl** regular expressions.
- The regular path expression `?s :P* ?o` won't return all the nodes in the database that appears as a subject or object in some triple as the standard says. Instead it will only return the nodes that appears as a subject in a triple with predicate `:P`.

This is explained in more detail [here](doc/sparql/sparql_deviations.md).


## [Property Graph Model](#millenniumdb)
The definition of the graph model and how to create a graph file is explained [here](doc/quad_model/data_model.md).
The query language is inspired on Cypher and its defined [here](doc/quad_model/query_language.md).



[Project build](#millenniumdb)
================================================================================
MillenniumDB should be able to be built on any x86-64 Linux distribution.
On windows, Windows Subsystem for Linux (WSL) can be used. MacOS is supported if using a Mac chip.
For Mac with Intel chips or Windows without WSL, Docker can be used: see [Docker](#docker).


Install Dependencies:
--------------------------------------------------------------------------------
MillenniumDB needs the following dependencies:
- GCC >= 8.1
- CMake >= 3.12
- Git
- libssl
- ncursesw and less for the CLI
- Python >= 3.8 with venv to run tests
- Boost 1.82

On current Debian and Ubuntu based distributions they can be installed by running:
```bash
sudo apt update && sudo apt install git g++ cmake libssl-dev libncurses-dev less python3 python3-venv libicu-dev
```

On mac:
```bash
brew install cmake ncurses openssl@3 icu4c
```

Clone the repository
--------------------------------------------------------------------------------
 Clone this repository, enter the repository root directory and set `MDB_HOME`:
```bash
git clone git@github.com:MillenniumDB/MillenniumDB.git
cd MillenniumDB
export MDB_HOME=$(pwd)
```


Install Boost
--------------------------------------------------------------------------------
Download [`boost_1_82_0.tar.gz`](https://archives.boost.io/release/1.82.0/source/boost_1_82_0.tar.gz) using a browser or wget:
```bash
wget -q --show-progress https://archives.boost.io/release/1.82.0/source/boost_1_82_0.tar.gz
```

and run the following in the directory where boost was downloaded:
```bash
tar -xf boost_1_82_0.tar.gz
mkdir -p $MDB_HOME/third_party/boost_1_82/include
mv boost_1_82_0/boost $MDB_HOME/third_party/boost_1_82/include
rm -r boost_1_82_0.tar.gz boost_1_82_0
```



Build the Project:
--------------------------------------------------------------------------------
Go back into the repository root directory and configure and build MillenniumDB:
```bash
cmake -Bbuild/Release -DCMAKE_BUILD_TYPE=Release && cmake --build build/Release/
```
To use multiple cores during compilation (much faster) use the following command and replace `<n>` with the desired number of threads:
```bash
cmake -Bbuild/Release -DCMAKE_BUILD_TYPE=Release && cmake --build build/Release/ -j <n>
```



[Using MillenniumDB](#millenniumdb)
================================================================================
MillenniumDB supports two database formats: RDF and QuadModel. A RDF database can only be queried with SPARQL and a QuadModel database can only be queried with MQL. In this document we will focus on RDF/SPARQL.


Creating a Database
--------------------------------------------------------------------------------
```bash
build/Release/bin/mdb-import <data-file> <db-directory> [--prefixes <prefixes-file>]
```
- `<data-file>` is the path to the file containing the data to import, using the [Turtle](https://www.w3.org/TR/turtle/) format for RDF, or [QuadModel Format](doc/quad_model/data_model.md#import-format) for Property Graphs.
- `<db-directory>` is the path of the directory where the new database will be created.
- `--prefixes <prefixes-file>` is an optional path to a prefixes file (used only when using RDF).

### Prefix Definitions
The optional prefixes file passed using the `--prefixes` option contains one prefix per line. Each line consists of a prefix alias and the prefix itself:

```
http://www.myprefix.com/
https://other.prefix.com/foo
https://other.prefix.com/bar
```

Using a prefix file is optional, but helps reduce the space occupied by IRIs in the database when using the RDF model. MillenniumDB generates IDs for each prefix, and when importing IRIs into the database replaces any prefixes with IDs. For large databases this can safe a significant amount of space. The total number of user defined prefixes cannot exceed 255.


Querying a Database
--------------------------------------------------------------------------------
We implement the typical client/server model, so in order to query a database, you need to have a server running and then send queries to it.

### Run the Server
To run the server use the following command, passing the `<db-directory>` where the database was created:
```bash
build/Release/bin/mdb-server <db-directory>
```

**IMPORTANT:** we supposing you execute the server from the root directory of this repository (`MDB_HOME`). If you execute the server from another directory, the web server won't be available unless set the environment variable `MDB_BROWSER` is `$MDB_HOME/browser`.

### Execute a Query
The easiest way to run a query is to use the Web Browser at http://localhost:4321/ after starting the server.

The other option is sending the query via HTTP request.

The MillenniumDB SPARQL server supports all three query operations specified in the [SPARQL 1.1 Protocol](https://www.w3.org/TR/2013/REC-sparql11-protocol-20130321/#query-operation):
- `query via GET`
- `query via URL-encoded POST`
- `query via POST directly`

When using Property Graphs, we expect an HTTP POST where query is contained in the request body.

We provide a script to make queries using curl.
To use it you have to pass a file with the query as a parameter:
```bash
bash scripts/query <query-file>
```

where `<query-file>` is the path to a file containing a query in SPARQL format.

For updates, we have an analogous script:
```bash
bash scripts/update <query-file>
```


[Example](#millenniumdb)
================================================================================
This is a step by step example of creating a database, running the server and making a query.
To run this example MillenniumDB has to be [built](#project-build) first.


Create an Example Database
--------------------------------------------------------------------------------
From the repository root directory run the following command to create the example database:
```bash
build/Release/bin/mdb-import data/example-rdf-database.ttl data/example-rdf-database
```
That should have created the directory `data/example-rdf-database` containing a database initialized with the data from `data/example-rdf-database.ttl`.


Launch the Server
--------------------------------------------------------------------------------
The server can now be launched with the previously created database:
```
build/Release/bin/mdb-server data/example-rdf-database
```


Execute a Query
--------------------------------------------------------------------------------
Go to http://localhost:4321/

Remove the Database
--------------------------------------------------------------------------------
To remove the database that was created just delete the directory:
```bash
rm -r data/example-rdf-database
```



[Docker](#millenniumdb)
================================================================================
We also supply a Dockerfile to build and run MillenniumDB using Docker.


Build the Docker Image
--------------------------------------------------------------------------------
To build a Docker image of MillenniumDB run the following:
```bash
docker build -t mdb .
```


Creating a Database
--------------------------------------------------------------------------------
Put any `.ttl` files into the `data` directory and from the repository root directory run:
```bash
docker run --rm --volume "$PWD"/data:/data mdb \
    mdb-import \
    /data/example-rdf-database.ttl \
    /data/example-rdf-database
```

You can change `/data/example-rdf-database.ttl` to the path of of your `.ttl` and
`/data/example-rdf-database` to the directory where you want the database to be
created. The `.ttl` files and database directories have to be inside `data`. The `.ttl`
file must not be a symbolic link to a `.ttl` file but a real one. Also the `.ttl` file must exist or else the DB will be created empty.

Running a Server
--------------------------------------------------------------------------------
To run the server with the previously created database use:
```bash
docker run --rm --volume "$PWD"/data:/data -p 1234:1234 -p 4321:4321 mdb \
    mdb-server \
    /data/example-rdf-database
```


Executing a Query
--------------------------------------------------------------------------------
Go to http://localhost:4321/ to see the web interface (available while running the server).

Also we provide a script to make queries using the console:

```bash
bash scripts/query <query-file>
```


Remove the Database
--------------------------------------------------------------------------------
To remove the database that was created just delete the directory:
```bash
rm -r data/example-rdf-database
```
Depending on your Docker configuration you may have to use sudo.
