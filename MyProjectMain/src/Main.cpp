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
#include <GeneticAlgorithm.cpp>


#define CITY_COUNT 81
#define START 5

// Reading CSV file and writing into StaticVectors
void readCSVFile(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& cityDistances, StaticVector<std::string, CITY_COUNT>& cityNames) {
	std::ifstream file("ilmesafe.csv", std::ios::in | std::ios::binary); // Open the CSV file

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
			cityNames[i] = token;


			while (std::getline(iss, token, ';')) {
				int value;
				std::istringstream(token) >> value;
				cityDistances[i][j] = value;
				j++;
			}

			i++;
			j = 0;

			if (i == CITY_COUNT)
				break;
		}

		file.close(); // Close the CSV file
	}
	else
		std::cout << "File is not open" << std::endl;
}

// Testing the static vector and linked list classes
void RunTests() {

	RunStaticVectorTests();
	RunLinkedListTests();
}

bool WriteToFile(std::string fileName, StaticVector<std::string, CITY_COUNT>& cityNames, StaticVector<int, CITY_COUNT>& foundPath) {

	// Open an output file stream
	std::ofstream outputFile(fileName.c_str(), std::ios::out | std::ios::binary);

	std::cout << fileName << " is filename" << std::endl;

	if (!outputFile.is_open()) {
		std::cerr << "Failed to open the output file." << std::endl;
		return 1; // Exit with an error code
	}

	std::string startingCityName = cityNames[foundPath[0]];

	outputFile << "PATH FOUND FOR " << foundPath[0] + 1 << " : " << startingCityName << std::endl;
	outputFile << "PATH LENGTH IS -> " << foundPath.GetSize() << std::endl;

	for (int i = 0; i < foundPath.GetSize(); ++i) {
		outputFile << cityNames[foundPath[i]] << std::endl;
	}

	// Close the output file
	outputFile.close();

	return 0;

}


// Checking if the traversed path is correct or not
bool CheckPath(std::string fileName, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& cityDistances, StaticVector<std::string, CITY_COUNT> cityNames) {

	StaticVector<int, CITY_COUNT> foundPath;
	int visited[CITY_COUNT] = { 0 };

	std::ifstream file(fileName, std::ios::in | std::ios::binary); // Open the CSV file

	if (!file.is_open()) {
		std::cerr << "Failed to open the output file." << std::endl;
		return 1; // Exit with an error code
	}

	std::string city;
	std::getline(file, city);
	std::getline(file, city);

	while (!file.eof()) {
		std::getline(file, city);

		for (int i = 0; i < CITY_COUNT; ++i) {
			if (cityNames[i] == city) {
				foundPath.PushBack(i);
			}
		}
	}

	file.close();

	for (int i = 0; i < foundPath.GetSize(); ++i)
		visited[foundPath[i]]++;

	for (int i = 0; i < CITY_COUNT; ++i) {
		if (visited[i] > 1) {
			std::cout << "There is a duplicate element in the path" << std::endl;
			return false;
		}
	}

	for (int i = 0; i < foundPath.GetSize() - 1; ++i) {
		int distance = cityDistances[foundPath[i]][foundPath[i + 1]];
		if (distance < DISTANCE - TOLERANCE || distance > DISTANCE + TOLERANCE) {
			std::cout << "These two are not neighbors (" << foundPath[i] << ", " << foundPath[i + 1] << ")" << std::endl;
			return false;
		}
	}

	return true;
}

void FindMinimumDistanceTolerance(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> adjMatrix, Algorithms<int, CITY_COUNT>** algorithms){

	int distance = 220;
	int tolerance = 50;
	bool visited[CITY_COUNT] = { false };
	int maximumDistance = 0;
	int maximumTolerance = 0;
	int maximumResult = 0;

	while (distance > 0) {
		tolerance = 50;

		while (tolerance > 0) {
			StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> graph1 = adjMatrix;

			CreateGraph(graph1, distance, tolerance);

			for (int i = 0; i < CITY_COUNT; ++i) visited[i] = false;

			std::cout << "Testing for " << distance << " and " << tolerance << std::endl;
 			int score = FindLongestPathCombination(graph1, visited, START, -1, algorithms);
			std::cout << "Found score is " << score << std::endl;

			if (score >= maximumResult) {
				maximumDistance = distance;
				maximumTolerance = tolerance;
				maximumResult = score;
			}


			tolerance -= 1;
		}

		distance -= 1;
	}

	std::cout << "OPTIMUM DISTANCE AND TOLERANCE: " << maximumDistance << ", " << maximumTolerance << " WITH CITY COUNT:" << maximumResult << std::endl;
}
/*
void find_optimum_xy2(LocalOptima& lo) {
	Vector<Vector<int, SIZE>, SIZE> graph = cities;
	int distance = 1000, tolerance = 100;
	bool visited[SIZE] = { false };
	double max = 0;
	int maxdist = 0, maxtolerance = 0, globalmax = 0;

	for (; distance >= 100; distance -= 3) {
		for (; tolerance > 10; tolerance -= 2) {
			graph = cities;
			init_graph(distance, tolerance, graph);
			//for (int i = 0; i < SIZE; ++i) visited[i] = false;
			//int cur = find_max_travel(5, -1, visited, graph, lo);

			double nt = tolerance / 100.0, nd = (distance - 100) / 100.0;
			int edges = edge_count(graph); for (int i = 0; i < SIZE; ++i) visited[i] = false;
			double score = (double)find_max_travel(5, -1, visited, graph, lo) / (nt + nd);
			if (score > max) { max = score; maxdist = distance; maxtolerance = tolerance; }
		}
	}


	init_graph(maxdist, maxtolerance, graph);
	std::cout << "OPTIMUM DISTANCE AND TOLERANCE: " << maxdist << ", " << maxtolerance << " WITH CITY COUNT:" << find_max_travel(5, -1, visited, graph, lo) << std::endl;
}

*/
int main() {
	//RunTests();
	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> cityDistances;
	StaticVector<std::string, CITY_COUNT> cityNames;

	readCSVFile(cityDistances, cityNames);

	CreateGraph(cityDistances, DISTANCE, TOLERANCE);
	
	Algorithms<int, CITY_COUNT>* algorithms[4];
	FirstOrderNeighbors<int, CITY_COUNT> firstOrderNeighbors;
	SecondOrderNeighbors<int, CITY_COUNT> secondOrderNeighbors;
	ThirdOrderNeighbors<int, CITY_COUNT> thirdOrderNeighbors;
	ClosenessCentrality<int, CITY_COUNT> closenessCentrality;
	BetweennessCentrality<int, CITY_COUNT> betweennessCentrality;

	algorithms[0] = &firstOrderNeighbors;
	algorithms[1] = &secondOrderNeighbors;
	algorithms[2] = &thirdOrderNeighbors;
	algorithms[3] = &closenessCentrality;


	FindMinimumDistanceTolerance(cityDistances, algorithms);



	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> graph = cityDistances;
	bool visited[CITY_COUNT];
	

	for (int i = 0; i < CITY_COUNT; ++i) visited[i] = false;
	std::cout << "First Order Neighbor Score" << std::endl;
	std::cout << "Found score is: " << FindLongestPathAlgorithms(graph, visited, START, -1, firstOrderNeighbors) << std::endl;
	std::cout << "-------------------------------------" << std::endl;

	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> graph2 = cityDistances;
	for (int i = 0; i < CITY_COUNT; ++i) visited[i] = false;
	std::cout << "Second Order Neighbor Score " << std::endl;
	std::cout << "Found score is: " << FindLongestPathAlgorithms(graph2, visited, START, -1, secondOrderNeighbors) << std::endl;
	std::cout << "-------------------------------------" << std::endl;



	for (int i = 0; i < CITY_COUNT; ++i) visited[i] = false;
	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> graph3 = cityDistances;
	std::cout << "Third Order Neighbor Score " << std::endl;
	std::cout << "Found score is: " << FindLongestPathAlgorithms(graph3, visited, START, -1, thirdOrderNeighbors) << std::endl;
	std::cout << "-------------------------------------" << std::endl;

	for (int i = 0; i < CITY_COUNT; ++i) visited[i] = false;
	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> graph4 = cityDistances;
	std::cout << "Closeness Centrality Score " << std::endl;
	std::cout << "Found score is: " << FindLongestPathAlgorithms(graph4, visited, START, -1, closenessCentrality) << std::endl;
	std::cout << "-------------------------------------" << std::endl;


	for (int i = 0; i < CITY_COUNT; ++i) visited[i] = false;
	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> graph7 = cityDistances;
	std::cout << "Betweenness Centrality Score " << std::endl;
	//std::cout << "Found score is: " << FindLongestPathAlgorithms(graph7, visited, START, -1, bcbetweennessCentrality << std::endl;
	std::cout << "-------------------------------------" << std::endl;

	
	std::cout << "Combination" << std::endl;
	int correctPathCount = 0;
	std::string fileName = "results/";
	
	int j = START;
	
	//for (int j = 0; j < CITY_COUNT; ++j) {
		foundPath = StaticVector<int, CITY_COUNT>();
		StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> graph5 = cityDistances;
		for (int i = 0; i < CITY_COUNT; ++i) visited[i] = false;
		foundPath.PushBack(j);
		std::cout << "Combination for city: " << j << "->" << FindLongestPathCombination(graph5, visited, j, -1, algorithms) << std::endl;
		std::cout << "Found path size is " << foundPath.GetSize() << std::endl;
		std::cout << foundPath;
	
		WriteToFile(fileName + cityNames[foundPath[0]] + ".txt", cityNames, foundPath);

		if (CheckPath(fileName + cityNames[foundPath[0]] + ".txt", cityDistances, cityNames)) {
			std::cout << "This path is correct" << std::endl;
			correctPathCount++;
		}

		std::cout << "-------------------------------------" << std::endl;
	//}
	std::cout << "-------------------------------------" << std::endl;

	
	std::cout << "Correct path count is: " << correctPathCount << std::endl;
	std::cout << "-------------------------------------" << std::endl;

	// -------------------------------------------------------------------------------------------- \\
	
	//GeneticAlgorithmUtil<int, CITY_COUNT>(cityDistances);
	
	return 0;
}
