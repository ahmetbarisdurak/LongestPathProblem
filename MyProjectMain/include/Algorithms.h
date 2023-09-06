#pragma once

#include<StaticVectorLibrary.h>
#include<LinkedListLibrary.h>

template <class T, unsigned int N>
class Algorithms {
public:
	virtual double Score(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]) = 0;
};

template <class T, unsigned int N>
class ClosenessCentrality : public Algorithms<T, N> {
public:
	double Score(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]);
private:
	void ComputeClosenessCentrality(T start, StaticVector<StaticVector<T, N>, N>& graph, int distances[N]);
};


template <class T, unsigned int N>
class BetweennessCentrality : public Algorithms<T, N> {
public:
	double Score(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]);
private:
	int ComputeBetweennessCentrality(T node, T start, T end, StaticVector<StaticVector<T, N>, N>& graph);
};

template <class T, unsigned int N>
class FirstOrderNeighbors : public Algorithms<T, N> {
public:
	double Score(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]);

};

template <class T, unsigned int N>
class SecondOrderNeighbors : public Algorithms<T, N> {
public:
	double Score(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]);
private:
	int FirstOrderNeighborScore(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]);
};

template <class T, unsigned int N>
class ThirdOrderNeighbors : public Algorithms<T, N> {
public:
	double Score(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]);
private:
	int FirstOrderNeighborScore(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]);
	int SecondOrderNeighborScore(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]);
};


template <class T, unsigned int N>
double BetweennessCentrality<T, N>::Score(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]) {
	int betweenScore = 0;
	int totalScore = 0;
	for (int i = 0; i < N; ++i) {
		for (int j = i; j < N; ++j) {
			if (i == j)
				continue;

			int s = ComputeBetweennessCentrality(node, i, j, graph);
			if (s == 1)
				betweenScore += 1;
			if (s != -1)
				totalScore += 1;
		}
	}

	if (totalScore == 0)
		return 0.0;

	return static_cast<double>(betweenScore) / totalScore;
}

template <class T, unsigned int N>
int BetweennessCentrality<T, N>::ComputeBetweennessCentrality(T node, T startNode, T endNode, StaticVector<StaticVector<T, N>, N>& graph) {
	LinkedList<int, N> queue;
	queue.PushBack(startNode);
	bool visited[N] = { false };
	int next[N] = { -1 };

	while (queue.GetSize() != 0) {
		int currentCity = queue.Front();
		queue.Erase(queue.GetIterator());

		if (currentCity == endNode)
			break;

		if (!visited[currentCity]) {
			for (int i = 0; i < N; ++i) {
				if (graph[currentCity][i] && !visited[i]) {
					queue.PushBack(i);
					next[currentCity] = i;
				}
			}

			visited[currentCity] = true;
		}
	}

	bool temp = false;

	for (int i = 0; i < N; i++) {
		if (next[i] == endNode)
			temp = true;
	}

	if (temp) { // start from end to start if there is a city in between
		int currentCity = startNode;
		while (currentCity != endNode) {
			if (currentCity == node)
				return 1;

			currentCity = next[currentCity];
		}

		return 0; // If node city is not between return 0
	}

	return 0;
}

// Find according to visited states of cities
template <class T, unsigned int N>
double FirstOrderNeighbors<T, N>::Score(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]) {
	double tempScore = 0;

	for (int i = 0; i < N; ++i) {
		if (graph[node][i] && !visited[i])
			tempScore++;
	}

	return 1.0 - 1.0 / tempScore;
}

// How many neighbors does the node have
template <class T, unsigned int N>
int SecondOrderNeighbors<T, N>::FirstOrderNeighborScore(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]) {
	int tempScore = 0;

	for (int i = 0; i < N; ++i) {
		if (graph[node][i] && !visited[i])
			tempScore++;
	}

	return tempScore;
}

template <class T, unsigned int N>
double SecondOrderNeighbors<T, N>::Score(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]) {

	if (visited[node])
		return 0.0;

	double highestScore = 0;
	int highestScoreIndex = -1;
	for (int i = 0; i < N; ++i) {
		if (graph[node][i] && !visited[i]) {
			int tempScore = FirstOrderNeighborScore(i, graph, visited);
			if (tempScore > highestScore) {
				highestScore = tempScore;
				highestScoreIndex = i;
			}
		}
	}

	return 1.0 - 1.0 / highestScore;

}

template <class T, unsigned int N>
int ThirdOrderNeighbors<T, N>::FirstOrderNeighborScore(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]) {
	int tempScore = 0;
	for (int i = 0; i < N; ++i) {
		if (graph[node][i] && !visited[i])
			tempScore++;
	}

	return tempScore;
}

template <class T, unsigned int N>
int ThirdOrderNeighbors<T, N>::SecondOrderNeighborScore(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]) {

	int highestScore = 0;
	int highestScoreIndex = -1;
	for (int i = 0; i < N; ++i) {
		if (graph[node][i]) {
			int tempScore = FirstOrderNeighborScore(i, graph, visited);
			if (tempScore > highestScore) {
				highestScore = tempScore;
				highestScoreIndex = i;
			}
		}
	}

	return highestScore;
}

template <class T, unsigned int N>
double ThirdOrderNeighbors<T, N>::Score(T node, StaticVector<StaticVector<T, N>, N>& graph, bool visited[N]) {
	if (visited[node])
		return 0.0;

	double highestScore = 0;
	int highestScoreIndex = -1;
	for (int i = 0; i < N; ++i) {
		if (graph[node][i] && !visited[i]) {
			int tempScore = SecondOrderNeighborScore(i, graph, visited);
			if (tempScore > highestScore) {
				highestScore = tempScore;
				highestScoreIndex = i;
			}
		}
	}

	return 1.0 - 1.0 / highestScore;
}

template <class T, unsigned int N>
double ClosenessCentrality<T, N>::Score(T node, StaticVector<StaticVector<T, N>, N>& adjMatrix, bool visited[N]) {
	int distances[N];

	ComputeClosenessCentrality(node, adjMatrix, distances);

	int sum = 0;
	for (int i = 0; i < N; ++i)
		sum += distances[i]; // Adding all distances to current city

	return 1.0 / sum;
}

template <class T, unsigned int N>
void ClosenessCentrality<T, N>::ComputeClosenessCentrality(T node, StaticVector<StaticVector<T, N>, N>& graph,
	int distances[N]) {
	LinkedList<int, N> queue;
	queue.PushBack(node);

	bool visited[N];

	for (int i = 0; i < N; ++i) {
		visited[i] = false;
		distances[i] = 0;
	}

	while (queue.GetSize() != 0) {
		int currentCity = queue.Front();
		queue.Erase(queue.GetIterator());

		if (!visited[currentCity]) {
			for (int neighborCity = 0; neighborCity < N; ++neighborCity) {
				if (graph[currentCity][neighborCity] && !visited[neighborCity]) {
					distances[neighborCity] = distances[currentCity] + 1;
					queue.PushBack(neighborCity); // adding neighbor to queue
				}
			}
			visited[currentCity] = true;
		}
	}
}

