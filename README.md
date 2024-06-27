# SymbolTable
Symbol Table implemented in various Data Structures, with C++/Java comparison.
Here, a symbol table is a collection of pairs of the form (key,value), where key is a String and value an int.

## Operations on SymbolTable

```java
void insert( String key )
int retrieve( String key )
void remove( String key )
int rank( String key )
String select( int )
```

The C++ version of insert() actually has as a prototype similar to `void insert( String key , int value )`, so you could insert any value to a given key. This behavior was changed when creating the java version, since the use of insert() method was always to increment the value. When inserting a key that is not already in the symbol table, it's value is initialized as 1. Then, it is incremented by 1 at every new insert on the same key.

## Data Structures

- Unordered Vector
- Ordered Vector
- Unordered Linked List
- Ordered Linked List
- Binary Search Tree
- Treap
- 2-3 Search Tree
- Red-black Search Tree
- Hash Table
