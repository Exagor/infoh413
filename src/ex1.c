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

#define MAXITERATIONS 4000

char *FileName = NULL;
int improvFlag = 0; // 0 for first, 1 for best
int permutFlag = 0; // 0 for exchange, 1 for transpose, 2 for insert
int initFlag = 0; // 0 for random, 1 for CW
int vndFlag = 0; // 1 for VND1, 2 for VND2
int nbIterations = 0;

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
    } else if (strcmp(argv[i], "--vnd1") == 0) {
      vndFlag = 1;
      improvFlag = 0;
      initFlag = 1;
      printf("Variable neighbourhood descending 1 selected\n");
    } else if (strcmp(argv[i], "--vnd2") == 0) {
      vndFlag = 2;
      improvFlag = 0;
      initFlag = 1;
      printf("Variable neighbourhood descending 2 selected\n");
    } else if (strcmp(argv[i], "--exchange") == 0) {
      permutFlag = 0;
      printf("Permutation mode : Exchange selected\n");
    } else if (strcmp(argv[i], "--transpose") == 0) {
      permutFlag = 1;
      printf("Permutation mode : Transpose selected\n");
    } else if (strcmp(argv[i], "--insert") == 0) {
      permutFlag = 2;
      printf("Permutation mode : Insert selected\n");
    } else if (strcmp(argv[i], "--random") == 0) {
      initFlag = 0;
      printf("Initial solution : Random selected\n");
    } else if (strcmp(argv[i], "--cw") == 0) {
      initFlag = 1;
      printf("Initial solution : CW selected\n");
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
  long int *currentSolution;
  int cost, newCost;

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

  /* A solution is just a vector of int with the same size as the instance */
  currentSolution = (long int *)malloc(PSize * sizeof(long int));

  /* Create an initial solution. 
     The only constraint is that it should always be a permutation */
  if (initFlag == 0){
    createRandomSolution(currentSolution);
  }
  else if (initFlag == 1){
    createCWSolution(currentSolution);
  }

  /* Print solution */
  //printf("Initial solution:\n");
  //printSolution(currentSolution);

  /* Compute cost of solution and print it */
  cost = computeCost(currentSolution);
  printf("Cost of this initial solution: %d\n\n", cost);

  //Copy the solution into newSol
  long int *newSol = (long int *)malloc(PSize * sizeof(long int));
  for (int i = 0; i < PSize; i++){
    newSol[i] = currentSolution[i];
  }

  newCost = cost;
  int prevCost = cost;
  printf("Computing ... \n");

  if (vndFlag==0){//Normal iterative improvement mode
    for (int i = 1; i <= MAXITERATIONS; i++){ //Loop the number of iterations
      // if (i % 100 == 0)
        //printf("Iteration %d : %d\n", i, newCost);
      nbIterations++;

      // Use of first improvement algorithm
      if (improvFlag == 0){
        newCost = firstImprovement(currentSolution, newSol, newCost, permutFlag);
        if (newCost == prevCost){ //Stop condition
          break;
        }
      }
      else if (improvFlag == 1){
        newCost = bestImprovement(currentSolution, newSol, newCost, permutFlag);
        if (newCost == prevCost){
          break;
        }
      }

      prevCost = newCost; //Actualize the previous cost
    }
  }
  else{//VND mode
    for (int i = 1; i <= MAXITERATIONS; i++){
      int descent = 0;
      while (descent < 3){
        if (vndFlag == 1){
          newCost = VND1(currentSolution, newSol, newCost, descent);
        }
        else if (vndFlag == 2){
          newCost = VND2(currentSolution, newSol, newCost, descent);
        }
        if (newCost > prevCost){
          descent = 0;
          prevCost = newCost;
          nbIterations++;
        }
        else{
          descent++;
        }
      }
      if (newCost == prevCost){
          break;
        }
    }
  }
  /* Recompute cost of solution */
  /* There are some more efficient way to do this, instead of recomputing everything... */
  printf("Cost of the solution after applying the algo: %d\n", newCost);

  if (newCost == cost)
    printf("Second solution is as good as first one\n");
  else if (newCost > cost)
    printf("Second solution is better than first one\n");
  else
    printf("Second solution is worse than first one\n");

  if (nbIterations == MAXITERATIONS)
    printf("Maximum number of iterations reached (%d)\n",MAXITERATIONS);
  else
    printf("Number of iterations to obtain (local) maxima : %d\n", nbIterations);
  double timeTaken = elapsed_time(VIRTUAL);
  printf("Time elapsed since we started the timer: %g\n\n", timeTaken);

  /* Save the results in a file*/
  statsToFile(FileName, improvFlag, permutFlag, initFlag, vndFlag, timeTaken, newCost, nbIterations);
  /* Free memory */
  free(currentSolution);
  free(newSol);

  return 0;
}
