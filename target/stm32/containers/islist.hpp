#pragma once


#include <critical/critical.h>


/*
 * IsList is a Interrupt Save List. It can  be called insight an
 * ISR (Interrupt Service Routing).
 */
template<class T> class IsList
{
public:
	IsList() : count_(0) ,first(NULL) ,last(NULL)
	{}

	virtual ~IsList()
	{
	    removeAll();
	}

	class Item
	{
	public:
		friend class IsList<T>;

		Item(const T & theConcept) : concept(theConcept), next(NULL)
		{}

		Item(const Item& other) : concept(other.concept), next(other.next)
		{}

		Item()
		{}

		~Item()
		{
		    next = NULL;
		}

	public:

		// Connect to the specified item
		// Argument Item* item :
		// The item to connect to
		inline void connectTo(Item* item)
		{
		    next = item;
		}

		// Get the next item in the linked list
		inline Item * getNext() const
		{
			return next;
		}

		// Assignment operator
		// Argument const Item& other :
		// The item to copy
		inline Item &  operator=(const Item& other)
		{

			next = other.next;

			// Todo: Because concept is a pointer need to duplicate data here
			concept = other.concept;	// assuming operator = for Concept

			return *this;
		}

	public:
		// the data of the node
		T concept;
		// the next item in the list
		Item * next;
	};

public:
	// Add an object to the list (at its end)
	// Argument const T & c :
	// The element to add
	void add(const T & c);

	// if getCount>=i - Add c after i'th element else Add c at end
	// Argument int i :
	// The index
	// Argument const T & c :
	// The element to add
	void addAt(int i, const T & c);

	// Add an object to the list (at its beginning)
	// Argument const T & c :
	// The element to add
	bool addFirst(const T & c);

	// Find an object in the list
	// Argument const T & c :
	// The element to find
	bool find(const T & c) const;

	// Get the element in the given index
	// Argument int i :
	// The index
	T & getAt(int i) const;

	// Get the element in the head of the list
	T getFirstConcept() const;

	// Get the element in the tail of the list
	T getLastConcept() const;

	virtual T getNULLContent() const;

	// Check if the list is empty
	bool isEmpty() const;

	// Get the element at the provided index
	// Argument int i :
	// The index
	T & operator[](int i) const;

	T pop();

	// Remove the first occurrence of a specific Object (Concept)
	// from list
	// Argument T c :
	// The element to remove
	void remove(T c);

	// Remove all items from list
	//## operation removeAll()
	void removeAll();

	// Remove first item from list
	void removeFirst();

	// Remove a specific Item from list
	// Argument IsList<T>::Item * item :
	// The list node to remove
	void removeItem(IsList<T>::Item * item);

	// Remove last item from list - inefficient as we keep no
	// backward pointers
	void removeLast();

protected :

	// unsafe Remove first item from list
	void _removeFirst();

	// Get the element at the given position (called by the iterator)
	// Argument void * pos :
	// The iterator position
	virtual T getCurrent(void * pos) const;

	// Set the initial position for the iterator
	// Argument void*& pos :
	// The list head (out)
	virtual void getFirst(void*& pos) const;

	// Bet the position of the list tail
	// Argument void*& pos :
	// The list tail (out)
	virtual void getLast(void*& pos) const;

	// Update the provided position to the next position in the container
	// Argument void*& pos :
	// The iterator position to advance
	virtual void getNext(void*& pos) const;


protected:
	// the number of elements in the list
	unsigned long count_;

	// the list head
	Item * first;

	// the tail head
	Item * last;
};

template<class T> void IsList<T>::_removeFirst()
{
    Item * tmp = first;

    // * Protection should be made outside,
    // * when calling this function
    //
    // enterCritical();

    first = first->next;
    if (first == NULL)
    {
    	last = NULL;
    }
    if (tmp != NULL)
    {
    	delete tmp;
    	--count_;
    }

    // exitCritical();
}

template<class T> void IsList<T>::add(const T & c)
{
    // Create a new list item
    Item * item = new Item(c);

    enterCritical();

    // Insert it into the list at the end
    if (first == NULL)
    {
    	first = item;
    }
    else
    {
    	last->connectTo(item);
    }
    last = item;
    ++count_;

    exitCritical();
}

template<class T> void IsList<T>::addAt(int i, const T & c)
{
    enterCritical();

    if (i == 0 || first == NULL)
    {
    	// Insert c in first position
    	addFirst(c);

    	exitCritical();
    	return;
    }

    // Find the position of insertion
    Item * item = first;

    for(int j = 1; (j < i) && (item != last); j++)
    {
    	item = item->next;
    }

    if (item == last)
    {
    	// Insert c in last position
    	add(c);
    }
    else
    {
    	Item* newGuy = new Item(c);
    	Item* nextGuy = item->next;
    	item->connectTo(newGuy);
    	newGuy->connectTo(nextGuy);
    	++count_;
    }

    exitCritical();
}

template <class T> bool IsList<T> ::addFirst(const T & c)
{
    // Create a new list item
    Item * item = new Item(c);

    if (item)
    {
    	enterCritical();

    	// Insert it into the list at the beginning
    	item->connectTo(first);
    	first = item;
    	if (last == NULL)
    	{
    		last = first;
    	}
    	++count_;

    	exitCritical();

    	return true;
    }
    return false;
}

template <class T> bool IsList<T> ::find(const T & c) const
{
    for (Item* i = first; i != NULL; i = i->next)
    {
    	if (i->concept == c)
    	{
    		return true;
    	}
    }
    return false;
}

template <class T> T & IsList<T> ::getAt(int i) const
{
    void* item;

    getFirst(item);

    for(int j = 0; j < i; j++)
    {
    	if (item)
    	{
    		getNext(item);
    	}
    	else
    	{
    		break;
    	}
    }
    return getCurrent(item);
}

template <class T> T IsList<T> ::getCurrent(void * pos) const
{
    if (pos != NULL)
    {
    //	return reinterpret_cast<Item*>(pos)->concept;
    	return ((Item*)(pos))->concept;
    }
    else
    {
    	return getNULLContent();
    }
}

template<class T> void IsList<T>::getFirst(void*& pos) const
{
    pos = first;
}

template <class T> T IsList<T> ::getFirstConcept() const
{
    return getCurrent(first);
}

template<class T> void IsList<T>::getLast(void*& pos) const
{
    pos = last;
}

template <class T> T IsList<T> ::getLastConcept() const
{
    return getCurrent(last);
}

template <class T> T IsList<T> ::getNULLContent() const
{
    static bool bInitialized = false;
    static T sContent = T();

    if (bInitialized)
    {
    	bInitialized = true;

    	memset((void*)&sContent, 0, sizeof(T));
    }
    return sContent;
}

template<class T> void IsList<T>::getNext(void*& pos) const
{
    // pos = reinterpret_cast<Item*>(pos)->next;
    pos = ((Item*)(pos))->next;
}

template <class T> bool IsList<T> ::isEmpty() const
{
    return (first == NULL);
}

template <class T> T & IsList<T> ::operator[](int i) const
{
    return getAt(i);
}

template <class T> T IsList<T> ::pop()
{
    enterCritical();

    T c = getLastConcept();
    removeLast();

    exitCritical();

    return c;
}

template<class T> void IsList<T>::remove(T c)
{

    if (first == NULL)
    {
    	// Nothing to remove
    	return;
    }

    enterCritical();

    if (first->concept == c)
    {
    	// Special case the object is first
    	_removeFirst();

    	exitCritical();
    	return;
    }

    // Search for the object
    Item * prev = first;
    Item * cur = first->next;

    while (cur!=NULL)
    {
    	if (cur->concept == c)
    	{
    		prev->connectTo(cur->next);
    		if (cur->next == NULL)
    		{
    			last = prev;
    		}
    		delete cur;
    		--count_;

    		exitCritical();
    		return;
    	}

    	prev = cur;
    	cur = cur->next;
    }
    exitCritical();
}

template<class T> void IsList<T>::removeAll()
{
    enterCritical();

    while(first != NULL)
    {
    	_removeFirst();
    }

    exitCritical();
}

template<class T> void IsList<T>::removeFirst()
{

    enterCritical();

    if (first != NULL)
    {
    	_removeFirst();
    }

    exitCritical();
}

template<class T> void IsList<T>::removeItem(IsList<T>::Item * item)
{
    if (first == NULL)
    {
    	// Nothing to remove
    	return;
    }

    enterCritical();

    if (first == item)
    {
    	// Special case the required item is first
    	_removeFirst();

    	exitCritical();
    	return;
    }

    // Search for the item
    Item * prev = first;
    Item * cur = first->next;

    while (cur != NULL)
    {
    	if (cur == item)
    	{ // item found
    		prev->connectTo(cur->next);
    		if (cur->next == NULL)
    		{
    			last = prev;
    		}
    		delete cur;
    		--count_;

    		exitCritical();
    		return;
    	}
    	// Advance to next item
    	prev = cur;
    	cur = cur->next;
    }

    exitCritical();
}

template<class T> void IsList<T>::removeLast()
{
    enterCritical();

    removeItem(last);

    exitCritical();
}

