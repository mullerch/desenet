#pragma once


#include "IsList"


/*
 * IsQueue is a Interrupt Save Queue. It can  be called insight an
 * ISR (Interrupt Service Routing).
 */
template<class T> class IsQueue : public IsList<T>
{
public:
	IsQueue()
    {}

    virtual ~IsQueue()
    {}

    T dequeue()
    {
    	return IsList<T>::pop();
    }

    void enqueue(const T & t)
    {
    	IsList<T>::addFirst(t);
    }
};
