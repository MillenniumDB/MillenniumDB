# The generic data model

Everything in our graph model is an **Object**, and there are different 4 types of objects:

1. **Literals**: they are the classic basic types:
    - **Integer**
    - **String**
    - **Float**
    - **Boolean**

2. **Nodes**:
    they are objects that can have a an **identifier** set of **labels** and **properties**
    - `labels`: a (possibly empty) set of strings.
    - `properties`: a (possibly empty) set of pairs <key, value>. Where the key is a string and the value is a literal. A key cannot appear twice in the set.

    We have 2 types of nodes:

    2.1. **Named Nodes**: they have a string as identifier when you add them into the database.

    2.2. **Anonymous Nodes**: they don't have a string as identifier when you add them into the database. They will have an auto-generated identifier to direcly refeer to them later.


3. **Edges**: an edge is an object that relates other objects, having the following attributes:
    - An auto-generated identifier.
    - `from`: any **Object** (including other edge object but not itself).
    - `to`: any **Object** (including other edge object but not itself).
    - `types`: a (possibly empty) set of **Named Nodes**.
    - `properties`: a (possibly empty) set of pairs <key, value>. Where the key is a string and the value is a literal. A key cannot appear twice in the set.


# Domain restrictions
- **Integer**: integers from -(2<sup>56</sup>-1) to 2<sup>56</sup>-1.

- **String**: sequence of characters. They often are delimited by double quotes (`""`). The only exception is when you are working with keys. For that reason, property keys cannot be any string, they must match the regular expression `[A-Za-z][A-Za-z0-9_]*`.

- **Float**: same as in C language. Use with caution as they may lose presicion.

- **Boolean** `true` or `false`.

- **Named Nodes**: they must match the regular expresion `[A-Za-z][A-Za-z0-9_]*`. There are exceptions for `true` and `false` because they are interpreted as booleans. Identifiers are case sensitive.

- **Anonymous Nodes** The auto-generated identifier will match the regular expresion `_a[1-9][0-9]*`.

- **Edges** The auto-generated identifier will match the regular expresion `_e[1-9][0-9]*`.

# Model constraints
The model presented before is very flexible, but most of the time we have some constrains we could take advantage on.
For that reason, we allow having multiples models depending on what constrains do we have. Depending on the model you use, you end up creating different indexes to store the data and thus, each model has its own query solver.

## Quad Model
Currently this is the only model implemented. The only restriction to the generic model is that **every edge must have one type**.

## Triple Model
We are planning to implkeent this in the future, will be something similar to RDF. As in Quad Model, edges have one type, but also they don't have an ID, so they can't have propreties or being part of another edge.
