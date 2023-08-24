// MyProject.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <LinkedListLibrary.h>
#include <StaticVectorLibrary.h>
#include <gtest/gtest.h>

using namespace std;

class StaticVectorTest : public ::testing::Test {
protected:
	void SetUp() override {
	}

	void TearDown() override {
	}

};

class LinkedListTest : public ::testing::Test {
protected:
	void SetUp() override {
	}

	void TearDown() override {
	}
};


// Test case for PushBack and GetSize methods
TEST_F(StaticVectorTest, PushBackAndGetSize) {
	StaticVector<int, 5> staticVector;

	staticVector.PushBack(1);
	staticVector.PushBack(2);

	EXPECT_EQ(staticVector.GetSize(), 2);
}

// Test case for PushBack which checks whether the correct value has been pushed or not
TEST_F(StaticVectorTest, PushBackCorrectValue) {
	StaticVector<int, 5> staticVector;

	EXPECT_TRUE(staticVector.PushBack(1));
	EXPECT_EQ(staticVector.GetSize(), 1);
	EXPECT_EQ(staticVector.Last(), 1);
}

// Test case for PushBack exceeding capacity
TEST_F(StaticVectorTest, PushBackExceedingCapacity) {
	StaticVector<int, 2> staticVector;  // Capacity set to 2

	EXPECT_TRUE(staticVector.PushBack(1));
	EXPECT_TRUE(staticVector.PushBack(2));
	EXPECT_FALSE(staticVector.PushBack(3));  // Should fail since out of capacity
	EXPECT_EQ(staticVector.GetSize(), 2);
	EXPECT_EQ(staticVector.Last(), 2);
}


// Test case for PopBack and GetSize methods
TEST_F(StaticVectorTest, PopBackAndGetSize) {
	StaticVector<int, 5> staticVector;

	staticVector.PushBack(1);
	staticVector.PushBack(2);
	staticVector.PopBack();

	EXPECT_EQ(staticVector.GetSize(), 1);
}

// Test case for PopBack whether correct value has been popped
TEST_F(StaticVectorTest, PopBackCorrectValue) {
	StaticVector<int, 5> staticVector;

	staticVector.PushBack(1);
	staticVector.PushBack(2);
	staticVector.PopBack();

	EXPECT_EQ(staticVector.Last(), 1);
	EXPECT_EQ(staticVector.GetSize(), 1);
}

// Test case for PopBack when there is no element
TEST_F(StaticVectorTest, PopBackCapacity) {
	StaticVector<int, 2> staticVector;  // Capacity set to 2

	staticVector.PushBack(1);
	staticVector.PushBack(2);
	staticVector.PopBack();
	staticVector.PopBack();

	
	EXPECT_FALSE(staticVector.PopBack());
}

// Test case for First and Last methods
TEST_F(StaticVectorTest, FirstAndLastTest) {
	StaticVector<int, 5> staticVector;

	staticVector.PushBack(1);
	staticVector.PushBack(2);
	staticVector.PushBack(3);

	EXPECT_EQ(staticVector.First(), 1);
	EXPECT_EQ(staticVector.Last(), 3);
}

TEST_F(StaticVectorTest, ForwardIteratorTest) {
	StaticVector<int, 5> staticVector;

	staticVector.PushBack(1);
	staticVector.PushBack(2);
	staticVector.PushBack(3);

	StaticVector<int, 5>::StaticVectorIterator staticVectorIterator = staticVector.GetIterator();
	int i = 0;
	while (staticVectorIterator.HasNext()) {
		i++;
		EXPECT_EQ(staticVectorIterator.Next(), i);
	}
}

TEST_F(StaticVectorTest, BackwardIteratorTest) {
	StaticVector<int, 5> staticVector;

	staticVector.PushBack(1);
	staticVector.PushBack(2);
	staticVector.PushBack(3);

	StaticVector<int, 5>::StaticVectorIterator staticVectorIterator = staticVector.End();
	int i = 3;
	while (staticVectorIterator.HasPrev()) {
		EXPECT_EQ(staticVectorIterator.Prev(), i);
		i--;
	}
}

// Test case for PushBack and GetSize methods
TEST_F(LinkedListTest, PushBackAndGetSize) {
	LinkedList<int, 5> linkedList;

	linkedList.PushBack(1);
	linkedList.PushBack(2);

	EXPECT_EQ(linkedList.GetSize(), 2);
}

TEST_F(LinkedListTest, PushBackCorrectValue) {
	LinkedList<int, 5> linkedList;

	EXPECT_TRUE(linkedList.PushBack(1));
	EXPECT_EQ(linkedList.GetSize(), 1);
	EXPECT_EQ(linkedList.Back(), 1);
}

// Test case for PushBack exceeding capacity
TEST_F(LinkedListTest, PushBackExceedingCapacity) {
	LinkedList<int, 2> linkedList;  // Capacity set to 2

	EXPECT_TRUE(linkedList.PushBack(1));
	EXPECT_TRUE(linkedList.PushBack(2));
	EXPECT_FALSE(linkedList.PushBack(3));  // Should fail since out of capacity
	EXPECT_EQ(linkedList.GetSize(), 2);
	EXPECT_EQ(linkedList.Back(), 2);
}

TEST_F(LinkedListTest, PopBackAndGetSize) {
	LinkedList<int, 5> linkedList;

	linkedList.PushBack(1);
	linkedList.PushBack(2);
	linkedList.PopBack();

	EXPECT_EQ(linkedList.GetSize(), 1);
}

// Test case for PopBack whether correct value has been popped
TEST_F(LinkedListTest, PopBackCorrectValue) {
	LinkedList<int, 5> linkedList;

	linkedList.PushBack(1);
	linkedList.PushBack(2);
	linkedList.PopBack();

	EXPECT_EQ(linkedList.Back(), 1);
	EXPECT_EQ(linkedList.GetSize(), 1);
}

// Test case for PopBack when there is no element
TEST_F(LinkedListTest, PopBackCapacity) {
	LinkedList<int, 2> linkedList;  // Capacity set to 2

	linkedList.PushBack(1);
	linkedList.PushBack(2);
	linkedList.PopBack();
	linkedList.PopBack();


	EXPECT_FALSE(linkedList.PopBack());
}

// Test case for Insert Correct Value and GetSize
TEST_F(LinkedListTest, InsertAndGetSize) {
	LinkedList<int, 5> linkedList;


	linkedList.PushBack(1);
	linkedList.PushBack(2);
	linkedList.PushBack(3);
	linkedList.PushBack(4);

	LinkedList<int, 5>::LinkedListIterator linkedListIterator = linkedList.GetIterator();

	linkedList.Insert(linkedListIterator, 10);

	EXPECT_EQ(linkedList.Front(), 10);
	EXPECT_EQ(linkedList.GetSize(), 5);
}

// Test case for Insert exceeding capacity
TEST_F(LinkedListTest, InsertExceedingCapacity) {
	LinkedList<int, 5> linkedList;

	linkedList.PushBack(1);
	linkedList.PushBack(2);
	linkedList.PushBack(3);
	linkedList.PushBack(4);

	LinkedList<int, 5>::LinkedListIterator linkedListIterator = linkedList.GetIterator();

	linkedList.Insert(linkedListIterator, 10);

	EXPECT_FALSE(linkedList.Insert(linkedListIterator, 15));
}

TEST_F(LinkedListTest, EraseAndGetSize) {
	LinkedList<int, 5> linkedList;

	linkedList.PushBack(1);
	linkedList.PushBack(2);
	linkedList.PushBack(3);
	linkedList.PushBack(4);

	LinkedList<int, 5>::LinkedListIterator linkedListIterator = linkedList.GetIterator();

	linkedList.Insert(linkedListIterator, 10);
	
	EXPECT_EQ(linkedList.Front(), 10);

	linkedListIterator = linkedList.GetIterator();

	linkedList.Erase(linkedListIterator);

	EXPECT_EQ(linkedList.Front(), 1);

	EXPECT_EQ(linkedList.GetSize(), 4);
}

// Test case for Erase when there is no element
TEST_F(LinkedListTest, EraseExceedingCapacity) {
	LinkedList<int, 2> linkedList;  // Capacity set to 2

	linkedList.PushBack(1);
	linkedList.PushBack(2);

	LinkedList<int, 2>::LinkedListIterator linkedListIterator = linkedList.GetIterator();

	linkedList.Erase(linkedListIterator);
	linkedList.Erase(linkedListIterator);
	

	EXPECT_FALSE(linkedList.Erase(linkedListIterator));
}

TEST_F(LinkedListTest, ForwardIteratorTest) {
	LinkedList<int, 5> linkedList;

	linkedList.PushBack(1);
	linkedList.PushBack(2);
	linkedList.PushBack(3);

	LinkedList<int, 5>::LinkedListIterator linkedListIterator = linkedList.GetIterator();
	
	int i = 0;
	while (linkedListIterator.HasNext()) {
		i++;
		EXPECT_EQ(linkedListIterator.Next(), i);
	}
}

TEST_F(LinkedListTest, BackwardIteratorTest) {
	LinkedList<int, 5> linkedList;

	linkedList.PushBack(1);
	linkedList.PushBack(2);
	linkedList.PushBack(3);

	LinkedList<int, 5>::LinkedListIterator linkedListIterator = linkedList.GetIterator();

	while (linkedListIterator.HasNext()) {
		linkedListIterator.Next();
	}

	int i = 3;
	while (linkedListIterator.HasPrev()) {
		EXPECT_EQ(linkedListIterator.Prev(), i);
		i--;
	}
}

TEST_F(LinkedListTest, ClearTest) {
	LinkedList<int, 5> linkedList;

	linkedList.PushBack(1);
	linkedList.PushBack(2);
	linkedList.PushBack(3);

	EXPECT_EQ(linkedList.GetSize(), 3);

	linkedList.Clear();

	EXPECT_EQ(linkedList.GetSize(), 0);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
