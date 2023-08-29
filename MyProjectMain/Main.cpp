// MyProject.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <ostream>
#include <LinkedListLibrary.h>
#include <StaticVectorLibrary.h>
#include <fstream>
#include <sstream>
#include <string>
#include <LinkedListUnitTest.cpp>
#include <StaticVectorUnitTest.cpp>
//#include <GeneticAlgorithm.h>


#define DISTANCE 200
#define TOLERANCE 50
#define CITY_COUNT 81 // 81
#define START 5

// Reading CSV file and writing into StaticVectors
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
/*
int dfs(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, StaticVector<bool, CITY_COUNT>& visited, int vertex, int pathLength, LinkedList<int, CITY_COUNT>& visitOrder) {
	visited.SetIndex(vertex, true); // Set index as visited
	visitOrder.PushBack(vertex);

	int longestPath = pathLength;

	for (int neighbor = 0; neighbor < CITY_COUNT; ++neighbor) {
		if (adjMatrix.GetIndex(vertex).GetIndex(neighbor) >= (DISTANCE - TOLERANCE) && adjMatrix.GetIndex(vertex).GetIndex(neighbor) <= (DISTANCE + TOLERANCE) && !visited.GetIndex(neighbor)) {
			
			LinkedList<int, CITY_COUNT> subOrder;

			int newPathLength = dfs(adjMatrix, visited, neighbor, pathLength + 1, subOrder);

			if (longestPath < newPathLength) { // finding the max componenet size and it's visiting order

				LinkedList<int, CITY_COUNT>::LinkedListIterator llIterator = subOrder.GetIterator();

				visitOrder.Clear();
				visitOrder.PushBack(vertex);

				while (llIterator.HasNext()) {
					visitOrder.PushBack(llIterator.Next());
				}

				longestPath = newPathLength;
			}
		}
	}

	return longestPath;
}*/

// DFS that returns the path order
LinkedList<int, CITY_COUNT> dfs(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, StaticVector<bool, CITY_COUNT>& visited, int currentVertex) {
	visited.SetIndex(currentVertex, true); // Set index as visited
	LinkedList<int, CITY_COUNT> longestPath;
		 
	for (int neighbor = 0; neighbor < CITY_COUNT; ++neighbor) {
		if (adjMatrix.GetIndex(currentVertex).GetIndex(neighbor) >= (DISTANCE - TOLERANCE) && adjMatrix.GetIndex(currentVertex).GetIndex(neighbor) <= (DISTANCE + TOLERANCE) && !visited.GetIndex(neighbor)) {

			LinkedList<int, CITY_COUNT> neighborPath = dfs(adjMatrix, visited, neighbor);

			if (neighborPath.GetSize() > longestPath.GetSize()) { // finding the max componenet size and it's visiting order

				longestPath.Clear();

				LinkedListIterator<int, CITY_COUNT> llIterator = neighborPath.GetIterator();

				while (llIterator.HasNext()) {
					longestPath.PushBack(llIterator.Next());
				}
			}
		}
	}
	longestPath.Insert(longestPath.GetIterator(), currentVertex); // Insertin the current vertex to start of the path
	
	return longestPath;
}

void findMaxConnectedVertices(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix) {
	
	StaticVector<bool, CITY_COUNT> visited(false);
	LinkedList<int, CITY_COUNT> visitOrder;

	int startVertex = 80; // city plate number

	
	for (startVertex = 0; startVertex < CITY_COUNT; ++startVertex) {

		StaticVector<bool, CITY_COUNT> visited(false);
		LinkedList<int, CITY_COUNT> visitOrder;

		visitOrder = dfs(adjMatrix, visited, startVertex);

		//if(startVertex == CITY_COUNT - 1)
		std::cout << "Visit order is: " << std::endl;
		//std::cout << visitOrder;

		std::cout << "Size of the final values are: " << visitOrder.GetSize() << std::endl;
	}
	/*
	
	visitOrder = dfs(adjMatrix, visited, startVertex);
	
	std::cout << "Found the final values " << std::endl;

	std::cout << visitOrder << std::endl;

	std::cout << "Size of the final values are: " << visitOrder.GetSize() << std::endl;
	*/
}

/*
* Used For Test Purposes 
* 
* 
void findLongestPath(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, int startCity, std::vector<bool>& visitedCities, int currentDistance, std::vector<int>& currentPath, int& maxDistance, std::vector<int>& maxPath) {
	visitedCities[startCity] = true;
	currentPath.push_back(startCity);

	if (currentDistance > maxDistance) {
		maxDistance = currentDistance;
		maxPath = currentPath;
	}

	for (int city = 0; city < CITY_COUNT; ++city) {
		if (!visitedCities[city] && adjMatrix.GetIndex(startCity).GetIndex(city) >= DISTANCE - TOLERANCE && adjMatrix.GetIndex(startCity).GetIndex(city) <= DISTANCE + TOLERANCE) {
			int newDistance = currentDistance + 1;
			vector<int> newPath = currentPath;

			findLongestPath(adjMatrix, city, visitedCities, newDistance, newPath, maxDistance, maxPath);
		}
	}

}
*/

void RunTests() {

	RunStaticVectorTests();
	RunLinkedListTests();
}

int main() {
	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> adjMatrix;
	
	adjMatrix.GetIndex(0).SetIndex(0, 0);
	adjMatrix.GetIndex(0).SetIndex(1, 0);
	adjMatrix.GetIndex(0).SetIndex(2, 200);
	adjMatrix.GetIndex(0).SetIndex(3, 0);
	adjMatrix.GetIndex(0).SetIndex(4, 200);
	adjMatrix.GetIndex(0).SetIndex(5, 0);
	adjMatrix.GetIndex(0).SetIndex(6, 200);

	adjMatrix.GetIndex(6).SetIndex(0, 200);
	adjMatrix.GetIndex(6).SetIndex(1, 0);
	adjMatrix.GetIndex(6).SetIndex(2, 0);
	adjMatrix.GetIndex(6).SetIndex(3, 0);
	adjMatrix.GetIndex(6).SetIndex(4, 0);
	adjMatrix.GetIndex(6).SetIndex(5, 0);
	adjMatrix.GetIndex(6).SetIndex(6, 0);

	adjMatrix.GetIndex(2).SetIndex(0, 200);
	adjMatrix.GetIndex(2).SetIndex(1, 0);
	adjMatrix.GetIndex(2).SetIndex(2, 0);
	adjMatrix.GetIndex(2).SetIndex(3, 200);
	adjMatrix.GetIndex(2).SetIndex(4, 0);
	adjMatrix.GetIndex(2).SetIndex(5, 0);
	adjMatrix.GetIndex(2).SetIndex(6, 0);

	adjMatrix.GetIndex(3).SetIndex(0, 0);
	adjMatrix.GetIndex(3).SetIndex(1, 0);
	adjMatrix.GetIndex(3).SetIndex(2, 200);
	adjMatrix.GetIndex(3).SetIndex(3, 0);
	adjMatrix.GetIndex(3).SetIndex(4, 0);
	adjMatrix.GetIndex(3).SetIndex(5, 0);
	adjMatrix.GetIndex(3).SetIndex(6, 0);

	adjMatrix.GetIndex(4).SetIndex(0, 200);
	adjMatrix.GetIndex(4).SetIndex(1, 0);
	adjMatrix.GetIndex(4).SetIndex(2, 0);
	adjMatrix.GetIndex(4).SetIndex(3, 0);
	adjMatrix.GetIndex(4).SetIndex(4, 0);
	adjMatrix.GetIndex(4).SetIndex(5, 200);
	adjMatrix.GetIndex(4).SetIndex(6, 0);

	adjMatrix.GetIndex(5).SetIndex(0, 0);
	adjMatrix.GetIndex(5).SetIndex(1, 200);
	adjMatrix.GetIndex(5).SetIndex(2, 0);
	adjMatrix.GetIndex(5).SetIndex(3, 0);
	adjMatrix.GetIndex(5).SetIndex(4, 200);
	adjMatrix.GetIndex(5).SetIndex(5, 0);
	adjMatrix.GetIndex(5).SetIndex(6, 0);
   
	adjMatrix.GetIndex(1).SetIndex(0, 0);
	adjMatrix.GetIndex(1).SetIndex(1, 0);
	adjMatrix.GetIndex(1).SetIndex(2, 0);
	adjMatrix.GetIndex(1).SetIndex(3, 0);
	adjMatrix.GetIndex(1).SetIndex(4, 0);
	adjMatrix.GetIndex(1).SetIndex(5, 200);
	adjMatrix.GetIndex(1).SetIndex(6, 0);

	//RunTests();


	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> cityDistances;
	StaticVector<std::string, CITY_COUNT> cityNames;

	readCSVFile(cityDistances, cityNames);


	//GeneticAlgorithmUtil<int, CITY_COUNT>(cityDistances);
	

	findMaxConnectedVertices(cityDistances);  // Output should be 3

	/*
	
	int startCity = 0;  // Replace with the index of your starting city
	int numCities = CITY_COUNT;

	vector<bool> visitedCities(numCities, false);
	int initialDistance = 0;
	vector<int> initialPath;

	int longestDistance = 0;
	vector<int> longestPath;

	findLongestPath(adjMatrix, startCity, visitedCities, initialDistance, initialPath, longestDistance, longestPath);
	int i = 0;
	cout << "Longest Path:";
	for (int city : longestPath) {
		cout << " " << city;
		i++;
	}
	cout << endl;
	cout << " i is " << i << endl;
	cout << "Longest Distance: " << longestDistance << endl;
	*/


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
