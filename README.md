# Millennium DB
MillenniumDB is a graph oriented database management system developed by the [Millennium Institute for Foundational Research on Data (IMFD)](https://imfd.cl/).

MillenniumDB was created because we though the existent graph models were not enough to represent efficiently the real world data as a graph.

Our main objective with this project is to be a fully functional, easy-to-extend system that serves as the basis for testing new techniques and algorithms related to databases and graphs.

## Main features
- A flexible and more expressive graph [data model](#data-model).
- Efficient property paths solving.
- Worst case optimal join algorithm.

This project is in an early stage of development and is not production ready yet, as it may contain bugs. Also, there are some features we think are important for a graph database and are not supported yet. However we want them to be supported in the future:

- A database can't be modified after its created.
- Only [well-designed patterns](https://users.dcc.uchile.cl/~jperez/papers/tods-a16-perez.pdf) are supported when using the `OPTIONAL` operator.
- More datatypes for properties values, e.g: dates, points and lists.
- Filters only support basic operations (<, <=, ==, != >=, >). We would like to support pattern matching and functions.

___
# Table of Contents
- [Project build](#project-build)
- [Data model](#data-model)
- [Creating a database](#creating-a-database)
- [Querying a database](#querying-a-database)
  * [Run the server](#run-the-server)
  * [Execute a query](#execute-a-query)
___
# Project build

MillenniumDB should be able to be built in any x86-64 linux distribution.
If you work on windows, you can use Windows Subsystem for Linux (WSL).

1. Install prerequisites to compile:

    - Boost Library version 1.71.0 or newer
    - Gcc version 8.1 or newer
    - Cmake version 3.10 or newer

    For distributions based on **Ubuntu 20.04 or newer** this can be done by running:
    - `sudo apt update`
    - `sudo apt install git g++ cmake libboost-all-dev`

     Other linux distributions may need to install the prerequisites differently. Some distributions might have repositories with too old versions and the project won't compile, in that case you'll need to manually install the appropiate versions.

2. Clone this repository and enter to the 'MillenniumDB' folder:

3. Build the project:
    - `cmake -Bbuild/Release -DCMAKE_BUILD_TYPE=Release && cmake --build build/Release/`

# Data model
Our data model is similar to the known *labeled property graph* model. In simplified terms we could say that edges were extended such that the source or destination may be another edge.

To understand better out data model you can read our [data model document](/doc/data_model.md).

# Creating a database
The command to create a new database looks like this:
- `build/Release/bin/create_db [path/to/import_file] [path/to/new_database_folder]`

For instance, if you want to create a database into the folder `tests/dbs/example` using the example we provide in `tests/dbs/example-db.txt` you need to run:
- `build/Release/bin/create_db tests/dbs/example-db.txt tests/dbs/example`

If you want to import a big database you should to specify a custom buffer size with the option `-b` to get a faster import. The parameter tells how many GB will be used as buffer, the default buffer size is 1GB.

To delete a database just manually delete the created folder.

If you want to use your import file read our [import format document](/doc/import.md)

# Querying a database
We implement the typical a client/server model, so in order to query a database, you need to have a server running and then send queries to it.
## Run the server
- `build/Release/bin/server [path/to/database_folder]`

## Execute a query
- `build/Release/bin/query < [path/to/query_file]`
