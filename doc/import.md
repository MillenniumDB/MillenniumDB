# Import Format

We use a plain text file, where each line will be a definition of a node or and edge. A node doesn't need to be defined in order to use it in an edge.

## Node definition
1. First we have a **Node ID**, which may be **Node Name** or a **Anonymous identifier**.
2. Then we have a (possibly empty) list of **Labels**.
3. Finally we have a (possibly empty) list of **Properties** (key/value pairs).

Valid node names, labels and property keys must match the regular expression `[A-Za-z][A-Za-z0-9_]*` (with the exception of `true` and `false` which are reserved words). An anonymous identifier match the regular expression `_a[1-9][0-9]*`.


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
Note the distinction betweem **Node Name** and **string literal**, `"Joe"` is a string literal, and `Joe` is a Node Name.
