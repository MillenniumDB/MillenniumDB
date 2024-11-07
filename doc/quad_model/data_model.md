# MillenniumDB Property Graph Model

Everything in our graph model is an **Object**, and there are different 3 types of objects:

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

    2.2. **Anonymous Nodes**: they don't have a string as identifier when you add them into the database.


3. **Edges**: an edge is an object that relates other objects, having the following attributes:
    - An auto-generated identifier.
    - `from`: any **Object** (including other edge object but not itself).
    - `to`: any **Object** (including other edge object but not itself).
    - `type`: a **Named Node**.
    - `properties`: a (possibly empty) set of pairs <key, value>. Where the key is a string and the value is a literal. A key cannot appear twice in the set.


# Domain restrictions
- **Integer**: for now, we only support integers from -(2<sup>56</sup>-1) to 2<sup>56</sup>-1.

- **String**: sequence of characters. They often are delimited by double quotes (`""`). The only exception is when you are working with keys. For that reason, property keys cannot be any string, they must match the regular expression `[A-Za-z][A-Za-z0-9_]*`. They must have a size less than 1024\*1024\*64;

- **Float**: same as in C language. Use with caution as they may lose precision.

- **Boolean** `true` or `false`.

- **Named Nodes**: they match the regular expression `[A-Za-z][A-Za-z0-9_]*`. Identifiers are case sensitive.

- **Anonymous Nodes** The identifier will match the regular expression `_a[1-9][0-9]*`.

- **Edges** The auto-generated identifier will match the regular expression `_e[1-9][0-9]*`.

# Import Format

Our import expects a plain text file, with extension `.qm`, where each line will be a definition of a node or and edge. A node doesn't need to be defined prior to be used in an edge.

## Node definition
1. First we have a **Node ID**, which may be **Node Name** or a **Anonymous identifier**.
2. Then we have a (possibly empty) list of **Labels**.
3. Finally we have a (possibly empty) list of **Properties** (key/value pairs).

Example:
```
Joe :Person :Student age:20 gender:"Male" height:1.81 key_1:true
```
- `Joe` is the node identifier (node name).
- `Person` and `Student` are the Labels of the node.
- `age:20`, `gender:"Male"`, `height:1.81`, `key_1:true` are the Properties of the node.

## Edge definition
1. Node Identifier or Literal
2. Edge direction (`->` or `<-`)
3. Node Identifier or Literal
4. Edge Type (must be a Node Name)
5. A (possibly empty) list of **Properties** (key/value pairs).

Some examples of valid edge definitions:
```
Joe->John :Follows
Joe->20 :Age
Joe->"Hello World" :Posted date:"06-10-2010"
```
