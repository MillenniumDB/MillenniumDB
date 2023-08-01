MillenniumDB
================================================================================
MillenniumDB is a graph oriented database management system developed by the [Millennium Institute for Foundational Research on Data (IMFD)](https://imfd.cl/).

The main objective of this project is to create a fully functional and easy-to-extend DBMS that serves as the basis for testing new techniques and algorithms related to databases and graphs. We aim to support multiple graph models. RDF/SPARQL support is fairly complete and we are also working on a variation of property graphs.

This project is still in active development and is not production ready yet, some functionality is missing and there may be bugs.



Table of Contents
================================================================================
- [Other Documents](#other-documents)
- [SPARQL Support](#sparql-support)
- [Project Build](#project-build)
- [Using MillenniumDB](#using-millenniumdb)
- [Example](#example)
- [Docker](#docker)



[Other Documents](#millenniumdb)
================================================================================
- [Developer Documentation](doc/developer.md)
- [SPARQL Deviations](doc/sparql/sparql_deviations.md)



[SPARQL Support](#millenniumdb)
================================================================================
Currently Unsupported SPARQL Features
--------------------------------------------------------------------------------
- Updates
- Named graphs
- The `FROM` clause
- The `GRAPH` keyword
- Regular expression flags other than `i`


Deviations from the SPARQL Specification
--------------------------------------------------------------------------------
- **Language tag** (`@`) handling is **case sensitive** for `JOIN`s and related operators, but in **expressions** it is **case insensitive**.
- We do **not** store the exact **lexical representation** of numeric datatypes, only the **numeric value**. For example, `"01"^^xsd:integer` and `"1"^^xsd:integer` are identical in MillenniumDB.
- Our implementation uses **ECMAScript** regular expressions, not **Perl** regular expressions.
- We do not differentiate between `"0"^^xsd:boolean` and `false` / `"false"^^xsd:boolean` or between `"1"^^xsd:boolean` and `true` / `"true"^^xsd:boolean`.

This is explained in more detail [here](doc/sparql/sparql_deviations.md).



[Project build](#millenniumdb)
================================================================================
MillenniumDB should be able to be built on any x86-64 Linux distribution.
On windows, Windows Subsystem for Linux (WSL) can be used. On Mac or Windows without WSL, Docker can be used: see [Docker](#docker).


Install Dependencies:
--------------------------------------------------------------------------------
MillenniumDB needs at least the following GCC and CMake versions:
- GCC version 8.1 or newer
- CMake version 3.12 or newer

Additionally `Git` and `libssl-dev` are needed.

On distributions based on **Ubuntu 20.04 or newer** they can be installed by running:
```
sudo apt update && sudo apt install git g++ cmake libssl-dev
```

On other Linux distributions it may be necessary to install the dependencies differently. Some distributions might have repositories with older versions and the project won't compile, in that case the dependencies will have to be installed manually.

The `en_US.UTF-8` locale also needs to be generated, which can be done as follow:
```
sudo locale-gen en_US.UTF-8
sudo update-locale
```


Clone the repository
--------------------------------------------------------------------------------
 Clone this repository, enter the repository root directory and set `MDB_HOME`:
```
git clone git@github.com:MillenniumDB/MillenniumDB.git
cd MillenniumDB
export MDB_HOME=$(pwd)
```


Install Boost
--------------------------------------------------------------------------------
Download `boost_1_81_0.tar.gz` from [https://www.boost.org/users/history/version_1_81_0.html](https://www.boost.org/users/history/version_1_81_0.html) and then extract it:
```
tar -xf boost_1_81_0.tar.gz
```
Enter the boost directory:
```
cd boost_1_81_0/
```

Run the following:
```
./bootstrap.sh --prefix=$MDB_HOME/third_party/boost_1_81
```
```
./b2 --prefix=$MDB_HOME/third_party/boost_1_81
```
```
./b2 install
```


Build the Project:
--------------------------------------------------------------------------------
Configure and build MillenniumDB:
```
cmake -B build/Release -D CMAKE_BUILD_TYPE=Release && cmake --build build/Release/
```
To use multiple cores during compilation (much faster) use the following command and replace `<n>` with the desired number of threads:
```
cmake -B build/Release -D CMAKE_BUILD_TYPE=Release && cmake --build build/Release/ -j <n>
```



[Using MillenniumDB](#millenniumdb)
================================================================================
MillenniumDB supports two database formats: RDF and QuadModel. A RDF database can only be queried with SPARQL and a QuadModel database can only be queried with MQL. In this document we will focus on RDF/SPARQL.


Creating a Database
--------------------------------------------------------------------------------
```
build/Release/bin/create_db_sparql <data-file> <db-directory> [--prefixes <prefixes-file>]
```
- `<data-file>` is the path to the file containing the data to import, in the [Turtle](https://www.w3.org/TR/turtle/) format.
- `<db-directory>` is the path of the directory where the new database will be created.
- `--prefixes <prefixes-file>` is an optional path to a prefixes file.

### Prefix Definitions
The optional prefixes file passed using the `--prefixes` option contains one prefix per line. Each line consists of a prefix alias and the prefix itself:

```
prefix1: http://www.myprefix.com/
prefix2: https://other.prefix.com/foo
prefix3: https://other.prefix.com/bar
```

Using a prefix file is optional, but helps reduce the space occupied by IRIs in the database. MillenniumDB generates IDs for each prefix, and when importing IRIs into the database replaces any prefixes with IDs. For large databases this can safe a significant amount of space.


Querying a Database
--------------------------------------------------------------------------------
We implement the typical client/server model, so in order to query a database, you need to have a server running and then send queries to it.

### Run the Server
To run the server use the following command, passing the `<db-directory>` where the database was created:
```
build/Release/bin/server_sparql <db-directory>
```

### Execute a Query
The MillenniumDB SPARQL server supports all three query operations specified in the [SPARQL 1.1 Protocol](https://www.w3.org/TR/2013/REC-sparql11-protocol-20130321/#query-operation):
- `query via GET`
- `query via URL-encoded POST`
- `query via POST directly`

We also provide a Python script that makes queries using the `SparqlWrapper` library.
To use it you have to install `SparqlWrapper`:
```
pip3 install sparqlwrapper
```
You can then use the script to make queries as follows:
```
python3 scripts/sparql_query.py <query-file>
```
where `<query-file>` is the path to a file containing a query in SPARQL format.



[Example](#millenniumdb)
================================================================================
This is a step by step example of creating a database, running the server and making a query.
To run this example MillenniumDB has to be [built](#project-build) first.


Create an Example Database
--------------------------------------------------------------------------------
From the repository root directory run the following command to create the example database:
```
build/Release/bin/create_db_sparql data/example-rdf-database.ttl data/example-rdf-database
```
That should have created the directory `data/example-rdf-database` containing a database initialized with the data from `data/example-rdf-database.ttl`.


Launch the Server
--------------------------------------------------------------------------------
The server can now be launched with the previously created database:
```
build/Release/bin/server_sparql data/example-rdf-database
```


Execute a Query
--------------------------------------------------------------------------------
If not already done previously, install SparqlWrapper:
```
pip3 install sparqlwrapper
```
Open another terminal and enter the repository root directory.
Then run the following command to execute an example query:
```
python3 scripts/sparql_query.py data/example-sparql-query.rq
```
The query result should be shown in the terminal.


Remove the Database
--------------------------------------------------------------------------------
To remove the database that was created just delete the directory:
```
rm -r data/example-rdf-database
```



[Docker](#millenniumdb)
================================================================================
We also supply a Dockerfile to build and run MillenniumDB using Docker.


Build the Docker Image
--------------------------------------------------------------------------------
To build a Docker image of MillenniumDB run the following:
```
docker build -t mdb.backend .
```


Creating a Database
--------------------------------------------------------------------------------
Put any `.ttl` files into the `data` directory and from the repository root directory run:
```
docker run --rm --volume "$PWD"/data:/data mdb.backend \
    /MillenniumDB/build/Release/bin/create_db_sparql \
    /data/example-rdf-database.ttl \
    /data/example-rdf-database
```

You can change `/data/example-rdf-database.ttl` to the path of of your `.ttl` and
`/data/example-rdf-database` to the directory where you want the database to be
created. The `.ttl` files and database directories have to be inside `data`.


Running a Server
--------------------------------------------------------------------------------
To run the server with the previously created database use:
```
docker run --rm --volume "$PWD"/data:/data --network="host" mdb.backend \
    /MillenniumDB/build/Release/bin/server_sparql \
    /data/example-rdf-database
```


Executing a Query
--------------------------------------------------------------------------------
If not already done previously, install SparqlWrapper:
```
pip3 install sparqlwrapper
```

A query can then be made as follow:
```
python3 scripts/sparql_query.py ./data/example-sparql-query.rq
```


Remove the Database
--------------------------------------------------------------------------------
To remove the database that was created just delete the directory:
```
sudo rm -r data/example-rdf-database
```
Depending on your Docker configuration you may have to use sudo.