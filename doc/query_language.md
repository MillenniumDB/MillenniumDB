# Query language

A query would look like:
```
// This query is asking for the age and name of people
// that knows John having between 60 and 70 years old,
// ordered by their age (ascending) and name (descending).
MATCH (?x :Person)-[:Knows]->(John)
WHERE ?x.age >= 60 AND ?x.age <= 70
ORDER BY ?x.name DESC, ?x.age ASC
RETURN ?x.age, ?x.name
LIMIT 1000
```
The first 3 lines are comments. A comment starts with a `//` and finish at the end of the line. Each following line is a Statement.

# MATCH Statement
Every query must have a MATCH statement at the beginning. The keyword MATCH is followed by a **graph pattern**. To define a graph pattern we have to define other smaller components that compose it.

## Node pattern
 First we define the **node pattern**. The most basic **node pattern** looks like this:
- `()`

That **node pattern** refers to any Object in the database, and there are some things you can add to a **node pattern** to make it more useful:

- An **Object Identifier** to refer to a specific object.
    | **Object type** | example |
    | :--- | ---: |
    |NamedNode|`(John_Doe)`|
    |AnonymousNode|`(_a123)`|
    |Edge|`(_e123)`|
    |String Literal|`"some string"`|
    |Boolean Literal|`true`|
    |Integer Literal|`123`|
    |Float Literal|`3.14`|

- If you are not using an **Object Identifier**, you can add a **variable** to bind the node
    - `(?x)`

- If you are not using an **Object Identifier**, you can add a list of labels (after the variable if it is present)
    - `(:Label)`
    - `(:Label1 :Label2)`
    - `(?x :Label)`
    - `(?x :Label1 :Label2 :Label3)`
- If you are not using an **Object Identifier**, you can add a set of properties (a the end)
    - `({key:"value"})`
    - `(?x {key1:"value1", key2:"value2"})`
    - `(?x :Label {key:"value"})`

## Edge pattern
Two **node patterns** can be connected to each other via **edges**.

And similar as in **node patterns**, **edges** may contain other some things:
- An **edge variable** to bind the edge object:
    - `(?x)-[?e]->(?y)`
- Or instead of a **edge variable**, a **fixed edge**:
    - `(?x)-[_e123]->(?y)`
- A **type variable** (after the edge variable or fixed edge if they are present):
    - `(?x)-[:?t]->(?y)`
    - `(?x)-[?e :?t]->(?y)`
- Or instad of a **type variable**, a **fixed type** (after the edge variable or fixed edge if they are present):
    - `(?x)-[:Type1]->(?y)`
    - `(?x)-[?e :Type2]->(?y)`
- A set of properties (at the end)
    - `(?x)-[{key:"value"}]->(?y)`
    - `(?x)-[?e :Type {key:"value"}]->(?y)`

**Edges** must have a **direction**:
- `(?x)->(?y)`
- `(?y)<-(?z)`

## Path pattern
Two **node patterns** can be connected to each other via **paths**. **Paths** have 4 components:
1. Regular Path Expression. This is similar to a regular expression, but instead of letters we have **Types** as the atom. The formal definition is:
    - If `t` is a valid **Type**, `t` is a Regular Path Expression.
    - If `t` is a Regular Path Expression:
        - `^t` is a Regular Path Expression (`t` occurs with an inverse direction).
        - `t*` is a Regular Path Expression (`t` occurs 0 or more times).
        - `t+` is a Regular Path Expression (`t` occurs 1 or more times).
        - `t?` is a Regular Path Expression (`t` occurs 0 or 1 times).
    - If `t1` and `t2` are Regular Path Expressions:
        - `t1 / t2` is a Regular Path Expression (`t1` is followed by `t2`).
        - `t1 | t2` is a Regular Path Expression (`t1` or `t2` occurs).
2. A path variable (optional)

3. The semantic (`ALL` or `ANY`), to specify if the results will contain any shortest path or all shortest path between two nodes. If none is specified, `ANY` is assumed.

4. A direction

Example of paths:
- `(?x)=[:Type+]=>(?y)`
- `(Foo)<=[ALL ?p :Type+]=(?z)`
- `(Foo)=[ANY ?p :Type1+/:Type2?]=>(Bar)`

## Linear pattern
Then a **linear pattern** is a set of one or more **node patterns** linked by edges and/or paths:
- `(John)=[:Knows+]=>(?x)-[:WorksIn]->(?y)`

## Basic graph pattern
A set of one or more **linear patterns** (separeted by comma) forms a **basic graph pattern**
- `(?x :Person)-[:Knows]->(?y)<-[:Knows]-(John), (?y)-[:LivesIn]->(Chile)`

## Graph pattern
A **graph pattern** is defined as follows:
- A **basic graph pattern** is a **graph pattern**.
- If `GP1` and `GP2` are **graph patterns**, `GP1 OPTIONAL { GP2 }` is a **graph pattern**

# WHERE Statement
A query may not have a WHERE statement. The WHERE statement filters the results obtained by the MATCH statement according to certain **Expression**. All results that not evaluate the given expression into `true` are filtered out.

# ORDER BY Statement
A query may not have a ORDER BY statement. You can specify how the order works with the keywords `ASC` and `DESC` after each element. If the order is not specified the default is `ASC`.

# RETURN Statement
Every query must have a RETURN statement. This statement specify which objects or object properties will be returned. Return statements look like this:
- `RETURN *`
- `RETURN ?x`
- `RETURN ?x.key`
- `RETURN ?x, ?y.key, ?z`

# LIMIT Statement
A query may not have a LIMIT statement. A LIMIT statement gives an upper bound on the number of results returned.
