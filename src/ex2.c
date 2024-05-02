/*  Heuristic Optimization assignment, 2015.
    Adapted by Jérémie Dubois-Lacoste from the ILSLOP implementation
    of Tommaso Schiavinotto:
    ---
    ILSLOP Iterated Local Search Algorithm for Linear Ordering Problem
    Copyright (C) 2004  Tommaso Schiavinotto (tommaso.schiavinotto@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "instance.h"
#include "utilities.h"
#include "timer.h"
#include "optimization.h"

char *FileName = NULL;
int improvFlag = 0; // 0 for first, 1 for best
int permutFlag = 0; // 0 for exchange, 1 for transpose, 2 for insert
int initFlag = 0; // 0 for random, 1 for CW
int algoFlag = 0; // 0 for memetic, 2 for simulated annealing
int nbGeneration = 0;
int MAXTIME = 100; // Max 240s normally, but for test 5s
int POPULATION = 25;
int nbCrossover = 12; //Number of offsprings
int nbMutation = 5; //Number of individuals to mutate nbMutation < POPULATION

void readOpts(int argc, char **argv) {
  /* Function that reads the options from the command line */
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0) {
      FileName = argv[i+1];
      i++; // Skip the next argument
    } else if (strcmp(argv[i], "--best") == 0) {
      improvFlag = 1;
      printf("Iterative improvement : Best selected\n");
    } else if (strcmp(argv[i], "--first") == 0) {
      improvFlag = 0;
      printf("Iterative improvement : First selected\n");
    } else if (strcmp(argv[i], "--meme") == 0) {
      algoFlag = 0;
      improvFlag = 0;
      initFlag = 0;
      printf("Memetic Algorithm selected\n");
    } else if (strcmp(argv[i], "--sa") == 0) {
      algoFlag = 1;
      improvFlag = 0;
      initFlag = 0;
      printf("Simulated annealing selected\n");
    } else if (strcmp(argv[i], "--exchange") == 0) {
      permutFlag = 0;
      printf("Permutation mode : Exchange selected\n");
    } else if (strcmp(argv[i], "--transpose") == 0) {
      permutFlag = 1;
      printf("Permutation mode : Transpose selected\n");
    } else if (strcmp(argv[i], "--insert") == 0) {
      permutFlag = 2;
      printf("Permutation mode : Insert selected\n");
    } else {
      fprintf(stderr, "Option %s not managed.\n", argv[i]);
    }
  }
  if ( !FileName ) {
    printf("No instance file provided (use -i <instance_name>). Exiting.\n");
    exit(1);
  }
}

void printSolution(long int *s){
  /*Function that print the solution in the standard output (permutation vector)*/
  int j;
  for (j=0; j < PSize; j++) 
    printf(" %ld", s[j]);
  printf("\n");
}

int main (int argc, char **argv) 
{
  long int i,j;
  int cost;
  long int *currentSolution;

  /* Do not buffer output */
  setbuf(stdout,NULL);
  setbuf(stderr,NULL);
  
  if (argc < 2) {
    printf("No instance file provided (use -i <instance_name>). Exiting.\n");
    exit(1);
  }
  
  /* Read parameters */
  readOpts(argc, argv);

  /* Read instance file */
  CostMat = readInstance(FileName);
  printf("Data have been read from instance file %s. Size of instance = %ld.\n\n", FileName, PSize);

  /* initialize random number generator, deterministically based on instance.
   * To do this we simply set the seed to the sum of elements in the matrix, so it is constant per-instance,
   but (most likely) varies between instances */
  Seed = (long int) 0;
    for (i=0; i < PSize; ++i)
      for (j=0; j < PSize; ++j)
        Seed += (long int) CostMat[i][j];
  printf("Seed used to initialize RNG: %ld.\n\n", Seed);
  
  /* starts time measurement */
  start_timers();

  printf("Computing ... \n");

  currentSolution = (long int *)malloc(PSize * sizeof(long int)); //Initialize current sol

  if (algoFlag==0){//memetic mode
    int maxCost = 0;
    int maxInd = 0;

    //allocate memory for 2D array stocking population
    long int **pop = (long int **)malloc(POPULATION * sizeof(long int *));
    for (int i = 0; i < POPULATION; i++){
      pop[i] = (long int *)malloc(PSize * sizeof(long int));
    }

    //Allocate memory for the costs and fill with 0's
    int *costPop = (int *)malloc(POPULATION * sizeof(int));
    for (int i = 0; i < POPULATION; i++){
      costPop[i] = 0;
    }

    //Allocate memory for the offsprings and costs
    long int **offsprings = (long int **)malloc(nbCrossover * sizeof(long int *));
    for (int i = 0; i < nbCrossover; i++){
      offsprings[i] = (long int *)malloc(PSize * sizeof(long int));
      // createRandomSolution(offsprings[i]);
    }
    int *costOff = (int *)malloc(nbCrossover * sizeof(int));
    for (int i = 0; i < nbCrossover; i++){
      costOff[i] = 0;
    }
    
    // Generate initial population
    generateInitPop(pop,costPop, POPULATION);
    printf("Initial population generated\n");
    //Repeat until timer is up
    while(elapsed_time(VIRTUAL) < MAXTIME){
      printf("Generation %d\n", nbGeneration);
      //Crossover
      for (int i = 0; i < nbCrossover; i++){
        crossover(pop, POPULATION, offsprings[i]); //Gives offspring
        printf("crossover terminated\n");
        printSolution(offsprings[i]);
        costOff[i]=localSearch(offsprings[i]);
      }
      //Mutation
      for(int i = 0; i < nbMutation; i++){
        mutation(pop,POPULATION, costPop);
      }

      //Select best
      selectBestPop(pop, costPop, POPULATION, offsprings, costOff, nbCrossover);

      //Update best solution
      //selectBest

      nbGeneration++;
    }

    for(int i = 1; i < POPULATION; i++) {//Find best solution
      if(costPop[i] > maxCost) {
          maxCost = costPop[i];
          maxInd = i;
      }
    }
    //Copy the best solution into currentSolution
    for (int i = 0; i < PSize; i++){
      currentSolution[i] = pop[maxInd][i];
    }
    cost = maxCost;
    printf("Max cost: %d, Index: %d\n", maxCost, maxInd);
    
    //free
    for(int i = 0; i < POPULATION; i++) {
      free(pop[i]);
    }
    free(pop);
    free(costPop);
    for(int i = 0; i < nbCrossover; i++) {
      free(offsprings[i]);
    }
    free(offsprings);
    free(costOff);
  }
  
  /* Recompute cost of solution */
  /* There are some more efficient way to do this, instead of recomputing everything... */
  printf("Cost of the solution after applying the algo: %d\n", cost);

  printf("Number of generations : %d\n", nbGeneration);
  double timeTaken = elapsed_time(VIRTUAL);
  printf("Time elapsed since we started the timer: %g\n\n", timeTaken);

  /* Save the results in a file*/
  statsToFile2(FileName, algoFlag, timeTaken, cost, nbGeneration);

  /* Free memory */
  free(currentSolution);

  return 0;
}
