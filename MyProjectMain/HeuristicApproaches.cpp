#pragma once

#include <iostream>
#include <LinkedListLibrary.h>
#include <StaticVectorLibrary.h>
#include <queue>
#define CITY_COUNT 81
#define DISTANCE 250
#define TOLERANCE 50
#define START 5

using ScoreFunction = int (*)(int, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>&, StaticVector<bool, CITY_COUNT>&);
using DoubleScoreFunction = double (*)(int, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>&, StaticVector<bool, CITY_COUNT>&);

StaticVector<double, CITY_COUNT> closenessCentrality(0.0);

class Algorithms {
public:
	virtual double Score(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph, bool visited[CITY_COUNT]) = 0;
};

class DistanceToStart : public Algorithms {
public:
	double Score(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph, bool visited[CITY_COUNT]);
};

class ClosenessCentrality : public Algorithms {
public:
	double Score(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph, bool visited[CITY_COUNT]);
private:
	int* ComputeClosenessCentrality(int start, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph);
};

class BetweennessCentrality : public Algorithms {
public:
	double Score(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph, bool visited[CITY_COUNT]);
private:
	int ComputeBetweennessCentrality(int node, int start, int end, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph);
};

class FirstOrderNeighbors : public Algorithms {
public:
	double Score(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph, bool visited[CITY_COUNT]);

};

class SecondOrderNeighbors : public Algorithms {
public:
	double Score(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph, bool visited[CITY_COUNT]);
private:
	int FirstOrderNeighborScore(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph);
};
double BetweennessCentrality::Score(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph, bool visited[CITY_COUNT]) {
	int total = 0, containing = 0;
	for (int i = 0; i < CITY_COUNT; ++i) {
		for (int j = i; j < CITY_COUNT; ++j) {
			if (i == j) continue;

			int s = ComputeBetweennessCentrality(node, i, j, graph);
			if (s == 1) containing += 1;
			if (s != -1) total += 1;
		}
	}
	return containing / (double)total;
}

int BetweennessCentrality::ComputeBetweennessCentrality(int node, int start, int end, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph) {
	LinkedList<int, CITY_COUNT> queue;
	queue.PushBack(start);
	bool visited[CITY_COUNT];
	int prevs[CITY_COUNT];

	for (int i = 0; i < CITY_COUNT; ++i) { visited[i] = false; prevs[i] = -1; };

	while (queue.GetSize() != 0) {
		int cur = queue.Front();
		queue.Erase(queue.GetIterator());

		if (cur == end) break;

		if (!visited[cur]) {
			for (int i = 0; i < CITY_COUNT; ++i) {
				if (graph[cur][i] && !visited[i]) {
					prevs[i] = cur;
					queue.PushBack(i);
				}
			}

			visited[cur] = true;
		}
	}

	if (prevs[end] != -1) {
		int cur = end;
		while (cur != start) {
			if (cur == node) {
				return 1;
			}

			cur = prevs[cur];
		}
		return 0;
	}
	else return -1;
}

double DistanceToStart::Score(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph, bool visited_[CITY_COUNT]) {
	
	return graph[START][node];
}

// Doesn't count the visited cities
double FirstOrderNeighbors::Score(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph, bool visited[CITY_COUNT]) {
	double tempScore = 0;

	for (int i = 0; i < CITY_COUNT; ++i) {
		if (graph[node][i] && !visited[i])
			tempScore++;
	}

	return tempScore;
}

// How many neighbors does the node have
int SecondOrderNeighbors::FirstOrderNeighborScore(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph) {
	int tempScore = 0;

	for (int i = 0; i < CITY_COUNT; ++i) {
		if (graph[node][i])
			tempScore++;
	}

	return tempScore;
}

double SecondOrderNeighbors::Score(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph, bool visited[CITY_COUNT]) {

	double tempScore = 0;
	for (int i = 0; i < CITY_COUNT; ++i) {
		if (graph[node][i] && !visited[i]) {
			tempScore += FirstOrderNeighborScore(i, graph);
		}
	}

	return tempScore;

}

double ClosenessCentrality::Score(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, bool visited[CITY_COUNT]) {
	int* distances = ComputeClosenessCentrality(node, adjMatrix); 
	int sum = 0;
	for (int i = 0; i < CITY_COUNT; ++i) 
		sum += distances[i];

	return 1.0 / sum;
}

int* ClosenessCentrality::ComputeClosenessCentrality(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph) {
	LinkedList<int, CITY_COUNT> queue;
	queue.PushBack(node);

	bool visited[CITY_COUNT];
	int distances[CITY_COUNT];

	for (int i = 0; i < CITY_COUNT; ++i) { 
		visited[i] = false; 
		distances[i] = 0;
	}

	while (queue.GetSize() != 0) {
		int currentCity = queue.Front();
		queue.Erase(queue.GetIterator());

		if (!visited[currentCity]) {
			for (int neighborCity = 0; neighborCity < CITY_COUNT; ++neighborCity) {
				if (graph[currentCity][neighborCity] && !visited[neighborCity]) {
					distances[neighborCity] = distances[currentCity] + 1;
					queue.PushBack(neighborCity);
				}
			}
			visited[currentCity] = true;
		}
	}

	return distances;
}

void FindMaximumPath(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph, int startingCity, Algorithms& algorithm) {
	bool visited[CITY_COUNT];
	StaticVector<int, CITY_COUNT> maxPath;

	int currentIndex = startingCity;

	for (int i = 0; i < CITY_COUNT; i++)
		visited[i] = false;


	maxPath.PushBack(currentIndex);
	visited[currentIndex] = true;

	while (true) {
		double highestScore = -1;
		int highestScoreIndex = -1;

		for (int i = 0; i < CITY_COUNT; ++i) {
			double tempScore = -1;
			if (graph[currentIndex][i] && !visited[i])
				tempScore = algorithm.Score(i, graph, visited);

			if (tempScore > highestScore) {
				highestScore = tempScore;
				highestScoreIndex = i;
			}
		}

		if (highestScoreIndex == -1)
			break;
		
		maxPath.PushBack(highestScoreIndex);
		visited[highestScoreIndex] = true;
		currentIndex = highestScoreIndex;
	}

	std::cout << maxPath.GetSize() << " number of cities are traveled" << std::endl;
	std::cout << "Printing path for these cities" << std::endl;
	std::cout << maxPath;
	std::cout << "-----------------------------------------------" << std::endl;
}
 
int Compare(int node1, int node2, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph) {

	ClosenessCentrality cc;
	BetweennessCentrality bc;
	DistanceToStart dc;
	FirstOrderNeighbors fon;
	SecondOrderNeighbors son;

	bool visited[CITY_COUNT];

	for (int i = 0; i < CITY_COUNT; ++i)
		visited[i] = false;

	double node1Score = fon.Score(node1, graph, visited) + son.Score(node1, graph, visited) + cc.Score(node1, graph, visited); // +bc.Score(node1, graph, visited);

	double node2Score = fon.Score(node2, graph, visited) + son.Score(node2, graph, visited) + cc.Score(node2, graph, visited); // +bc.Score(node2, graph, visited);

	if (node1Score > node2Score)
		return 1;
	else
		return 0;

}

// Sorting the cities according to their scores
void CalculateTotalScoreAndSort(StaticVector<int, CITY_COUNT>& sortedCities, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph) {

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

StaticVector<int, CITY_COUNT> longestPath;

bool findLongestPath(int currentNode, int endNode, StaticVector<int, CITY_COUNT>& currentPath, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph,
	bool visited[CITY_COUNT]) {
	
	visited[currentNode] = true;
	currentPath.PushBack(currentNode);

	if (currentNode == endNode) {
		if (currentPath.GetSize() > longestPath.GetSize()) {
			longestPath = currentPath;
		}

		//if (longestPath.GetSize() >= 63 || longestPath.GetSize() < 10 && longestPath.GetSize() > 1)
			//return true;
	}
	else {
		for (int neighbor = 0; neighbor < CITY_COUNT; ++neighbor) {
			if (!visited[neighbor] && graph[currentNode][neighbor]) {
				if (findLongestPath(neighbor, endNode, currentPath, graph, visited))
					return true;
					
			}
		}
	}

	visited[currentNode] = false;
	currentPath.PopBack();
	return false;
}



void FindMaximumPathTotalScore(int startingCity, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph) {
	bool visited[CITY_COUNT];
	StaticVector<int, CITY_COUNT> sortedCities;
	bool sortedVisited[CITY_COUNT];

	for (int i = 0; i < CITY_COUNT; ++i) {
		sortedCities.PushBack(i);
		visited[i] = false;
		sortedVisited[i] = false;
	}

	CalculateTotalScoreAndSort(sortedCities, graph);
	
	std::cout << "Sorted cities" << std::endl;
	std::cout << sortedCities;


	StaticVector<int, CITY_COUNT> foundPath;
	int currentNode = startingCity;

	// Traversing in sorted cities list for one time
	
	for(int i = 0; i < CITY_COUNT; ++i) {
		StaticVector<int, CITY_COUNT> currentPath;
		longestPath = StaticVector<int, CITY_COUNT>();

		std::cout << "Traversing through the list " << sortedCities[i] << std::endl;
		

		std::cout << longestPath;

		if (visited[currentNode]) {
			std::cout << "This node is already visited" << std::endl;
			continue;
		}

		if (visited[sortedCities[i]])
			continue;

		std::cout << sortedCities[i] << " is not in the path " << std::endl;

		visited[currentNode] = true;

		if (!findLongestPath(currentNode, sortedCities[i], currentPath, graph, visited)) {
			std::cout << "No path between " << currentNode << "  " << sortedCities[i] << std::endl;
			continue;
		}
		

		std::cout << "Therei s a path between " << currentNode << "  " << sortedCities[i] << std::endl;
		std::cout << currentPath;

		std::cout << "Longest path is ..... " << sortedCities[i] << std::endl;
		std::cout << longestPath;


		visited[longestPath[0]] = true;
		for (int j = 0; j < longestPath.GetSize(); ++j) {
			foundPath.PushBack(longestPath[j]);
			visited[longestPath[j]] = true;
		}
	
		
		foundPath.PopBack(); 
		currentNode = sortedCities[i];
		visited[currentNode] = false;

		std::cout << "FOUND PATH IS: " << std::endl;
		std::cout << foundPath;

	}

	foundPath.PushBack(currentNode);

	std::cout << "Number of cities is " << foundPath.GetSize() << std::endl;
	std::cout << " Printing the path " << std::endl;
	std::cout << foundPath;


	// Start from the starting city and traverse through the vector and add them to your path.




}

/*
void FindMaximumPathTotalScore(int startingCity, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph) {
	bool visited[CITY_COUNT];
	StaticVector<int, CITY_COUNT> sortedCities;

	for (int i = 0; i < CITY_COUNT; ++i) {
		sortedCities.PushBack(i);
		visited[i] = false;
	}

	CalculateTotalScoreAndSort(sortedCities, graph);

	std::cout << sortedCities;


	StaticVector<int, CITY_COUNT> foundPath;
	int currentNode = startingCity;
	visited[startingCity] = true;
	foundPath.PushBack(startingCity);

	// Traversing in sorted cities list for one time
	for (int i = 0; i < CITY_COUNT; ++i) {
		std::cout << "Traversing through the list " << i << std::endl;
		std::cout << "Adding current node: " << currentNode << std::endl;
		longestPath = StaticVector<int, CITY_COUNT>();
		StaticVector<int, CITY_COUNT> currentPath;

		if (!findLongestPath(currentNode, sortedCities[i], currentPath, graph, visited))
			continue;

		for(int i = 0; i < CITY_COUNT; i++)
			visited[i] = false;

		for (int j = 1; j < longestPath.GetSize(); ++j) {
			foundPath.PushBack(longestPath[j]);
			visited[longestPath[j]] = true;
		}

		currentNode = sortedCities[i];

	}

	std::cout << "Number of cities is " << foundPath.GetSize() << std::endl;
	std::cout << " Printing the path " << std::endl;
	std::cout << foundPath;


	// Start from the starting city and traverse through the vector and add them to your path.




}*/

int FindMaximumPathCentrality(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, bool visited[CITY_COUNT], int startingCity, int prevCity, Algorithms& algorithm) {
	
	if (prevCity != -1) {
		for (int i = 0; i < CITY_COUNT; ++i) adjMatrix[prevCity][i] = 0;
		for (int i = 0; i < CITY_COUNT; ++i) adjMatrix[i][prevCity] = 0;
	}
	
	visited[startingCity] = true;

	double highestScore = 0;
	int highestScoreIndex = -1;

	for (int i = 0; i < CITY_COUNT; ++i) {
		if (adjMatrix[startingCity][i]) {
			double cur = algorithm.Score(i, adjMatrix, visited);
			if (cur > highestScore) { highestScore = cur; highestScoreIndex = i; }
		}

	}

	if (highestScoreIndex != -1) {
		//std::cout << "Visited city: " << highestScoreIndex + 1 << " Distance from " << startingCity + 1 << " :" << adjMatrix[startingCity][highestScoreIndex] << std::endl;
		return 1 + FindMaximumPathCentrality(adjMatrix, visited, highestScoreIndex, startingCity, algorithm);

	}
	else
		return 1;
}