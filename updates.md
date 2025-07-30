# Updates

## INSERT statement

### Creating a node

To create a node named `N1`:

```mql
INSERT (N1)
```

You can also insert the node specifying labels and properties using the syntax of graph patterns:

```mql
INSERT (N1:Label_X {key1:"value", key2:false})
```

If the node `N1` existed before, the previous labels still remain, the previous properties also remain unless they are redefined with a different value, in that case the value changes.

For example executing these two queries sequentially:

```mql
INSERT (N1:L1:L2 {key1:"value", key2:false})
```

```mql
INSERT (N1:L1:L3 {key1:1, key3:3})
```

Will result in the equivalent insert:

```mql
INSERT (N1:L1:L2:L3 {key1:1, key2:false, key3:3})
```

### Creating an edge

To create an edge between `N1` and `N2`.

```mql
INSERT (N1)-[:WorksAt]->(N2)
```

The edge type **must** be specified, optionally you can add properties. If the nodes used don't exist, they will be created.

### Creating a graph pattern

You can insert multiple nodes and edges in one query:

```mql
INSERT (Jane)-[:Knows]->(Joe :Person {name:"Joe Doe"})-[:WorksAt]->(WorkPlace1),
       (Jane)-[:WorksAt {since:date("2020-01-31")}]->(WorkPlace2 {name:"Foo"}),
       (WorkPlace1)-[:LocatedAt]->(MyCity)<-[:LocatedAt]-(Workplace2)
```

## DELETE statement

### Deleting a node

To delete a node named `N1` and all of its labels and properties:

```mql
DELETE N1
```

However, if the node `N1` is present in an edge the query will fail. To delete the node and all of the edges where it appears use the `DETACH` keyword.

```mql
DETACH DELETE N1
```

### Deleting an edge

To delete an edge identified by `_e1` and all of its properties:

```mql
DELETE _e1
```

When deleting an edge, using the `DETACH` keyword has no effect.

## REMOVE statement

### Removing labels or properties

To remove the label `Person` from a node named `N1`:

```mql
REMOVE N1:Person
```

To remove the property `k` from a node named `N1`:

```mql
REMOVE N1.k
```

To remove the property `k` from an edge identified by `_e1`:

```mql
REMOVE _e1.k
```

You cannot remove the edge type, because in the quad model every edge must have one type. But you can edit it.

## SET statement

### Editing labels or properties

Edit node labels:

```mql
SET N1 :Person :Worker
```

this deletes previous labels of `N1`, and create the labels `Person` and `Worker` to `N1`.

Edit edge type:

```mql
SET _e1:NewType
```

Set a single property:

```mql
SET N1.k = "foo"
```

```mql
SET _e1.k = "foo"
```

Set all properties (deleting the previous properties):

```mql
SET N1 {k1: "foo", k2: "foo"}
```

```mql
SET _e1 {k1: "foo", k2: "foo"}
```

## Updates with variables

All the previous examples had a fixed node name or edge identifier, but you can use variables and expressions instead. To do that you can use the same query language (MQL) defined in (TODO: link to definition), replacing the return statement with the desired update statement.

Keep in mind an update can fail when:

1. It tries to assign a null property
2. It tries to create an edge with a null node
3. It tries to delete a connected node without using `DETACH`

When a query fails, the database remains in the same initial state.

Examples:

```mql
MATCH (?x :Account), (?y :Account)
WHERE ?x.account_id == 12345 && ?x.account_id == 54321
SET ?x.amount = ?x.amount + 10000, ?y.amount = ?y.amount - 10000
// this query fails if ?x.amount or ?y.amount are null
```

```mql
MATCH (?x)-[:T1]->(?y)
OPTIONAL { (?y)-[:T2]->(?z) }
INSERT (?x)-[:T3]->(?z)
// this query fails if at any binding ?z is null
```

```mql
MATCH (?x :Person)
WHERE ?x.name == "Joe"
DELETE ?x
// this query fails if at any binding ?x is part of some edge
```
