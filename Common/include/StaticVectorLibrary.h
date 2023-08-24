#pragma once

#include "iterator.h"

#include<iostream>

using namespace std;

template <class T, unsigned int N>
class StaticVector {
public:
	StaticVector();

	StaticVector(T element);

	~StaticVector() = default;

	// Printing the elements of the vector
	void PrintData();

	// adding a new element at the end of the vector
	bool PushBack(T element);

	// Removes the last element in the vector
	bool PopBack();

	// returns the first element of the vector
	T First();

	// returns the last element of the vector
	T Last();

	// returns the size of the vector
	int GetSize();

	T& GetIndex(int index);
	
	void SetIndex(int index, T element);

	class StaticVectorIterator : public Iterator<T> {
	private:
		int currentIndex = 0;
		int vectorSize;
		T* vectorData;
		template<typename U, unsigned int N>
		friend class StaticVector;
	public:
		// For general iterator
		StaticVectorIterator(int size, T* staticVectorData) : vectorSize(size), vectorData(staticVectorData) {
		}

		//  For specific iterator with given index
		StaticVectorIterator(int index, int size, T* staticVectorData) :
			currentIndex(index), vectorSize(size), vectorData(staticVectorData) {
		}

		// Checks if iterator has a next element or not
		bool HasNext() {
			if (currentIndex < vectorSize)
				return true;

			return false;
		}

		//Iterates through the next element and returns it
		T Next() {
			if (!HasNext())
				return {};

			return vectorData[currentIndex++];
		}

		// Checks if iterator has a previous element or not
		bool HasPrev() {
			if (currentIndex > 0)
				return true;

			return false;
		}

		//Iterates through the previous element and returns it
		T Prev() {
			if (!HasPrev())
				return {};

			return vectorData[--currentIndex];
		}

	};

	// Starts iterator index from the beginning of the vector
	StaticVectorIterator Begin();

	// Starts iterator index from the ending of the vector
	StaticVectorIterator End();

	// Starts iterator with first index as general
	StaticVectorIterator GetIterator();

private:
	const int capacity = N;
	int size;
	T staticVectorData[N];
};

template <class T, unsigned int N>
StaticVector<T, N>::StaticVector() : size(0) {}


template <class T, unsigned int N>
StaticVector<T, N>::StaticVector(T element) {
	size = N;
	for (int i = 0; i < N; i++) {
		staticVectorData[i] = element;
	}
}


template <class T, unsigned int N>
bool StaticVector<T, N>::PushBack(T element) {
	if (size >= capacity) {
		cout << "Out of capacity" << endl;
		return false;
	}
	else {
		staticVectorData[size++] = element;
		return true;
	}
}


template <class T, unsigned int N>
T& StaticVector<T, N>::GetIndex(int index) {
	return staticVectorData[index];
}


template <class T, unsigned int N>
void StaticVector<T, N>::SetIndex(int index, T element) {
	staticVectorData[index] = element;

}
template <class T, unsigned int N>
bool StaticVector<T, N>::PopBack() {
	if (size == 0)
		return false;
	else {
		size--;
		return true;
	}
}

template <class T, unsigned int N>
int StaticVector<T, N>::GetSize() {
	return size;
}

template <class T, unsigned int N>
T StaticVector<T, N>::First() {
	return staticVectorData[0];
}

template <class T, unsigned int N>
T StaticVector<T, N>::Last() {
	return staticVectorData[size - 1];
}

template <class T, unsigned int N>
void StaticVector<T, N>::PrintData() {
	for (int i = 0; i < size; i++)
		cout << staticVectorData[i] << " ";

	cout << endl;
}

template <class T, unsigned int N>
typename StaticVector<T, N>::StaticVectorIterator StaticVector<T, N>::Begin() {
	return StaticVectorIterator(0, size, staticVectorData);
}

template <class T, unsigned int N>
typename StaticVector<T, N>::StaticVectorIterator StaticVector<T, N>::End() {
	return StaticVectorIterator(size, size, staticVectorData);
}

template <class T, unsigned int N>
typename StaticVector<T, N>::StaticVectorIterator StaticVector<T, N>::GetIterator()
{
	return StaticVectorIterator(size, staticVectorData);
}


