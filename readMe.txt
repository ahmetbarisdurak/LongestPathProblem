
# Genetic Algorithm for Longest Path Problem

## Project Scenario

We are looking for the longest journey starting from city A, where at each step, we will travel X km (with a tolerance of +- Y) without visiting any city more than once.

X and Y can be taken as input from the console or read from a file.

The output can be displayed in the console or written to a file. We want to know how many cities were visited and the sequence of cities visited.


## Introduction
I used genetic algorithm in first approach to this problem, but It didn't work as I expected.
This code provides a Genetic Algorithm (GA) solution to the Longest Path Problem. 
The LP involves finding the longest possible path that visits a set of cities exactly once.
The code uses a genetic algorithm approach to find an approximate solution to this NP-hard problem.

## Features

- **Genetic Algorithm**: The core of the code is a genetic algorithm that evolves a population of potential solutions (individual paths) over several generations to find an optimal or near-optimal solution.

- **Population Initialization**: The code initializes a population of random paths (individuals) for the Longest Path.

- **Crossover**: The genetic algorithm uses crossover (mating) to create new paths by combining genes (cities) from two parent paths.

- **Mutation**: Mutation introduces genetic diversity by randomly swapping two cities in a path. Also mutation can add a random city to path

- **Fitness Function**: The fitness of a path is determined by its total number of cities that are visited, and the algorithm seeks to maximize this number.

- **Termination Criteria**: The algorithm runs for a specified number of generations or until a stopping criterion is met.

- **Output**: The code outputs the best path found, its total number of cities, and writes the path to a text file.

## Usage

1. **Input Data**: The code reads city distances and names from a CSV file named `ilmesafe.csv`. Make sure to provide this file with the correct format and data.

2. **Configuration**: You can customize various parameters in the code, such as population size, distance, and tolerance thresholds, and the number of generations.

3. **Running the Algorithm**: Run the code, and it will execute the genetic algorithm to find a solution to the Longest Path Problem.

4. **Output**: The code will output the best path found, its total number of cities, and create a text file with the path details in a folder named "results."

5. **Validation**: You can use the `CheckPath` function to validate the correctness of the generated path.

## Dependencies

The code depends on two custom libraries: `LinkedListLibrary` and `StaticVectorLibrary`. I wrote this libraries for real time system

## Important Notes

- The code currently uses a simple fitness function based on distance, and it may not always find the optimal solution. You can explore more sophisticated fitness functions and genetic operators to improve performance.

- The genetic algorithm parameters (population size, mutation rate, etc.) can be tuned for better results on specific problem instances.

