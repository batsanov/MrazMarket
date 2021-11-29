#pragma once
#include <iostream>

template<class T>
class PQueue {
private:
	struct Node {
		T data;
		Node* next;
		int priority;
		Node(T data, int priority) : data(data), next(nullptr), priority(priority) {};
	};
	Node* begin;
	
	void copy(Node* start);
	void clear();

public:
	PQueue() : begin(nullptr) {};
	PQueue(const PQueue& other);
	PQueue& operator=(const PQueue& other);
	~PQueue() { clear(); };



	void dequeue()
	{
		
		if (begin == nullptr)
			throw std::underflow_error("Empty queue!");
		else
		{
			Node* temp;
			temp = begin;
			begin = begin->next;
			delete temp;
		}
	}

	void insert(T data, int priority)
	{
		Node* it;
		Node* temp = new Node(data,priority);
		
		if (begin == nullptr || priority < begin->priority)
		{
			temp->next = begin;
			begin = temp;
		}
		else
		{
			it = begin;
			while (it->next != nullptr && it->next->priority <= priority)
				it = it->next;
			temp->next = it->next;
			it->next = temp;
		}
	}


	const T& first();

	bool isEmpty()const { return begin == nullptr; };

	void testPrint();
	
	int size();
};

template<class T>
inline void PQueue<T>::copy(Node* start)
{
	begin = new Node(start->data, start->priority);

	Node* it = start->next;

	while (it)
	{
		enqueue(it->data);
		it = it->next;
	}
}

template<class T>
inline void PQueue<T>::clear()
{
	while (!isEmpty())
	{
		Node* helper = begin;
		begin = begin->next;
		delete helper;
	}

}

template<class T>
inline PQueue<T>::PQueue(const PQueue& other)
{
	copy(other.begin);
}

template<class T>
inline PQueue<T>& PQueue<T>::operator=(const PQueue& other)
{

	if (this != &other)
	{
		clear();
		copy(other.begin);
	}
	return *this;
}




template<class T>
inline const T& PQueue<T>::first()
{
	return begin->data;
}

template<class T>
inline void PQueue<T>::testPrint()
{
	Node* it = begin;
	while (it)
	{
		std::cout << it->data;
		it = it->next;
	}
}

template<class T>
inline int PQueue<T>::size()
{
	int count = 0;
	Node* it = begin;
	while (it)
	{
		count++;
		it = it->next;
	}
	return count;
}
