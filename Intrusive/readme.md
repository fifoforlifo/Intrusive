# C++ Intrusive Data Types

There's plenty of intrusive data structure implementations out there, including the
excellent https://www.boost.org/doc/libs/1_64_0/doc/html/intrusive.html[Boost.Intrusive].

However, IMO these all have over-wrought or over-complicated interfaces.  This library presents
both type-safe and simple interfaces, while still ensuring compile-time optimization of all data conversions.

## LinkedListBI

*   Linked-list supporting bidirectional iterators.
*   The sacrifice that comes with Bidit is that the move-constructor invalidates the end iterator.
    *   To decrement the end iterator, the end needs to be a Node; to avoid heap allocation, the root Node
        is located in the LinkedList struct (by-value).  A move of a LinkedList instance implies a new
        root Node at the destination LinkedList address, which also requires other Nodes in the list
        to be rewired to the new root.
*   Easy inter-convertibility between {iterator, LinkedListNode, Owner} types.
*   The LinkedListNode can be directly unlinked from its container.
*   The underlying algorithm is a doubly-linked circular list.  The circular aspect allows decrementing
    the end iterator, and allows the root node to act as the end node.




