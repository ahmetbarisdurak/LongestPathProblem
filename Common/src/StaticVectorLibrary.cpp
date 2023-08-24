#include <StaticVectorLibrary.h>


/*
using namespace std;

template <class T, unsigned int N>
StaticVector<T, N>::StaticVector() : size(0) {
	cout << "Creating the StaticVector" << endl;
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
typename StaticVector<T, N>::StaticVectorIterator StaticVector<T, N>::Iterator()
{
	return StaticVectorIterator(size, staticVectorData);
}
*/