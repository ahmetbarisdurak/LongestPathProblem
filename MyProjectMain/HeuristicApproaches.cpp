#pragma once

#include <iostream>
#include <LinkedListLibrary.h>
#include <StaticVectorLibrary.h>
#include <queue>
#define CITY_COUNT 81
#define DISTANCE 250
#define TOLERANCE 50

using ScoreFunction = int (*)(int, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>&, StaticVector<bool, CITY_COUNT>&);
using DoubleScoreFunction = double (*)(int, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>&, StaticVector<bool, CITY_COUNT>&);

StaticVector<double, CITY_COUNT> closenessCentrality(0.0);

class ClosenessCentrality {
public:
	double ClosenessCentralityScore(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph, bool visited[CITY_COUNT]);
private:
	int* ComputeClosenessCentrality(int start, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& graph);
};




// How many neighbors does the node have
int FirstOrderNeighborScore(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix) {
	int tempScore = 0;

	for (int i = 0; i < CITY_COUNT; ++i) {
		int distance = adjMatrix[node][i];
		if (distance <= DISTANCE + TOLERANCE && distance >= DISTANCE - TOLERANCE)
			tempScore++;
	}

	return tempScore;
}

// Doesn't count the visited cities
int FirstOrderNeighborScore(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, StaticVector<bool, CITY_COUNT>& visited) {
	int tempScore = 0;

	for (int i = 0; i < CITY_COUNT; ++i) {
		int distance = adjMatrix[node][i];
		if (distance <= DISTANCE + TOLERANCE && distance >= DISTANCE - TOLERANCE && !visited.GetIndex(i))
			tempScore++;
	}

	return tempScore;
}


int SecondOrderNeighborScore(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, StaticVector<bool, CITY_COUNT>& visited) {

	int tempScore = 0;
	for (int i = 0; i < CITY_COUNT; ++i) {
		int distance = adjMatrix[node][i];
		if (distance <= DISTANCE + TOLERANCE && distance >= DISTANCE - TOLERANCE && !visited.GetIndex(i)) {
			tempScore += FirstOrderNeighborScore(i, adjMatrix, visited);
		}
	}

	return tempScore;

}

/*
double ComputeClosenessCentrality(int sourceCity, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, StaticVector<bool, CITY_COUNT>& visited) {
	//StaticVector<bool, CITY_COUNT> visited(false);
	StaticVector<int, CITY_COUNT> distance(0);

	visited[sourceCity] = true;
	distance[sourceCity] = 0;

	LinkedList<int, CITY_COUNT> queue;
	queue.PushBack(sourceCity);

	while (queue.GetSize() != 0) {
		int currentCity = queue.Front();
		queue.Erase(queue.GetIterator());

		if (!visited[currentCity]) {
			for (int neighborCity = 0; neighborCity < CITY_COUNT; ++neighborCity) {
				int cityDistance = adjMatrix[currentCity][neighborCity];
				if (cityDistance <= DISTANCE + TOLERANCE && cityDistance >= DISTANCE - TOLERANCE && !visited[neighborCity]) {
					distance[neighborCity] = distance[currentCity] + 1;
					queue.PushBack(neighborCity);
				}
			}
			visited[currentCity] = true;
		}
	}

	// Calculate closeness centrality score for the source city
	double closeness = 0.0;
	for (int i = 0; i < CITY_COUNT; ++i) {
		closeness += distance[i]; // sum up distances
		
	}

	return 1.0 / closeness;
}

void ClosenessCentrality(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, StaticVector<bool, CITY_COUNT>& visited) {
	for (int sourceCity = 0; sourceCity < CITY_COUNT; ++sourceCity) {
		//closenessCentrality[sourceCity] = ComputeClosenessCentrality(sourceCity, adjMatrix, visited);
	}
}
*/
double ClosenessCentrality::ClosenessCentralityScore(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, bool visited[CITY_COUNT]) {
	int* dists = ComputeClosenessCentrality(node, adjMatrix); int sum = 0;
	for (int i = 0; i < CITY_COUNT; ++i) sum += dists[i];

	return 1.0 / sum;
}
/*
int* ClosenessCentrality::ComputeClosenessCentrality(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix) {
	LinkedList<int, CITY_COUNT> queue;
	queue.PushBack(node);
	bool visited[CITY_COUNT];
	static int dists[CITY_COUNT];

	for (int i = 0; i < CITY_COUNT; ++i) { visited[i] = false; dists[i] = 0; };

	while (queue.GetSize() != 0) {
		int currentCity = queue.Front();

		queue.Erase(queue.GetIterator());


		
		if (!visited[currentCity]) {
			for (int neighborCity = 0; neighborCity < CITY_COUNT; ++neighborCity) {
				if (adjMatrix[currentCity][neighborCity] && !visited[neighborCity]) {
					dists[neighborCity] = dists[currentCity] + 1;
					queue.PushBack(neighborCity);
				}
			}
			visited[currentCity] = true;
		}
	}

	return dists;
}
*/
int* ClosenessCentrality::ComputeClosenessCentrality(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix) {
	std::queue<int> queue;
	queue.push(node);
	bool visited[CITY_COUNT];
	static int dists[CITY_COUNT];

	for (int i = 0; i < CITY_COUNT; ++i) { visited[i] = false; dists[i] = 0; };

	while (!queue.empty()) {
		int currentCity = queue.front();
		queue.pop();

		if (!visited[currentCity]) {
			for (int neighborCity = 0; neighborCity < CITY_COUNT; ++neighborCity) {
				if (adjMatrix[currentCity][neighborCity] && !visited[neighborCity]) {
					dists[neighborCity] = dists[currentCity] + 1;
					queue.push(neighborCity);
				}
			}
			visited[currentCity] = true;
		}
	}

	return dists;
}


void FindMaximumPath(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, int startingCity, ScoreFunction scoringFunction) {
	StaticVector<bool, CITY_COUNT> visited(false);
	StaticVector<int, CITY_COUNT> maxPath;

	int currentIndex = startingCity;

	maxPath.PushBack(currentIndex);
	visited.SetIndex(currentIndex, true);

	while (true) {
		int highestScore = -1;
		int highestScoreIndex = -1;

		for (int i = 0; i < CITY_COUNT; ++i) {
			int distance = adjMatrix[currentIndex][i];
			int tempScore = -1;
			if (distance <= DISTANCE + TOLERANCE && distance >= DISTANCE - TOLERANCE && !visited.GetIndex(i))
				tempScore = scoringFunction(i, adjMatrix, visited);

			if (tempScore > highestScore) {
				highestScore = tempScore;
				highestScoreIndex = i;
			}
		}

		if (highestScoreIndex == -1)
			break;
		
		maxPath.PushBack(highestScoreIndex);
		visited.SetIndex(highestScoreIndex, true);
		currentIndex = highestScoreIndex;
	}

	std::cout << maxPath.GetSize() << " number of cities are traveled" << std::endl;
	std::cout << "Printing path for these cities" << std::endl;
	std::cout << maxPath;
}

int CalculateTotalScore(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix) {


	return 1 * FirstOrderNeighborScore(node, adjMatrix); //+ 0 * //ClosenessCentralityScore(node, adjMatrix);
}

int Compare(int node1, int node2, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix) {

	int node1Score = FirstOrderNeighborScore(node1, adjMatrix);// +ClosenessCentralityScore(node1, adjMatrix);

	int node2Score = FirstOrderNeighborScore(node2, adjMatrix);// +ClosenessCentralityScore(node2, adjMatrix);

	if (node1Score > node2Score)
		return 1;
	else
		return 0;

}

void CalculateTotalScoreAndSort(StaticVector<int, CITY_COUNT>& sortedCities, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix) {

	int i, j;
	int key;
	for (i = 1; i < CITY_COUNT; i++) {
		key = sortedCities[i];
		j = i - 1;

		while (j >= 0 && Compare(key, sortedCities[j], adjMatrix)) {
			sortedCities[j + 1] = sortedCities[j];
			j = j - 1;
		}
		sortedCities[j + 1] = key;
	}

}

StaticVector<int, CITY_COUNT> longestPath;

void findLongestPath(int currentNode, int endNode, StaticVector<int, CITY_COUNT>& currentPath, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix,
	StaticVector<bool, CITY_COUNT>& visited) {
	
	visited[currentNode] = true;
	currentPath.PushBack(currentNode);

	if (currentNode == endNode) {
		if (currentPath.GetSize() > longestPath.GetSize()) {
			longestPath = currentPath;
		}
	}
	else {
		for (int neighbor = 0; neighbor < CITY_COUNT; ++neighbor) {
			if (adjMatrix[currentNode][neighbor] >= DISTANCE - TOLERANCE && adjMatrix[currentNode][neighbor] <= DISTANCE + TOLERANCE && !visited[neighbor]) {
				findLongestPath(neighbor, endNode, currentPath, adjMatrix, visited);
			}
		}
	}

	visited[currentNode] = false;
	currentPath.PopBack();
}


void FindMaximumPathTotalScore(int startingCity, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix) {
	StaticVector<bool, CITY_COUNT> visited(false);
	StaticVector<int, CITY_COUNT> sortedCities;

	for (int i = 0; i < CITY_COUNT; ++i)
		sortedCities.PushBack(i);

	CalculateTotalScoreAndSort(sortedCities, adjMatrix);

	StaticVector<int, CITY_COUNT> foundPath;
	int currentNode = startingCity;
	visited[startingCity] = true;
	foundPath.PushBack(startingCity);

	for (int i = 0; i < CITY_COUNT; ++i) {
		longestPath = StaticVector<int, CITY_COUNT>();
		StaticVector<int, CITY_COUNT> currentPath;
		
		findLongestPath(currentNode, sortedCities[i], currentPath, adjMatrix, visited);
		
		if (longestPath.GetSize() == 0)
			continue;

		visited = StaticVector<bool, CITY_COUNT>(false);

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




}

void FindMaximumPathCentrality(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, int startingCity, DoubleScoreFunction scoringFunction) {
	StaticVector<bool, CITY_COUNT> visited(false);
	StaticVector<int, CITY_COUNT> maxPath;

	int currentIndex = startingCity;

	maxPath.PushBack(currentIndex);
	visited[currentIndex] = true;

	while (true) {
		double highestScore = -1;
		int highestScoreIndex = -1;

		for (int i = 0; i < CITY_COUNT; ++i) {
			int distance = adjMatrix[currentIndex][i];
			double tempScore = -1;
			if (distance <= DISTANCE + TOLERANCE && distance >= DISTANCE - TOLERANCE && !visited.GetIndex(i))
				//tempScore = scoringFunction(i, adjMatrix, visited);

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
}

int FindMaximumPathCentrality1(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, bool visited[CITY_COUNT], int startingCity, int prevCity, ClosenessCentrality& cc) {
	
	if (prevCity != -1) {
		for (int i = 0; i < CITY_COUNT; ++i) adjMatrix[prevCity][i] = 0;
		for (int i = 0; i < CITY_COUNT; ++i) adjMatrix[i][prevCity] = 0;
	}
	
	visited[startingCity] = true;

	double highestScore = 0;
	int highestScoreIndex = -1;

	for (int i = 0; i < CITY_COUNT; ++i) {
		if (adjMatrix[startingCity][i]) {
			double cur = cc.ClosenessCentralityScore(i, adjMatrix, visited);
			std::cout << "CURRENT IS " << i + 1 << " AND SCORE IS " << cur << std::endl;
			if (cur > highestScore) { highestScore = cur; highestScoreIndex = i; }
		}

	}

	if (highestScoreIndex != -1) {
		std::cout << "Visited city: " << highestScoreIndex + 1 << " Distance from " << startingCity + 1 << " :" << adjMatrix[startingCity][highestScoreIndex] << std::endl;
		return 1 + FindMaximumPathCentrality1(adjMatrix, visited, highestScoreIndex, startingCity, cc);

	}
	else
		return 1;
}