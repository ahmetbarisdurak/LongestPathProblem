#pragma once

#include <iostream>
#include <LinkedListLibrary.h>
#include <StaticVectorLibrary.h>
#include <time.h>

using namespace std;

#define CITY_COUNT 7 // 81
#define START 0
#define DISTANCE 200
#define TOLERANCE 50


#define POPULATION_SIZE 15 // ? 100


// Genetic algorithm parameters
const double mutationRate = 0.1;
const int generations = 100;

//StaticVector<StaticVector<int, CITY_COUNT>, CITY_COUNT> cityDistances;

template <class T, unsigned int N>
struct IndividualPath {
    StaticVector<T, N> gnome;
    int fitnessScore = 0;

    IndividualPath& operator=(const IndividualPath& other) {
        if (this != &other) {
            gnome = other.gnome;
            fitnessScore = other.fitnessScore;
        }
        return *this;
    }
    template <class U>
    bool operator==(const IndividualPath<U, N>& other) const {
        return (gnome == other.gnome) && (fitnessScore == other.fitnessScore);
    }


};


/*
int fitness(const StaticVector<int, CITY_COUNT>& solution) {
    int numCitiesVisited = 0;
    StaticVector<bool, CITY_COUNT> visited(false);

    for (int city : solution) {
        if (!visited[city]) {
            visited[city] = true;
            numCitiesVisited++;
        }
    }

    return numCitiesVisited;
}
*/

// Function to return the fitness value of a gnome.
// The fitness value is the path length
// of the path represented by the GNOME.
template <class T, unsigned int N>
int CalculateFitness(StaticVector<T, N> gnome, StaticVector<StaticVector<T, N>, N>& adjMatrix) {
    int score = 0;
    for (int i = 0; i < gnome.GetSize() - 1; i++) {
        if (adjMatrix.GetIndex(gnome.GetIndex(i)).GetIndex(gnome.GetIndex(i + 1)) <= DISTANCE + TOLERANCE
            && adjMatrix.GetIndex(gnome.GetIndex(i)).GetIndex(gnome.GetIndex(i + 1)) >= DISTANCE - TOLERANCE)
            score += 1;
        else
            return -1;
        //score += 1; //adjMatrix.GetIndex(gnome.GetIndex(i)).GetIndex(gnome.GetIndex(i + 1));
    }
    return score;
}

// Function to return a random number from cities besides the starting point
int RandNum() {
    int num = CITY_COUNT;
    int rnum = 0;

    while (true) {
        rnum = rand() % num;
        
        if (rnum != START)
            break;
    }

    return rnum;
}

int RandNum(int start, int end)
{
    int r = end - start;
    int rnum = start + rand() % r;
    return rnum;
}

// Function to check if the character
// has already occurred in the string
template <typename T, unsigned int N>
bool Repeat(StaticVector<T,N>& gnome, T temp) {

    for (int i = 0; i < gnome.GetSize(); i++) {
        if (gnome.GetIndex(i) == temp) // comparator gerekebilir
            return true;
    }
    return false;
}

template <class T, unsigned int N>
int FindMostNeighborIndex(int startingIndex, StaticVector<T, N> tempGnome, StaticVector<StaticVector<T, N>, N>& adjMatrix) {

    int mostNeighbor = 0;
    int mostNeighborIndex = -1;

    for (int i = 0; i < N; i++) {
        
        int temp = 0;
        if (!Repeat(tempGnome, i) && adjMatrix.GetIndex(startingIndex).GetIndex(i) <= DISTANCE + TOLERANCE && adjMatrix.GetIndex(startingIndex).GetIndex(i) >= DISTANCE - TOLERANCE) {
            for (int j = 0; j < N; j++) {
                if (adjMatrix.GetIndex(i).GetIndex(j) <= DISTANCE + TOLERANCE && adjMatrix.GetIndex(i).GetIndex(j) >= DISTANCE - TOLERANCE)
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


template <class T, unsigned int N>
StaticVector<T, N> CreateGnome(StaticVector<StaticVector<T, N>, N>& adjMatrix) {
    int gnomeSize = RandNum(2, N);

    // Creating gnome using the neighbor's neighbor method
    // Selecting the neighbor which has the most neighbors
    StaticVector<T, N> tempGnome;
    tempGnome.PushBack(START);

    for (int i = 0; i < gnomeSize; i++) {
        int mostNeighborIndex = FindMostNeighborIndex(tempGnome.GetIndex(i), tempGnome, adjMatrix);

        if (mostNeighborIndex == -1) {
            // add a random neighbor which is not in gnome
            while (true) {

                T randomElement = RandNum(0, N);
                if (!Repeat(tempGnome, randomElement)) {
                    tempGnome.PushBack(randomElement);
                    break;
                }
            }

        }
        else
            tempGnome.PushBack(mostNeighborIndex);

    }

    return tempGnome;
}

// Function to return a mutated GNOME
// Mutated GNOME is a string
// with a random interchange
// of two genes to create variation in species
template <class T, unsigned int N>
StaticVector<T, N> MutatedGene(StaticVector<T, N> gnome)
{
    if (gnome.GetSize() <= 2) // If gnome is empty, 1 char or 2 char don't change anyting "", "0", "02" doesn't changes
        return gnome;

    while (true) {
        int r = RandNum(1, gnome.GetSize()); // Don't change the starting vertex
        int r1 = RandNum(1, gnome.GetSize());

        if (r1 != r && r1 != START && r != START) {
            T temp = gnome.GetIndex(r);
            gnome.SetIndex(r, gnome.GetIndex(r1));
            gnome.SetIndex(r1, temp);
            break;
        }
    }
    return gnome;
}

template <class T>
T RandomGene() {
    return RandNum(0, CITY_COUNT);
}

template <class T, unsigned int N>
StaticVector<T, N> Mate(IndividualPath<T, N> parent1, IndividualPath<T, N> parent2) {

    StaticVector<T, N> childGnome;
    int childGnomeSize = (parent1.gnome.GetSize() + parent2.gnome.GetSize()) / 2; // Getting mean of parent1 and parent2
    int i = 0;
    float p = 0;
    childGnome.PushBack(parent1.gnome.GetIndex(0)); // Start should be same
    i++;
    while(i < childGnomeSize) {
        
        p = float(RandNum(0, 100)) / 100;
        T temp;
        // if prob is less than 0.45, insert gene
        // from parent 1 
        if (p < 0.35) {

            if (parent1.gnome.GetSize() <= i) {
                temp = RandomGene<T>();
                if (!Repeat(childGnome, temp)) {
                    childGnome.PushBack(temp);
                    i++;
                }
            }
            else {
                temp = parent1.gnome.GetIndex(i);
                if (!Repeat(childGnome, temp)) {
                    childGnome.PushBack(temp);
                    i++;
                }
            }
        }
        // if prob is between 0.45 and 0.90, insert
        // gene from parent 2
        else if (p < 0.60) {
            if (parent2.gnome.GetSize() <= i) {
                temp = RandomGene<T>();
                if (!Repeat(childGnome, temp)) {
                    childGnome.PushBack(temp);
                    i++;
                }
            }
            else {
                temp = parent2.gnome.GetIndex(i);
                if (!Repeat(childGnome, temp)) {
                    childGnome.PushBack(temp);
                    i++;
                }
            }
        }
        else {
            temp = RandomGene<T>();
            if (!Repeat(childGnome, temp)) {
                childGnome.PushBack(temp);
                i++;
            }
        }
        // otherwise insert random gene(mutate), 
        // for maintaining diversity
    }

    return childGnome;
}



// Function to return the updated value
// of the cooling element.
int Cooldown(int temp) {
    return (90 * temp) / 100;
}


template< class T, unsigned int N>
bool ComparePathsByFitnessScore(const IndividualPath<T,N>& path1, const IndividualPath<T,N>& path2) {
    return path1.fitnessScore < path2.fitnessScore;
}

template <class T, unsigned int N>
StaticVector<T, N> GeneticAlgorithm(StaticVector<StaticVector<T, N>, N>& adjMatrix) {

    // Generation Number
    int gen = 1;
    // Number of Gene Iterations
    int genThreshold = 100;
    
    srand(time(NULL));

    StaticVector<struct IndividualPath<T, N>, POPULATION_SIZE> population;
    struct IndividualPath<T, N> tempPath;
    StaticVector<T, N> tempGnome;

    cout << "Printing the gnomes and their fitness scores" << endl;
    cout << "Creating random gnomes for population" << endl;
    // Populating the GNOME pool.
    
    for (int i = 0; i < POPULATION_SIZE; i++) {
        //struct IndividualPath<T, N> tempPath;
        //StaticVector<T, N> tempGnome;
        tempGnome = CreateGnome<T, N>(adjMatrix);
        tempPath.gnome = tempGnome;
        //cout << "Gnome is: " << endl;
        //tempPath.gnome.PrintData();
        tempPath.fitnessScore = CalculateFitness(tempPath.gnome, adjMatrix);
        //cout << "Fitness score is: " << tempPath.fitnessScore << endl;
        population.PushBack(tempPath);
    }

    
    cout << "\nInitial population: " << endl
        << "GNOME     FITNESS VALUE\n";
    for (int i = 0; i < POPULATION_SIZE; i++) {
        cout << "Gnome is: " << endl;
        population.GetIndex(i).gnome.PrintData();
        cout << "Fitness score is: " << population.GetIndex(i).fitnessScore << endl;
    }
    
    
    bool found = false;
    int temperature = 300000; // ???

    // Iteration to perform
    // population crossing and gene mutation.
    while (temperature > 1000 && gen <= genThreshold) {
        population.Sort(ComparePathsByFitnessScore); // Sort population with fitness scores
        StaticVector<struct IndividualPath<T, N>, POPULATION_SIZE> newPopulation;

        //cout << "Starting to mutate genes" << endl;
        /*
        for (int i = 0; i < POPULATION_SIZE; i++) {
            struct IndividualPath<T, N> p1 = population.GetIndex(i);

            while (true) {
                StaticVector<T, N> newGnome = MutatedGene<T, N>(p1.gnome);
                struct IndividualPath<T, N> newPath; ///////////////////////////////////
                newPath.gnome = newGnome;
                newPath.fitnessScore = CalculateFitness<T, N>(newPath.gnome, adjMatrix);

                
                if (newPath.fitnessScore >= population.GetIndex(i).fitnessScore) {
                    newPopulation.PushBack(newPath);
                    break;
                }
                else {

                    // Accepting the rejected children at
                    // a possible probability above threshold.
                    float prob = pow(2.7,
                        -1 * ((float)(newPath.fitnessScore
                            - population.GetIndex(i).fitnessScore)
                            / temperature));
                    if (prob > 0.5) {
                        newPopulation.PushBack(newPath);
                        break;
                    }
                }
            }
        }
        */
        
        //cout << "Starting to crossover" << endl;

        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            int r = RandNum(0, POPULATION_SIZE);
            IndividualPath<T,N> parent1 = population.GetIndex(r);
            r = RandNum(0, POPULATION_SIZE);
            IndividualPath<T, N> parent2 = population.GetIndex(r);
            StaticVector<T, N> newGnome = Mate(parent1, parent2);

            struct IndividualPath<T, N> offspring;
            offspring.gnome = newGnome;
            //cout << "Calculating fitness score" << endl;
            offspring.fitnessScore = CalculateFitness<T, N>(offspring.gnome, adjMatrix);

            newPopulation.PushBack(offspring);
        }


        //temperature = Cooldown(temperature);
        //population = newPopulation;
        //cout << "Generation " << gen << " \n";
        //cout << "GNOME     FITNESS VALUE\n";

        for (int i = 0; i < POPULATION_SIZE; i++) {
            //cout << "Gnome is: ";
            //newPopulation.GetIndex(i).gnome.PrintData();
            //cout << " Fitness score is: " << newPopulation.GetIndex(i).fitnessScore << endl;
        }
        gen++;

        if (!(gen <= genThreshold)) {
            //cout << "--------------------" << endl;
            //cout << "Found the best one " << endl;
            //cout << "--------------------" << endl;

            newPopulation.Sort(ComparePathsByFitnessScore);

            //for (int i = 0; i < POPULATION_SIZE; i++) {
               // cout << "Gnome is: ";
                newPopulation.GetIndex(0).gnome.PrintData();
                cout << "Best of the best" << endl;
                cout << " Fitness score is: " << newPopulation.GetIndex(0).fitnessScore << endl;
            //}
        }
    }
    
    return NULL;

}

template< class T, unsigned int N>
int GeneticAlgorithmUtil(StaticVector<StaticVector<T, N>, N>& adjMatrix) {

    // Call the genetic algorithm
    //StaticVector<T, N> bestSolution = GeneticAlgorithm<T, N>(adjMatrix);
    //for(int i = 0; i < 10; i++)
        GeneticAlgorithm<T, N>(adjMatrix);

    // Print the best solution
    /*
    cout << "Best solution:";
    for (int i = 0; i < N; i++) {
        cout << " " << bestSolution.GetIndex(i);
    }
    cout << endl;
    */
    return 0;
}
