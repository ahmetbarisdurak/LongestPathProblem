#pragma once

#include <iostream>
#include <LinkedListLibrary.h>
#include <StaticVectorLibrary.h>

#define CITY_COUNT 81
#define DISTANCE 250
#define TOLERANCE 50

using ScoreFunction = int (*)(int, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>&, StaticVector<bool, CITY_COUNT>&);
using DoubleScoreFunction = double (*)(int, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>&, StaticVector<bool, CITY_COUNT>&);

StaticVector<double, CITY_COUNT> closenessCentrality(0.0);

// How many neighbors does the node have
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

double ComputeClosenessCentrality(int sourceCity, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix) {
	StaticVector<bool, CITY_COUNT> visited(false);
	StaticVector<int, CITY_COUNT> distance(0);

	visited[sourceCity] = true;
	distance[sourceCity] = 0;

	LinkedList<int, CITY_COUNT> queue;
	queue.PushBack(sourceCity);

	while (queue.GetSize() != 0) {
		int currentCity = queue.Front();
		queue.Erase(queue.GetIterator());

		for (int neighborCity = 0; neighborCity < CITY_COUNT; ++neighborCity) {
			int cityDistance = adjMatrix[currentCity][neighborCity];
			if (cityDistance <= DISTANCE + TOLERANCE && cityDistance >= DISTANCE - TOLERANCE && !visited[neighborCity]) {
				distance[neighborCity] = distance[currentCity] + 1;
				queue.PushBack(neighborCity);
				visited[neighborCity] = true;
			}
		}
	}

	// Calculate closeness centrality score for the source city
	double closeness = 0.0;
	for (int i = 0; i < CITY_COUNT; ++i) {
		if (i != sourceCity) {
			closeness += static_cast<double>(distance[i]); // sum up distances
		}
	}

	return 1 / closeness;
}

void ClosenessCentrality(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix) {
	for (int sourceCity = 0; sourceCity < CITY_COUNT; ++sourceCity) {
		closenessCentrality[sourceCity] = ComputeClosenessCentrality(sourceCity, adjMatrix);
	}
}

double ClosenessCentralityScore(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, StaticVector<bool, CITY_COUNT>& visited) {

	return closenessCentrality[node];
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

void FindMaximumPathCentrality(StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, int startingCity, DoubleScoreFunction scoringFunction) {
	StaticVector<bool, CITY_COUNT> visited(false);
	StaticVector<int, CITY_COUNT> maxPath;

	int currentIndex = startingCity;

	maxPath.PushBack(currentIndex);
	visited.SetIndex(currentIndex, true);

	while (true) {
		double highestScore = -1;
		int highestScoreIndex = -1;

		for (int i = 0; i < CITY_COUNT; ++i) {
			int distance = adjMatrix[currentIndex][i];
			double tempScore = -1;
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
		visited[highestScoreIndex] = true;
		currentIndex = highestScoreIndex;
	}

	std::cout << maxPath.GetSize() << " number of cities are traveled" << std::endl;
	std::cout << "Printing path for these cities" << std::endl;
	std::cout << maxPath;
}

int find_max_travel(int start, int prev, StaticVector<bool, CITY_COUNT>& visited, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, DoubleScoreFunction scoringFunction) {
	//filter unavailable paths
	if (prev != -1) {
		for (int i = 0; i < CITY_COUNT; ++i) adjMatrix[prev][i] = 0;
		for (int i = 0; i < CITY_COUNT; ++i) adjMatrix[i][prev] = 0;
	}

	visited[start] = true;
	double max = 0, maxi = -1;
	for (int i = 0; i < CITY_COUNT; ++i) {
		if (adjMatrix[start][i] >= DISTANCE - TOLERANCE && adjMatrix[start][i] <= DISTANCE + TOLERANCE) {
			double cur = scoringFunction(i, adjMatrix, visited);
			if (cur > max) { max = cur; maxi = i; }
		}
	}

	if (maxi != -1) {
		
		std::cout << "Visited city: " << maxi + 1 << " Distance from " << start + 1 << " :" << adjMatrix[start][maxi] << std::endl;
		return 1 + find_max_travel(maxi, start, visited, adjMatrix, scoringFunction);
	}
	else return 1;
}