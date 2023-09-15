# Heuristic Approaches for Longest Path Problem

## Project Scenario

We are looking for the longest journey starting from city A, where at each step, we will travel X km (with a tolerance of +- Y) without visiting any city more than once.

X and Y can be taken as input from the console or read from a file.

The output can be displayed in the console or written to a file. We want to know how many cities were visited and the sequence of cities visited.


## Introduction
I used combination of some heuristic approaches after using genetic algorithm.
.
The code uses a combination of heuristic approaches to find an approximate solution to this NP-hard problem.



## Solution Method Overview:
The code focused on finding the longest path within a graph representing cities and their distances. The goal is to traverse cities while satisfying certain distance and tolerance constraints to find the longest possible path.

Here's a high-level overview of the solution method:

Reading City Data: The code reads data about cities and their distances from a CSV file named "ilmesafe.csv" and stores it in a matrix (cityDistances) and an array (cityNames).

Creating the Graph: The code then creates a graph representation based on the city distances, filtering out connections that don't meet certain distance and tolerance criteria.

Algorithms for Finding Longest Path:

The code defines several algorithms, such as finding first-order neighbors, second-order neighbors, third-order neighbors, closeness centrality, and possibly more (these are encapsulated within classes).
Each algorithm calculates a "score" for each city, and these scores are used to determine the order in which cities are visited to find the longest path.

## Finding Longest Path:

The code attempts to find the longest path using each of the defined algorithms separately.
It records the length of the longest path and the order in which cities are visited for each algorithm.
The results are printed to the console.

These are the parameters that are used:

First Order Neighbours
The score of selected city is equal to the number of its neighbors. O(n^3)

Second Order Neighbours
The score of selected city is equal to the number of neighbors of that city's neighbor with the most neighbors.
Doesn't count visited ones. O(n^4)

Third Order Neighbours
The score of a city is equal to the number of neighbor's neighbors of that city's neighbor with the most neighbors.
O(n^5)

Closeness Centrality
The score of city is proportional to it's location in the Turkiye map. If it is in border of Turkiye
or inside of Turkiye.
O(n^4)

Betweenness Centrality:
The score of city is proportional to it's location in the Turkiye map. If it is between some 
cities or not.
O(n^5)

## Combining Algorithms for Longest Path:

The code combines the results of multiple algorithms to determine the longest path. This is achieved by assigning weights to the algorithms.
The path found using this combination of algorithms is also printed to the console.
Writing Results to Files: The code writes the found paths and other relevant information to text files for further analysis.

To combine these functions I used some weights. Which is [-1,1,1,1]

First one which is first neighbor score is multiplied by -1 because eliminating the nodes with fewer cities
in the first place helps traversing in a lot of cities.

But in the second and third order neighbors score multiplying with -1 prevented path to go cities
that are in the border, so I used them with weight 1.

And I used closeness centrality as my main parameter which helps to visit the cities that are in border of Turkiye.
These cities are hard to visit so closeness centrality helps to visit them.

I didn't include betweennes centrality since it's running time was too much.

For 250 km distance and 50 km tolerance I found 76 cities that can be travelled If I start from Ankara.
## Path Validation:

The code validates the found paths to ensure they meet certain criteria, such as not having duplicate cities and satisfying distance constraints.
Output and Correct Path Count: The code prints information about the correct paths found and their count.

Key Functions:
FindLongestPathAlgorithms: This function tries to find the longest path using a specified algorithm. It is called separately for different algorithms.

FindLongestPathCombination: This function combines the results of multiple algorithms to find the longest path.

CreateGraph: This function filters out edges in the graph that do not meet distance and tolerance criteria.

CheckPath: This function validates whether a found path is correct based on certain criteria.

WriteToFile: This function writes the found paths to text files.

FindMinimumDistanceTolerance: This function iterates over different distance and tolerance values to find an optimal combination that maximizes the longest path's length.

readCSVFile: Reads city data from a CSV file and populates the cityDistances and cityNames data structures.

Other functions are related to DFS (Depth-First Search) for finding paths within the graph and other graph-related operations.