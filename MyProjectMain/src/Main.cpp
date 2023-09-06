// MyProject.cpp : Defines the entry point for the application.
//
// Pre-defined libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// My Libraries
#include <LinkedListLibrary.h>
#include <StaticVectorLibrary.h>
#include <LinkedListUnitTest.cpp>
#include <StaticVectorUnitTest.cpp>
#include <Algorithms.h>
#include <HeuristicApproaches.cpp>

#define DISTANCE 250
#define TOLERANCE 50
#define CITY_COUNT 81
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

// Testing the static vector and linked list classes
void RunTests() {

	RunStaticVectorTests();
	RunLinkedListTests();
}

// Checking if the traversed path is correct or not
bool CheckPath(StaticVector<int, CITY_COUNT>& path, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& cityDistances) {

	int visited[CITY_COUNT];

	for (int i = 0; i < CITY_COUNT; ++i)
		visited[i] = 0;

	for (int i = 0; i < CITY_COUNT; ++i) {
		if (visited[i] == 1) {
			std::cout << "There is a duplicate element in the path" << std::endl;
			return false;
		}
		visited[i]++;
	}

	for (int i = 0; i < path.GetSize() - 1; ++i) {
		int distance = cityDistances[path[i]][path[i + 1]];
		if (distance < DISTANCE - TOLERANCE || distance > DISTANCE + TOLERANCE) {
			std::cout << "These two are not neighbors (" << path[i] << ", " << path[i+1] << ")" << std::endl;
			return false;
		}
	}

}

int main() {
	//RunTests();

	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> cityDistances;
	StaticVector<std::string, CITY_COUNT> cityNames;

	readCSVFile(cityDistances, cityNames);

	CreateGraph(cityDistances);
	
	FirstOrderNeighbors<int, CITY_COUNT> firstOrderNeighbors;
	SecondOrderNeighbors<int, CITY_COUNT> secondOrderNeighbors;
	ThirdOrderNeighbors<int, CITY_COUNT> thirdOrderNeighbors;
	ClosenessCentrality<int, CITY_COUNT> closenessCentrality;
	BetweennessCentrality<int, CITY_COUNT> betweennessCentrality;
	Algorithms<int, CITY_COUNT>* algorithms[4];

	algorithms[0] = &firstOrderNeighbors;
	algorithms[1] = &secondOrderNeighbors;
	algorithms[2] = &thirdOrderNeighbors;
	algorithms[3] = &closenessCentrality;


	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> graph = cityDistances;
	bool visited[CITY_COUNT];
	

	for (int i = 0; i < CITY_COUNT; ++i) visited[i] = false;
	std::cout << "First Order Neighbor Score " << std::endl;
	std::cout << FindLongestPathAlgorithms(graph, visited, START, -1, firstOrderNeighbors) << std::endl;
	std::cout << "-------------------------------------" << std::endl;

	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> graph2 = cityDistances;
	for (int i = 0; i < CITY_COUNT; ++i) visited[i] = false;
	std::cout << "Second Order Neighbor Score " << std::endl;
	std::cout << FindLongestPathAlgorithms(graph2, visited, START, -1, secondOrderNeighbors) << std::endl;
	std::cout << "-------------------------------------" << std::endl;



	for (int i = 0; i < CITY_COUNT; ++i) visited[i] = false;
	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> graph3 = cityDistances;
	std::cout << "Third Order Neighbor Score " << std::endl;
	std::cout << FindLongestPathAlgorithms(graph3, visited, START, -1, thirdOrderNeighbors) << std::endl;
	std::cout << "-------------------------------------" << std::endl;

	for (int i = 0; i < CITY_COUNT; ++i) visited[i] = false;
	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> graph4 = cityDistances;
	std::cout << "Closeness Centrality Score " << std::endl;
	std::cout << FindLongestPathAlgorithms(graph4, visited, START, -1, closenessCentrality) << std::endl;
	std::cout << "-------------------------------------" << std::endl;


	for (int i = 0; i < CITY_COUNT; ++i) visited[i] = false;
	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> graph7 = cityDistances;
	std::cout << "Betweenness Centrality Score " << std::endl;
	//std::cout << FindLongestPathAlgorithms(graph7, visited, START, -1, bcbetweennessCentrality << std::endl;
	std::cout << "-------------------------------------" << std::endl;

	
	std::cout << "Combination" << std::endl;
	int correctPathCount = 0;

	for (int j = 0; j < CITY_COUNT; ++j) {
		foundPath = StaticVector<int, CITY_COUNT>();
		StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> graph5 = cityDistances;
		for (int i = 0; i < CITY_COUNT; ++i) visited[i] = false;
		foundPath.PushBack(j);
		std::cout << "Combination for city: " << j << "->" << FindLongestPathCombination(graph5, visited, j, -1, algorithms) << std::endl;
		std::cout << "Found path size is " << foundPath.GetSize() << std::endl;
		std::cout << foundPath << std::endl;
	
		if (CheckPath(foundPath, cityDistances)) {
			std::cout << "This path is correct" << std::endl;
			correctPathCount++;
		}

	}
	std::cout << "-------------------------------------" << std::endl;

	
	std::cout << "Correct path count is: " << correctPathCount << std::endl;
	std::cout << "-------------------------------------" << std::endl;

	// -------------------------------------------------------------------------------------------- \\
	//GeneticAlgorithmUtil<int, CITY_COUNT>(adjMatrix);
	

	return 0;
}
