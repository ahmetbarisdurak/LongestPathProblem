#pragma once

#include <iostream>
#include <LinkedListLibrary.h>
#include <StaticVectorLibrary.h>

#define CITY_COUNT 81
#define DISTANCE 250
#define TOLERANCE 50

using ScoreFunction = int (*)(int, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>&, StaticVector<bool, CITY_COUNT>&);

// How many neighbors does the node have
int FirstOrderNeighborScore(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, StaticVector<bool, CITY_COUNT>& visited) {
	int tempScore = 0;

	for (int i = 0; i < CITY_COUNT; ++i) {
		int distance = adjMatrix.GetIndex(node).GetIndex(i);
		if (distance <= DISTANCE + TOLERANCE && distance >= DISTANCE - TOLERANCE && !visited.GetIndex(i))
			tempScore++;
	}

	return tempScore;
}

int SecondOrderNeighborScore(int node, StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT>& adjMatrix, StaticVector<bool, CITY_COUNT>& visited) {

	int tempScore = 0;
	for (int i = 0; i < CITY_COUNT; ++i) {
		int distance = adjMatrix.GetIndex(node).GetIndex(i);
		if (distance <= DISTANCE + TOLERANCE && distance >= DISTANCE - TOLERANCE && !visited.GetIndex(i)) {
			tempScore += FirstOrderNeighborScore(i, adjMatrix, visited);
		}
	}

	return tempScore;

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

		for (int i = 0; i < CITY_COUNT; i++) {
			int distance = adjMatrix.GetIndex(currentIndex).GetIndex(i);
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