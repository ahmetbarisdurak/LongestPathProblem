#pragma once

#include <iostream>
#include <Algorithms.h>
#include <LinkedListLibrary.h>
#include <StaticVectorLibrary.h>

#define CITY_COUNT 81
#define DISTANCE 250
#define TOLERANCE 50

StaticVector<int, CITY_COUNT> longestPath;
StaticVector<int, CITY_COUNT> foundPath;
StaticVector<int, CITY_COUNT> cityTimes(0);

// DFS that returns the path order
LinkedList<int, CITY_COUNT> dfs(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, bool visited[CITY_COUNT], int currentVertex) {
	visited[currentVertex] = true;
	LinkedList<int, CITY_COUNT> longestPath;

	for (int neighbor = 0; neighbor < CITY_COUNT; ++neighbor) {
		if (adjMatrix[currentVertex][neighbor] >= (DISTANCE - TOLERANCE) && adjMatrix[currentVertex][neighbor] <= (DISTANCE + TOLERANCE) && !visited[neighbor]) {

			LinkedList<int, CITY_COUNT> neighborPath = dfs(adjMatrix, visited, neighbor);

			if (neighborPath.GetSize() > longestPath.GetSize()) { // finding the max componenet size and it's visiting order


				LinkedListIterator<int, CITY_COUNT> longestPathIterator = longestPath.GetIterator();

				while (longestPathIterator.HasNext()) {
					int temp = longestPathIterator.Next();
					visited[temp] = false;
				}

				longestPath.Clear();

				LinkedListIterator<int, CITY_COUNT> llIterator = neighborPath.GetIterator();

				while (llIterator.HasNext()) {
					int temp = llIterator.Next();
					visited[temp] = true;
					longestPath.PushBack(temp);

				}
			}
		}
	}
	longestPath.Insert(longestPath.GetIterator(), currentVertex); // Insertin the current vertex to start of the path

	return longestPath;
}

// trying to find max connected vertices using dfs but doesn't work correctly
void findMaxConnectedVertices(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix) {

	StaticVector<bool, CITY_COUNT> visited(false);
	LinkedList<int, CITY_COUNT> visitOrder;

	int startVertex = 80; // city plate number


	for (startVertex = 0; startVertex < CITY_COUNT; ++startVertex) {

		bool visited[CITY_COUNT] = { false };
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

// Tries to brute force and find longest path using dfs and returns the list, doesn't work because of visited array
LinkedList<int, CITY_COUNT> DFSLongestPath(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, bool visited[CITY_COUNT], int currentVertex, int targetVertex, int& maxDepth) {
	visited[currentVertex] = true; // Set index as visited
	LinkedList<int, CITY_COUNT> longestPath;

	for (int neighbor = 0; neighbor < CITY_COUNT; ++neighbor) {
		if (adjMatrix[currentVertex][neighbor] >= (DISTANCE - TOLERANCE) && adjMatrix[currentVertex][neighbor] <= (DISTANCE + TOLERANCE) && !visited[neighbor]) {

			LinkedList<int, CITY_COUNT> neighborPath = dfs(adjMatrix, visited, neighbor);

			if (neighborPath.GetSize() > maxDepth && neighborPath.Back() == targetVertex) { // finding the max componenet size and it's visiting order

				longestPath.Clear();

				LinkedListIterator<int, CITY_COUNT> llIterator = neighborPath.GetIterator();

				while (llIterator.HasNext()) {
					int temp = llIterator.Next();
					longestPath.PushBack(temp);
				}

				maxDepth = neighborPath.GetSize();
			}
		}
	}
	longestPath.Insert(longestPath.GetIterator(), currentVertex); // Insertin the current vertex to start of the path

	return longestPath;
}

// Finding the nearest neighbor and traversing through that path
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

void BFSSearchLongest(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix) {
	//int t1, t2;

	// first bfs to find one end point of
	// longest path
	//t1 = BFS(adjMatrix, 22);

	//  second bfs to find actual longest path
	//t2 = BFS(adjMatrix, t1);

}

// Creating graph for better usability assigning 0 if there are no paths between cities assigning the length if there is a path between
void CreateGraph(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph) {
	for (int i = 0; i < CITY_COUNT; ++i) {
		for (int j = 0; j < CITY_COUNT; ++j) {
			if (graph[i][j] > DISTANCE + TOLERANCE || graph[i][j] < DISTANCE - TOLERANCE)
				graph[i][j] = 0;
		}
	}
}

// Compare two cities according to their score combination
int Compare(int node1, int node2, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph) {

	ClosenessCentrality<int, CITY_COUNT> closenessCentrality;
	FirstOrderNeighbors<int, CITY_COUNT> firstOrderNeighbors;
	SecondOrderNeighbors<int, CITY_COUNT> secondOrderNeighbors;
	ThirdOrderNeighbors<int, CITY_COUNT> thirdOrderNeighbors;

	bool visited[CITY_COUNT] = { false };

	//calculates score for node 1 and node 2 

	double node1Score = firstOrderNeighbors.Score(node1, graph, visited) + secondOrderNeighbors.Score(node1, graph, visited) 
		+ closenessCentrality.Score(node1, graph, visited) + thirdOrderNeighbors.Score(node1, graph, visited);

	double node2Score = firstOrderNeighbors.Score(node2, graph, visited) + secondOrderNeighbors.Score(node2, graph, visited) 
		+ closenessCentrality.Score(node2, graph, visited) + thirdOrderNeighbors.Score(node2, graph, visited);

	if (node1Score > node2Score)
		return 1;
	else
		return 0;

}

// Sorting the cities according to their combination scores
void CalculateTotalScoreAndSort(StaticVector<int, CITY_COUNT>& sortedCities, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph) {
	// Uses insertion sort to sort cities
	int i, j;
	int key;
	for (i = 1; i < CITY_COUNT; i++) {
		key = sortedCities[i];
		j = i - 1;

		while (j >= 0 && Compare(key, sortedCities[j], graph)) {
			sortedCities[j + 1] = sortedCities[j];
			j = j - 1;
		}
		sortedCities[j + 1] = key;
	}
}

// finds the longest path between two given nodes using dfs but it takes long so i have to manually stop it
bool LongestPath(int currentNode, int endNode, StaticVector<int, CITY_COUNT>& currentPath, StaticVector<StaticVector<int, CITY_COUNT>, 
	CITY_COUNT>& graph, bool visited[CITY_COUNT]) {
	
	visited[currentNode] = true;
	currentPath.PushBack(currentNode);
	cityTimes[currentNode]++;

	if (cityTimes[currentNode] > 10000000) { // If we traverse through a city more than 10000000 algorithm stops. 
		visited[currentNode] = false;
		currentPath.PopBack();
		return true;
	}

	if (currentNode == endNode) { // If path encounter with endNode it checks whether the new path is longer than old path
		if (currentPath.GetSize() > longestPath.GetSize()) { // then if new path is longer than longest path it assigns new path as longest path
			longestPath = currentPath;
			std::cout << "Longest path is " << std::endl << longestPath;
		}
	}
	else {
		for (int neighbor = 0; neighbor < CITY_COUNT; ++neighbor) {
			
			if (!visited[neighbor] && graph[currentNode][neighbor] && cityTimes[neighbor] <= 10000000) {
				if (LongestPath(neighbor, endNode, currentPath, graph, visited)) // If current node is not encountered with end node it continues to search for it from different paths
					return true;
			}
		}
	}

	visited[currentNode] = false;
	currentPath.PopBack();
	return false; // If path can't find a solution it backtracks to the first place that has neighbor
}

void FindMaximumPathTotalScore(int startingCity, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph) {
	bool visited[CITY_COUNT] = { false };
	StaticVector<int, CITY_COUNT> sortedCities;
	bool sortedVisited[CITY_COUNT] = { false };

	for (int i = 0; i < CITY_COUNT; ++i)
		sortedCities.PushBack(i);

	CalculateTotalScoreAndSort(sortedCities, graph); // sorts the cities according to their scores
	
	StaticVector<int, CITY_COUNT> foundPath;
	int currentNode = startingCity;

	// Traversing in sorted cities list for one time to create path
	// Start from the starting city and traverse through the vector and add them to your path.
	for(int i = 0; i < CITY_COUNT; ++i) {
		StaticVector<int, CITY_COUNT> currentPath;
		longestPath = StaticVector<int, CITY_COUNT>();
		

		if (visited[currentNode] || visited[sortedCities[i]]) { // checks if the current node is visited or not
			std::cout << "This node is already visited" << std::endl;
			continue;
		}

		visited[currentNode] = true;

		if (!LongestPath(currentNode, sortedCities[i], currentPath, graph, visited)) {
			std::cout << "No path between " << currentNode << "  " << sortedCities[i] << std::endl;
			continue;
		}

		visited[longestPath[0]] = true;

		for (int j = 0; j < longestPath.GetSize(); ++j) { // if LongestPath function returns true it takes the longestPath and assigns to foundPath
			foundPath.PushBack(longestPath[j]);
			visited[longestPath[j]] = true;
		}
	
		
		foundPath.PopBack(); 
		currentNode = sortedCities[i];
		visited[currentNode] = false;
	}

	foundPath.PushBack(currentNode);

	std::cout << "Number of cities is " << foundPath.GetSize() << std::endl;
	std::cout << " Printing the path " << std::endl;
	std::cout << foundPath;


}

// Finds longest path using different algorithms
int FindLongestPathAlgorithms(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph, bool visited[CITY_COUNT], 
	int startingCity, int prevCity, Algorithms<int, CITY_COUNT>& algorithm) {
	
	if (prevCity != -1) { // Blocking the previous city paths, not to visit again
		for (int i = 0; i < CITY_COUNT; ++i) graph[prevCity][i] = 0;
		for (int i = 0; i < CITY_COUNT; ++i) graph[i][prevCity] = 0;
	}
	
	visited[startingCity] = true;

	double highestScore = 0;
	int highestScoreIndex = -1;

	for (int i = 0; i < CITY_COUNT; ++i) {
		if (graph[startingCity][i] && !visited[i]) {
			double tempScore = algorithm.Score(i, graph, visited); // finds score for the neighbors
			if (tempScore > highestScore) { // selects the neighbor with the highest score
				highestScore = tempScore;
				highestScoreIndex = i; 
			}
		}

	}

	if (highestScoreIndex != -1) { // If there is a highest score found then it pushes to found path then recursively continue with the neighbor
		//foundPath.PushBack(highestScoreIndex);
		return 1 + FindLongestPathAlgorithms(graph, visited, highestScoreIndex, startingCity, algorithm);
	}
	
	return 1;
}

// Finds the longest path using combination of algoritmhs
int FindLongestPathCombination(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph, bool visited[CITY_COUNT],
	int startingCity, int prevCity, Algorithms<int, CITY_COUNT>** algorithms) {

	if (prevCity != -1) { // Blocking the previous city paths not to visit again
		for (int i = 0; i < CITY_COUNT; ++i) 
			graph[prevCity][i] = 0;
		for (int i = 0; i < CITY_COUNT; ++i) 
			graph[i][prevCity] = 0;
	}

	visited[startingCity] = true;

	double highestScore = 0;
	int highestScoreIndex = -1;

	for (int i = 0; i < CITY_COUNT; ++i){
		if (graph[startingCity][i]) {
			double tempScore = -1 * algorithms[0]->Score(i, graph, visited) + 1 * algorithms[1]->Score(i, graph, visited)  // finds score combination for the neighbors
				+ 1 * algorithms[2]->Score(i, graph, visited) + 1 * algorithms[3]->Score(i, graph, visited);
			
			if (tempScore > highestScore) { // selects the neighbor with the highest score
				highestScore = tempScore;
				highestScoreIndex = i; 
			}
		}

	}

	if (highestScoreIndex != -1) { // If there is a highest score found then it pushes to found path then recursively continue with the neighbor
		foundPath.PushBack(highestScoreIndex);
		return 1 + FindLongestPathCombination(graph, visited, highestScoreIndex, startingCity, algorithms);

	}
		
	return 1;
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
			std::cout << "These two are not neighbors (" << path[i] << ", " << path[i + 1] << ")" << std::endl;
			return false;
		}
	}

}