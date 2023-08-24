#pragma once

#include "iterator.h"

#include<iostream>
#include <ObjectPool.h>

using namespace std;


template <class T, unsigned int N>
class LinkedList {
private:
	Node<T>* first;
	Node<T>* last;
	int size;
	ObjectPool<Node<T>, N> nodePool;
	
public:
	LinkedList();
	~LinkedList();  // Destructor to release memory
	void Clear();
	bool PushBack(const T& element);
	bool PopBack();
	T Front();
	T Back();
	void PrintValues();
	int GetSize();

	class LinkedListIterator : public Iterator<T> {
	private:
		Node<T>* first;
		Node<T>* currentNode;
		template<typename U, unsigned int N>
		friend class LinkedList;
	public:
		// For general iterator
		LinkedListIterator(Node<T>* head) :  first(head) {
			currentNode = head;
		}

		// Checks if iterator has a next element or not
		bool HasNext() {
			return currentNode != NULL;
		}

		//Iterates through the next element and returns it
		T Next() {
			if (!HasNext())
				return {};
			
			T returnData = currentNode->data;
			currentNode = currentNode->next;

			return returnData;
		}

		// Checks if iterator has a previous element or not
		bool HasPrev() {
			if (currentNode == first)
				return false;
			/*
			Node<T>* temp = first;

			while (temp->next != currentNode) {
				temp = temp->next;
			}
			*/

			return true;
		}

		//Iterates through the previous element and returns it
		T Prev() {
			if (!HasPrev())
				return {};

			Node<T>* temp = first;

			while (temp->next != currentNode) {
				temp = temp->next;
			}

			T returnData = temp->data;
			currentNode = temp;

			return returnData;
		}

	};

	bool Insert(LinkedListIterator pos, const T& element);
	bool Erase(LinkedListIterator& pos);
	LinkedListIterator GetIterator();
};

template <class T>
Node<T>::Node() {
	this->next = NULL;
}

template <class T>
Node<T>::Node(const T data) {
	this->next = NULL;
	this->data = data;
}

template <class T, unsigned int N>
LinkedList<T, N>::LinkedList() {
	size = 0;
	first = NULL;
	last = NULL;
}

template <class T, unsigned int N>
LinkedList<T, N>::~LinkedList() {
	Clear();
}

template <class T, unsigned int N>
int LinkedList<T, N>::GetSize() {
	return size;
}

template <class T, unsigned int N>
void LinkedList<T, N>::Clear() {

	Node<T>* temp = first;

	while (temp != NULL) {
		Node<T>* next = temp->next;
		nodePool.Free(temp);
		temp = next;
	}

	first = NULL;
	last = NULL;
	size = 0;
}

template <class T, unsigned int N>
bool LinkedList<T, N>::PushBack(const T& element) {
	if (first == NULL) {

		Node<T>* allocatedNode = nodePool.Allocate();
	
		if (allocatedNode == NULL)
			return false;

		allocatedNode->data = element;
		allocatedNode->next = NULL;

		first = allocatedNode;

		last = first;
		cout << "Element pushed to first place!" << endl;

	}
	else {

		Node<T>* allocatedNode = nodePool.Allocate();

		if (allocatedNode == NULL)
			return false;

		allocatedNode->data = element;
		allocatedNode->next = NULL;

		last->next = allocatedNode;
		cout << "Element pushed to back of the list " << endl;
		last = last->next;
	}

	size++;
	return true;
}

template <class T, unsigned int N>
bool LinkedList<T, N>::PopBack() {

	if (first == NULL) {
		cout << "Linked list is empty!" << endl;
		return false;
	}

	if (first->next == NULL) {
		nodePool.Free(first);
		first = NULL;
		last = NULL;
	}

	Node<T>* temp = first;

	while (temp != NULL) {
		if (temp->next->next == NULL) {
			nodePool.Free(temp->next);
			nodePool.Free(new Node<T>()); // Outside object for testing
			temp->next = NULL;
			last = temp;
			cout << "last item is popped" << endl;
			break;
		}
		temp = temp->next;
	}

	size--;
	return true;
}

template <class T, unsigned int N>
void LinkedList<T, N>::PrintValues() {

	if (first == NULL)
		cout << "Linked list is empty" << endl;

	Node<T>* temp = first;
	cout << "List is: " << endl;
	while (temp != NULL) {
		cout << temp->data << endl;
		temp = temp->next;
	}
}

template <class T, unsigned int N >
T LinkedList<T, N>::Front() {

	if (first == NULL)
		return NULL;

	return first->data;
}

template <class T, unsigned int N>
T LinkedList<T, N>::Back() {
	if (last == NULL)
		return NULL;

	return last->data;
}

template <class T, unsigned int N>
typename LinkedList<T, N>::LinkedListIterator LinkedList<T, N>::GetIterator()
{
	return LinkedListIterator(first);
}

template <class T, unsigned int N>
bool LinkedList<T, N>::Insert(LinkedListIterator pos, const T& element) {

	if (first == pos.currentNode) {
		Node<T>* allocatedNode = nodePool.Allocate();

		if (allocatedNode == NULL)
			return false;

		allocatedNode->data = element;
		allocatedNode->next = NULL;

		first = allocatedNode;
		first->next = pos.currentNode;
		
	}
	else if (last->next == pos.currentNode) {
		PushBack(element);
	}
	else {
		Node<T>* temp = pos.currentNode;
		pos.Prev();

		Node<T>* allocatedNode = nodePool.Allocate();

		if (allocatedNode == NULL)
			return false;

		allocatedNode->data = element;
		allocatedNode->next = NULL;

		pos.currentNode->next = allocatedNode;
		pos.currentNode->next->next = temp;
	}

	size++;
	return true;
}

template <class T, unsigned int N>
bool LinkedList<T, N>::Erase(LinkedListIterator& pos) {

	if (pos.currentNode == NULL) {
		cout << "Can't delete the null node" << endl;
		return false;
	}
	else if (first == pos.currentNode) { // If iterator is at the first element
		Node<T>* temp = first;
		first = first->next;
		nodePool.Free(temp);
		pos.Next();
		
	}
	else if (last == pos.currentNode) { // If iterator is at the end element

		pos.Prev();
		last = pos.currentNode;
		cout << "Current last data is " << last->data << endl;
		if (nodePool.Free(last->next))
			cout << "Free is successful1" << endl;
	}
	else {
		Node<T>* nextNode = pos.currentNode->next;
		pos.Prev();
		
		if (nodePool.Free(pos.currentNode->next))
			cout << "Free is successful2" << endl;

		pos.currentNode->next = nextNode;

	}
	cout << "Erase is succesful3" << endl;
	size--;
	return true;
}