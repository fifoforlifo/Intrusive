#include <stdio.h>
#include <algorithm>
#include "../Intrusive/LinkedList.h"

struct IntNode
{
    int value;
    nst::LinkedListNode nodeA;
    nst::LinkedListNode nodeB;
    nst::LinkedListNode nodeC;
};

template <class List>
void PrintIntList(const List& list, const char* pName)
{
    printf("%s: ", pName);
    for (const IntNode& intNode : list)
    {
        printf("%d,", intNode.value);
    }
    printf("\n");
}

void TestLinkedList()
{
    nst::LinkedListBI<IntNode, &IntNode::nodeA> aList;
    nst::LinkedListBI<IntNode, &IntNode::nodeB> bList;
    nst::LinkedListBI<IntNode, &IntNode::nodeC> cList;

    IntNode n1 = { 1 };
    IntNode n2 = { 2 };
    IntNode n3 = { 3 };

    aList.push_back(&n1);
    aList.push_back(&n2);
    bList.push_back(&n2);
    bList.push_back(&n3);

    cList.push_back(&n1);
    cList.push_back(&n2);
    cList.push_back(&n3);

    PrintIntList(aList, "aList");
    PrintIntList(bList, "bList");
    PrintIntList(cList, "cList");
    std::for_each(cList.begin(), cList.end(), [](IntNode& intNode) { intNode.value += 10; });
    PrintIntList(aList, "aList");
    PrintIntList(bList, "bList");
    PrintIntList(cList, "cList");
}

int main()
{
    TestLinkedList();
    return 0;
}

