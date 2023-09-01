// MyProject.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <LinkedListLibrary.h>
#include <StaticVectorLibrary.h>
#include <fstream>
#include <sstream>
#include <string>
#include <LinkedListUnitTest.cpp>
#include <StaticVectorUnitTest.cpp>
#include <HeuristicApproaches.cpp>

#define DISTANCE 250
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


				LinkedListIterator<int, CITY_COUNT> longestPathIterator = longestPath.GetIterator();

				while (longestPathIterator.HasNext()) {
					std::cout << "unvisited";
					visited.SetIndex(longestPathIterator.Next(), false);
				}

				longestPath.Clear();

				LinkedListIterator<int, CITY_COUNT> llIterator = neighborPath.GetIterator();

				while (llIterator.HasNext()) {
					int temp = llIterator.Next();
					visited.SetIndex(temp, true);
					longestPath.PushBack(temp);

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

LinkedList<int, CITY_COUNT> DFSLongestPath(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, StaticVector<bool, CITY_COUNT>& visited, int currentVertex, int targetVertex, int& maxDepth) {
	visited.SetIndex(currentVertex, true); // Set index as visited
	LinkedList<int, CITY_COUNT> longestPath;

	for (int neighbor = 0; neighbor < CITY_COUNT; ++neighbor) {
		if (adjMatrix.GetIndex(currentVertex).GetIndex(neighbor) >= (DISTANCE - TOLERANCE) && adjMatrix.GetIndex(currentVertex).GetIndex(neighbor) <= (DISTANCE + TOLERANCE) && !visited.GetIndex(neighbor)) {

			LinkedList<int, CITY_COUNT> neighborPath = dfs(adjMatrix, visited, neighbor);

			if (neighborPath.GetSize() > maxDepth && neighborPath.Back() == targetVertex) { // finding the max componenet size and it's visiting order

				longestPath.Clear();

				LinkedListIterator<int, CITY_COUNT> llIterator = neighborPath.GetIterator();

				while (llIterator.HasNext()) {
					longestPath.PushBack(llIterator.Next());
				}

				maxDepth = neighborPath.GetSize();
			}
		}
	}
	longestPath.Insert(longestPath.GetIterator(), currentVertex); // Insertin the current vertex to start of the path

	return longestPath;





}

/*
int FindNumberOfCities(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, int index1, int index2) {
	StaticVector<bool, CITY_COUNT> visited(false);
	LinkedList<int, CITY_COUNT> visitOrder;
	int maxDepth = 0;
	visitOrder = DFSLongestPath(adjMatrix, visited, index1, index2, maxDepth);

	std::cout << "Visit order size is " << visitOrder.GetSize() << std::endl;
	std::cout << visitOrder;

	return 0;

}

// Perform a 2-Opt swap to maximize the road length
void Perform2OptSwap(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, StaticVector<int, CITY_COUNT>& path) {
	for (int i = 1; i < path.GetSize() - 2; ++i) {
		for (int j = i + 1; j < path.GetSize() - 1; ++j) {
			//int oldEdgesSum = FindNumberOfCities(adjMatrix, i, i + 1) + FindNumberOfCities(adjMatrix, j, j + 1);

			int oldEdgesSum = 2;
			int newEdgesSum = FindNumberOfCities(adjMatrix, i, j) + FindNumberOfCities(adjMatrix, i + 1, j + 1);

			if (newEdgesSum > oldEdgesSum) {
				// Perform the 2-Opt swap
				path.SetIndex(i + 1, path.GetIndex(j));
				path.SetIndex(j, path.GetIndex(i + 1));
			}
		}
	}
}
*/

void NearestNeighborAlgorithm(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, int startingCity) {

	StaticVector<bool, CITY_COUNT> visited(false);
	StaticVector<int, CITY_COUNT> path;

	int currentCityIndex = startingCity;


	path.PushBack(currentCityIndex);
	visited.GetIndex(currentCityIndex) = true;


	while (true) {
		double minDistance = DISTANCE + TOLERANCE;
		int nearestCityIndex = -1;

		for (int i = 0; i < CITY_COUNT; ++i) {
			if (!visited.GetIndex(i)) {
				int distance = adjMatrix.GetIndex(currentCityIndex).GetIndex(i);
				if (distance >= DISTANCE - TOLERANCE && distance <= DISTANCE + TOLERANCE && distance < minDistance) {
					minDistance = distance;
					nearestCityIndex = i;
				}
			}
		}

		if (nearestCityIndex == -1) {
			break; // No neighbors
		}

		path.PushBack(nearestCityIndex);
		visited.SetIndex(nearestCityIndex, true);
		currentCityIndex = nearestCityIndex; // Continue to traverse from neighbor
	}

	std::cout << "Final path is: " << std::endl;
	std::cout << path;
	std::cout << "\n Total of " << path.GetSize() << " cities are traversed" << std::endl;

}
//  method returns farthest node and its distance from node u
int BFS(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, int u, StaticVector<bool, CITY_COUNT>& visited)
{
	//  mark all distance with -1
	StaticVector<int, CITY_COUNT> distances(-1);
	LinkedList<int, CITY_COUNT> queue;

	queue.PushBack(u);

	//  distance of u from u will be 0
	distances.SetIndex(u, 0);

	while (queue.GetSize() != 0)
	{
		int currentVertex = queue.Front();

		queue.Erase(queue.GetIterator());
		for (int neighbor = 0; neighbor < CITY_COUNT; ++neighbor) {
			int distance = adjMatrix.GetIndex(currentVertex).GetIndex(neighbor);
			if (distance > (DISTANCE - TOLERANCE) && distance < (DISTANCE + TOLERANCE)) {

				if (distances.GetIndex(neighbor) == -1) {
					queue.PushBack(neighbor);

					distances.SetIndex(neighbor, distances.GetIndex(currentVertex) + 1);

				}
			}
		}
	}
	int maxDistance = 0;
	int nodeIdx = -1;

	//  get farthest node distance and its index
	for (int i = 0; i < CITY_COUNT; i++)
	{
		if (distances.GetIndex(i) > maxDistance && !visited.GetIndex(i)) {
			maxDistance = distances.GetIndex(i);
			nodeIdx = i;
		}
	}
	
	std::cout << "maxDistance from " << u << " to " << nodeIdx << " is " << maxDistance << std::endl;
	
	return nodeIdx;
}

void BFSSearchLongest(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>&adjMatrix) {
	//int t1, t2;

	// first bfs to find one end point of
	// longest path
	//t1 = BFS(adjMatrix, 22);

	//  second bfs to find actual longest path
	//t2 = BFS(adjMatrix, t1);

}

int FindNeighborCount(int startingCity, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, StaticVector<bool, CITY_COUNT>& visited) {

	int mostNeighbor = 0;
	int mostNeighborIndex = -1;

	for (int i = 0; i < CITY_COUNT; i++) {

		int temp = 0;
		if (adjMatrix.GetIndex(startingCity).GetIndex(i) <= DISTANCE + TOLERANCE && adjMatrix.GetIndex(startingCity).GetIndex(i) >= DISTANCE - TOLERANCE && !visited.GetIndex(i)) {
			for (int j = 0; j < CITY_COUNT; j++) {
				if (adjMatrix.GetIndex(i).GetIndex(j) <= DISTANCE + TOLERANCE && adjMatrix.GetIndex(i).GetIndex(j) >= DISTANCE - TOLERANCE )
					temp++;

			}
		}

		if (temp > mostNeighbor) {
			mostNeighbor = temp;
			mostNeighborIndex = i;
		}

	}


	return mostNeighborIndex;

}

void AddLongestNeighbor(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, int startingCity) {

	int currentCityIndex = startingCity;
	StaticVector<bool, CITY_COUNT> visited(false);
	StaticVector<int, CITY_COUNT> path;

	path.PushBack(currentCityIndex);
	visited.SetIndex(currentCityIndex, true);


	while (true) {

		int temp = BFS(adjMatrix, currentCityIndex, visited);

		if (temp == -1)
			break;

		currentCityIndex = temp;
		path.PushBack(temp);
		visited.SetIndex(temp, true);

	}


	std::cout << "Final path is: " << std::endl;
	//std::cout << path;
	std::cout << "\n Total of " << path.GetSize() << " cities are traversed" << std::endl;

}

void RunTests() {

	RunStaticVectorTests();
	RunLinkedListTests();
}

int main() {
	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> adjMatrix;
	
	adjMatrix.GetIndex(0).SetIndex(0, 0);
	adjMatrix.GetIndex(0).SetIndex(1, 200);
	adjMatrix.GetIndex(0).SetIndex(2, 0);
	adjMatrix.GetIndex(0).SetIndex(3, 200);
	adjMatrix.GetIndex(0).SetIndex(4, 200);
	adjMatrix.GetIndex(0).SetIndex(5, 0);
	adjMatrix.GetIndex(0).SetIndex(6, 0);

	adjMatrix.GetIndex(1).SetIndex(0, 200);
	adjMatrix.GetIndex(1).SetIndex(1, 0);
	adjMatrix.GetIndex(1).SetIndex(2, 200);
	adjMatrix.GetIndex(1).SetIndex(3, 0);
	adjMatrix.GetIndex(1).SetIndex(4, 0);
	adjMatrix.GetIndex(1).SetIndex(5, 200);
	adjMatrix.GetIndex(1).SetIndex(6, 0);

	adjMatrix.GetIndex(2).SetIndex(0, 0);
	adjMatrix.GetIndex(2).SetIndex(1, 200);
	adjMatrix.GetIndex(2).SetIndex(2, 0);
	adjMatrix.GetIndex(2).SetIndex(3, 0);
	adjMatrix.GetIndex(2).SetIndex(4, 0);
	adjMatrix.GetIndex(2).SetIndex(5, 0);
	adjMatrix.GetIndex(2).SetIndex(6, 0);

	adjMatrix.GetIndex(3).SetIndex(0, 200);
	adjMatrix.GetIndex(3).SetIndex(1, 0);
	adjMatrix.GetIndex(3).SetIndex(2, 0);
	adjMatrix.GetIndex(3).SetIndex(3, 0);
	adjMatrix.GetIndex(3).SetIndex(4, 0);
	adjMatrix.GetIndex(3).SetIndex(5, 0);
	adjMatrix.GetIndex(3).SetIndex(6, 200);

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
   
	adjMatrix.GetIndex(6).SetIndex(0, 0);
	adjMatrix.GetIndex(6).SetIndex(1, 0);
	adjMatrix.GetIndex(6).SetIndex(2, 0);
	adjMatrix.GetIndex(6).SetIndex(3, 200);
	adjMatrix.GetIndex(6).SetIndex(4, 0);
	adjMatrix.GetIndex(6).SetIndex(5, 0);
	adjMatrix.GetIndex(6).SetIndex(6, 0);
	

	
	//RunTests();


	StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> cityDistances;
	StaticVector<std::string, CITY_COUNT> cityNames;

	readCSVFile(cityDistances, cityNames);

	//FindMaximumPath(adjMatrix, START);
	FindMaximumPath(cityDistances, START, FirstOrderNeighborScore);


	FindMaximumPath(cityDistances, START, SecondOrderNeighborScore);

	//std::cout << "Adding the longest Neighbor" << std::endl;
		//AddLongestNeighbor(adjMatrix, 5);



	//std::cout << "Adding the most Neighbor" << std::endl;

	//for (int i = 0; i < CITY_COUNT; i++)
		//MostNeighbourAlgorithm(cityDistances, i);



	//std::cout << "Adding the nearest Neighbor" << std::endl;

	//for(int i = 0; i < CITY_COUNT; i++)
		//NearestNeighborAlgorithm(cityDistances, i);


	//GeneticAlgorithmUtil<int, CITY_COUNT>(adjMatrix);
	
	//FindNumberOfCities(adjMatrix, 0, 5);

	//BFSSearchLongest(cityDistances);


	//std::cout << "Classic DFS algorithm" << std::endl;
	//findMaxConnectedVertices(adjMatrix);  // Output should be 3

	return 0;
}
