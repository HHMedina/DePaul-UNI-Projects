  
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include "jmisc.h"

template <class item> class ListIter;

template <class item>
class LinkedList
{
public:

	template <class item>
	struct Node
	{
		item contents;
		Node<item> *next;
		Node<item> *prev;
	};

	LinkedList();
	LinkedList(const LinkedList& l);
	virtual ~LinkedList();

	void ClearList(void);
	void Clear(void) { ClearList(); }

	LinkedList& operator=(const LinkedList& l);

	void MoveToHead(void) const;
	void MoveFirst(void) const;
	void MoveNext(void) const;
    void MovePrev(void) const;
	void MoveToTail(void) const;
	void MoveLast(void) const;

	void InsertHead(const item &source);
	void Prepend(const item& source) { InsertHead(source); }
	void InsertTail(const item &source);
	void Append(const item &source) { InsertTail(source); }
	void InsertBefore(const item &source);
	void InsertAfter(const item& source);
	void Insert(const item& source) { InsertAfter(source); }
	void InsertSorted(const item& source);
	void InsertSortedPointer(const item& source);
	void Sort(void);
	void SortPointer(void);
	
	// Methods for "random" access
	bool MoveTo(int index);
	int CurrentIndex() { return curIndex; }

	bool Find(const item& source) const;

	void RemoveCurrent(void);
	void RemoveFirst(void);
	void RemoveLast(void);
	void Remove(const item &source);

	bool AtEnd(void) const { return (current == tail); }
	bool AtHead(void) const { return (current == head); }
	bool AtTail(void) const { return (current == tail); }
	bool IsEmpty(void) const { return (head->next == tail); }

	// really need a generic iterator to replace this!
	void SetBookmark() const { bookmark = current; }
	void RecallBookmark() const { if (bookmark != NULL) current = bookmark; }
	// This needs to be upgraded to protect against null pointer accesses
	// Also we must remember that this gives us write access to the contents
	// So we must manually sort if needed.
	item& BookmarkedItem() { return bookmark->contents; }
	bool BookmarkSet() { return (bookmark != NULL); }
	void ResetBookmark() { bookmark = NULL; }

	item& operator()(void) { if (current != head && current != tail) return current->contents; else complain("No current item in iterator ... Off end of list"); exit(1); }
	item& GetCurrent(void) const; 
	item& GetFirst() const;
	item& GetLast() const;

	size_t Length(void) const;

	ListIter<item> Begin() { return ListIter<item>(*this); }

	void MoveTo(const ListIter<item>& iter) { if (this == iter.list) current = iter.current; }

	friend class ListIter<item>;

protected:

	mutable Node<item> *current; // This can change in const methods
	Node<item> *head;
	Node<item> *tail;
	mutable Node<item> *bookmark;
	mutable int curIndex;
	int length;
};

template <class item>
class ListIter
{
public:

	ListIter() { current = NULL; list = NULL; }
	ListIter(LinkedList<item> &l)
	{ current = l.head->next; list = &l; }

	void Begin() { current = list->head->next; }
	void Begin(const LinkedList<item>& l) { current = l.head->next; list = &l; }
	void operator++(int k) { MoveNext(); }
	void operator--(void) { MovePrev(); }
	item& operator()(void) { if (current != list->head && current != list->tail) return current->contents; else complain("No current item in iterator ... Off end of list"); exit(1); }
	item& operator*(void) { if (current != list->head && current != list->tail) return current->contents; else complain("No current item in iterator ... Off end of list"); exit(1); }
	item& GetCurrent(void) { if (current != list->head && current != list->tail) return current->contents; else complain("No current item in iterator ... Off end of list"); exit(1); }
	item& GetHead(void) { return list->GetHead(); }
	item& GetTail(void) { return list->GetTail(); }
	bool Done() { return (current == list->tail); }
	bool AtEnd() { return (current == list->tail); } 
	bool AtTail() { return (current == list->tail); }
	bool AtHead() { return (current == list->head); }
	void MoveHead() { current = list->head; }
	void MoveFirst() { current = list->head->next; }
	void MoveTail() { current = list->tail; }
	void MoveLast() { current = list->tail->prev; } 
	void MoveNext() { if (current != list->tail) current = current->next; }
	void MovePrev() { if (current != list->head) current = current->prev; }

	friend class LinkedList<item>;

protected:

	typename LinkedList<item>::Node<item> *current;
	LinkedList<item> *list;
};

template <class item>
class Stack : protected LinkedList<item>
{
public:
	
	Stack();
	
	void Push(const item &newItem);
	item Pop(void);
	item &Peek(void); // Use only for temporary access!
	bool IsEmpty(void);
	void Clear(void);
	int Depth(void) { return LinkedList<item>::Length(); }
};

#include "LinkedList.tli"

#endif