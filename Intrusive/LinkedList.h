#pragma once
#include <stddef.h>
#include <assert.h>
#include <iterator>

namespace nst {

    class LinkedListNode
    {
    private:
        LinkedListNode* m_pNext;
        LinkedListNode* m_pPrev;

        template <class TOwner, LinkedListNode(TOwner::* TMemberPtr)>
        friend class LinkedListBI;

    private:
        // int overload initializes to self; used by LinkedList root node
        LinkedListNode(int)
            : m_pNext(this)
            , m_pPrev(this)
        {
        }
        void LinkNext(LinkedListNode* pNode)
        {
            assert(!pNode->m_pNext);
            assert(!pNode->m_pPrev);
            pNode->m_pNext = this->m_pNext;
            pNode->m_pPrev = this;
            this->m_pNext->m_pPrev = pNode;
            this->m_pNext = pNode;
        }
        void MakeNewRoot(LinkedListNode* pOldRoot)
        {
            pOldRoot->m_pPrev->m_pNext = this;
            pOldRoot->m_pNext->m_pPrev = this;
            this->m_pNext = pOldRoot->m_pNext;
            this->m_pPrev = pOldRoot->m_pPrev;
            pOldRoot->m_pNext = pOldRoot;
            pOldRoot->m_pPrev = pOldRoot;
        }

    public:
        LinkedListNode()
            : m_pNext{}
            , m_pPrev{}
        {
        }

        LinkedListNode* Next() const
        {
            return m_pNext;
        }
        LinkedListNode* Prev() const
        {
            return m_pPrev;
        }
        void Unlink()
        {
            this->m_pPrev->m_pNext = this->m_pNext;
            this->m_pNext->m_pPrev = this->m_pPrev;
#if _DEBUG
            this->m_pNext = nullptr;
            this->m_pPrev = nullptr;
#endif
        }
    };


    //  Linked list with bidirectional iterator support.
    //  *   Move constructor invalidates the 'end' iterator.
    template <class TOwner, LinkedListNode (TOwner::* TMemberPtr)>
    class LinkedListBI
    {
    public:
        typedef LinkedListBI<TOwner, TMemberPtr> This;
        typedef LinkedListNode Node;

    private:
        static size_t GetFieldOffset()
        {
            char* const pBase = (char*)(uintptr_t)0x100000; // any non-NULL value to induce pointer arithmetic
            TOwner* const pOwner = (TOwner*)pBase;
            char* const pField = (char*)(&(pOwner->*TMemberPtr));
            const size_t fieldOffset = pField - pBase;
            return fieldOffset;
        }

    public:
        class iterator
        {
        public:
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef TOwner value_type;
            typedef TOwner& reference;
            typedef TOwner* pointer;
            typedef ptrdiff_t difference_type;

        private:
            Node* m_pNode;
            friend class This;

        private:
            TOwner* ToOwnerPtr() const
            {
                const size_t fieldOffset = This::GetFieldOffset();
                TOwner* pOwner = (TOwner*)((char*)m_pNode - fieldOffset);
                return pOwner;
            }

        public:
            iterator()
                : m_pNode{}
            {
            }
            iterator(const iterator& rhs)
                : m_pNode(rhs.m_pNode)
            {
            }
            iterator(Node* pNode)
                : m_pNode(pNode)
            {
            }
            iterator(TOwner* pOwner)
                : m_pNode(&(pOwner->*TMemberPtr))
            {
            }
            iterator& operator=(const iterator& rhs)
            {
                m_pNode = rhs.m_pNode;
                return *this;
            }
            iterator& operator=(Node* pNode)
            {
                m_pNode = pNode;
                return *this;
            }
            iterator& operator=(TOwner* pOwner)
            {
                m_pNode = &(pOwner->*TMemberPtr);
                return *this;
            }

            Node* Node() const
            {
                return m_pNode;
            }
            TOwner& operator*() const
            {
                TOwner* pOwner = ToOwnerPtr();
                return *pOwner;
            }
            TOwner* operator->() const
            {
                TOwner* pOwner = ToOwnerPtr();
                return pOwner;
            }

            iterator& operator++()
            {
                m_pNode = m_pNode->Next();
                return *this;
            }
            iterator operator++(int)
            {
                return iterator{ m_pNode->Next() };
            }
            iterator& operator--()
            {
                m_pNode = m_pNode->Prev();
                return *this;
            }
            iterator operator--(int)
            {
                return iterator{ m_pNode->Prev() };
            }

            bool operator==(const iterator& rhs) const
            {
                return m_pNode == rhs.m_pNode;
            }
            bool operator!=(const iterator& rhs) const
            {
                return m_pNode != rhs.m_pNode;
            }
        };

        class const_iterator
        {
        public:
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef const TOwner value_type;
            typedef const TOwner& reference;
            typedef const TOwner* pointer;
            typedef ptrdiff_t difference_type;

        private:
            const Node* m_pNode;

        private:
            const TOwner* ToOwnerPtr() const
            {
                const size_t fieldOffset = This::GetFieldOffset();
                const TOwner* pOwner = (const TOwner*)((const char*)m_pNode - fieldOffset);
                return pOwner;
            }

        public:
            const_iterator()
                : m_pNode{}
            {
            }
            const_iterator(const iterator& rhs)
                : m_pNode(rhs.m_pNode)
            {
            }
            const_iterator(const Node* pNode)
                : m_pNode(pNode)
            {
            }
            const_iterator(const TOwner* pOwner)
                : m_pNode(&(pOwner->*TMemberPtr))
            {
            }
            const_iterator& operator=(const const_iterator& rhs)
            {
                m_pNode = rhs.m_pNode;
                return *this;
            }
            const_iterator& operator=(const Node* pNode)
            {
                m_pNode = pNode;
                return *this;
            }
            const_iterator& operator=(const TOwner* pOwner)
            {
                m_pNode = &(pOwner->*TMemberPtr);
                return *this;
            }
            const_iterator& operator=(const iterator& rhs)
            {
                m_pNode = rhs.m_pNode;
                return *this;
            }

            const TOwner& operator*() const
            {
                const TOwner* pOwner = ToOwnerPtr();
                return *pOwner;
            }
            const TOwner* operator->() const
            {
                const TOwner* pOwner = ToOwnerPtr();
                return pOwner;
            }

            const_iterator& operator++()
            {
                m_pNode = m_pNode->Next();
                return *this;
            }
            const_iterator operator++(int)
            {
                return const_iterator{ m_pNode->Next() };
            }
            const_iterator& operator--()
            {
                m_pNode = m_pNode->Prev();
                return *this;
            }
            const_iterator operator--(int)
            {
                return const_iterator{ m_pNode->Prev() };
            }

            bool operator==(const const_iterator& rhs) const
            {
                return m_pNode == rhs.m_pNode;
            }
            bool operator!=(const const_iterator& rhs) const
            {
                return m_pNode != rhs.m_pNode;
            }
        };

    private:
        Node m_root;

    public:
        ~LinkedListBI()
        {
#if _DEBUG
            for (iterator it = begin(), en = end(); it != en;)
            {
                erase(it++);
            }
#endif
        }
        LinkedListBI()
            : m_root(0)
        {
        }
        LinkedListBI(const LinkedListBI& rhs) = delete;
        LinkedListBI(LinkedListBI&& rhs)
        {
            m_root.MakeNewRoot(&rhs.m_root);
        }
        This& operator=(LinkedListBI&& rhs)
        {
            m_root.MakeNewRoot(&rhs.m_root);
        }

        iterator begin()
        {
            return iterator{ m_root.Next() };
        }
        iterator end()
        {
            return iterator{ &m_root };
        }
        const_iterator begin() const
        {
            return const_iterator{ m_root.Next() };
        }
        const_iterator end() const
        {
            return const_iterator{ &m_root };
        }

        This& push_front(TOwner* pOwner)
        {
            Node* pNode = &(pOwner->*TMemberPtr);
            m_root.LinkNext(pNode);
            return *this;
        }
        This& push_back(TOwner* pOwner)
        {
            Node* pNode = &(pOwner->*TMemberPtr);
            m_root.Prev()->LinkNext(pNode);
            return *this;
        }
        This& insert(iterator itr, TOwner* pOwner)
        {
            itr.m_pNode->LinkNext(pOwner);
            return *this;
        }
        This& erase(iterator itr)
        {
            assert(itr.m_pNode != &m_root);
            itr.m_pNode->Unlink();
            return *this;
        }
    };

}
