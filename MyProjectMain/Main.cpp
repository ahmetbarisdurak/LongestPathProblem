// MyProject.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <LinkedListLibrary.h>
#include <StaticVectorLibrary.h>
#include <gtest/gtest.h>
#include <fstream>
#include <string>

using namespace std;

#define DISTANCE 200
#define TOLERANCE 50
#define CITY_COUNT 81

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

void readCSVFile(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& cityDistances, StaticVector<std::string, CITY_COUNT>& cityNames) {
	std::ifstream file("ilmesafe.csv"); // Open the CSV file

	if (file.is_open()) {
		std::string line;
		int i = 0;
		int j = 0;

		std::getline(file, line);
		std::getline(file, line);
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string token;
			std::getline(iss, token, ';'); // city plate

			std::getline(iss, token, ';'); // city name
			cityNames.SetIndex(i, token);


			while (std::getline(iss, token, ';')) {
				int value;
				std::istringstream(token) >> value;
				cityDistances.GetIndex(i).SetIndex(j, value);
				j++;
			}

			i++;
			j = 0;

			if (i == 81)
				break;
		}

		file.close(); // Close the CSV file
	
	}
	else {
		std::cout << "File is not open" << std::endl;
	}



}

int dfs(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, StaticVector<bool, CITY_COUNT>& visited, int vertex, int pathLength, StaticVector<int, CITY_COUNT>& visitOrder) {
	visited.SetIndex(vertex, true); // Set index as visited
	visitOrder.SetIndex(pathLength - 1 , vertex); // add the index to visited order 

	int longestPath = pathLength;

	for (int neighbor = 0; neighbor < CITY_COUNT; ++neighbor) {
		if (adjMatrix.GetIndex(vertex).GetIndex(neighbor) >= (DISTANCE - TOLERANCE) && adjMatrix.GetIndex(vertex).GetIndex(neighbor) <= (DISTANCE + TOLERANCE) && !visited.GetIndex(neighbor)) {
			
			int newPathLength = dfs(adjMatrix, visited, neighbor, pathLength + 1, visitOrder);

			if (longestPath < newPathLength) { // finding the max componenet size and it's visiting order
				longestPath = newPathLength;
			}
			else {
				for (int j = 0; j < newPathLength - pathLength + 1; j++)
					visitOrder.PopBack();
			}
		}
	}

	return longestPath;
}

int findMaxConnectedVertices(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix) {
	
	StaticVector<bool, CITY_COUNT> visited(false);
	StaticVector<int, CITY_COUNT> visitOrder(-1);
	StaticVector<int, CITY_COUNT> maxVisitOrder(-1);

	int maxConnected = 0;
	int startVertex = 0;
	int componentSize = 0;

	maxConnected = dfs(adjMatrix, visited, startVertex, 1, visitOrder);

	cout << "Max connected is " << maxConnected << endl;
	cout << visitOrder.GetSize() << " is the size of visit order" << endl;
	
	for (int i = 0; i < visitOrder.GetSize(); i++) {
		if (visitOrder.GetIndex(i) == -1)
			break;
		cout << visitOrder.GetIndex(i) << "->";
	}

	cout << endl;

	return maxConnected;
}

int main() {
	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> adjMatrix;
	
	adjMatrix.GetIndex(0).SetIndex(0, 0);
	adjMatrix.GetIndex(0).SetIndex(1, 230);
	adjMatrix.GetIndex(0).SetIndex(2, 180);
	adjMatrix.GetIndex(0).SetIndex(3, 711);
	adjMatrix.GetIndex(0).SetIndex(4, 234);

	adjMatrix.GetIndex(1).SetIndex(0, 230);
	adjMatrix.GetIndex(1).SetIndex(1, 0);
	adjMatrix.GetIndex(1).SetIndex(2, 400);
	adjMatrix.GetIndex(1).SetIndex(3, 300);
	adjMatrix.GetIndex(1).SetIndex(4, 193);

	adjMatrix.GetIndex(2).SetIndex(0, 180);
	adjMatrix.GetIndex(2).SetIndex(1, 400);
	adjMatrix.GetIndex(2).SetIndex(2, 0);
	adjMatrix.GetIndex(2).SetIndex(3, 230);
	adjMatrix.GetIndex(2).SetIndex(4, 312);

	adjMatrix.GetIndex(3).SetIndex(0, 711);
	adjMatrix.GetIndex(3).SetIndex(1, 300);
	adjMatrix.GetIndex(3).SetIndex(2, 230);
	adjMatrix.GetIndex(3).SetIndex(3, 0);
	adjMatrix.GetIndex(3).SetIndex(4, 215);
	

	adjMatrix.GetIndex(4).SetIndex(0, 234);
	adjMatrix.GetIndex(4).SetIndex(1, 193);
	adjMatrix.GetIndex(4).SetIndex(2, 312);
	adjMatrix.GetIndex(4).SetIndex(3, 215);
	adjMatrix.GetIndex(4).SetIndex(4, 0);

	
	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> cityDistances;
	StaticVector<std::string, CITY_COUNT> cityNames;

	readCSVFile(cityDistances, cityNames);
	

	 findMaxConnectedVertices(cityDistances);  // Output should be 3

	return 0;
}






/*
int main(int argc, char** argv)
{
	StaticVector<StaticVector<int, 81>, 81> cityDistances;
	StaticVector<std::string, 81> cityNames;

	readCSVFile(cityDistances, cityNames);

	for (int i = 0; i < 81; i++) {
		for (int j = 0; j < 81; j++) {
			std::cout << cityDistances.GetIndex(i).GetIndex(j) << " ";
		}
		std::cout << std::endl;
	}

	for (int i = 0; i < 81; i++) {
		std::cout << cityNames.GetIndex(i) << endl;
	}

	return 0;

	
	//::testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();
	
}
*/
