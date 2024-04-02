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

#define ITERATIONS 500

char *FileName = NULL;
int improvFlag = 0; // 0 for first, 1 for best
int permutFlag = 0; // 0 for exchange, 1 for transpose, 2 for insert
int initFlag = 0; // 0 for random, 1 for CW

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
  int cost, newCost, temp, firstRandomPosition, secondRandomPosition;

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
  printf("Data have been read from instance file. Size of instance = %ld.\n\n", PSize);

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

  /* Create an initial random solution. 
     The only constraint is that it should always be a permutation */
  createRandomSolution(currentSolution);

  /* Print solution */
  printf("Initial solution:\n");
  printSolution(currentSolution);

  /* Compute cost of solution and print it */
  cost = computeCost(currentSolution);
  printf("Cost of this initial solution: %d\n\n", cost);

  /* Example: apply an exchange operation of two elements at random position
  firstRandomPosition = randInt(0,PSize-1);
  // Ensure second position is different from first one:
  secondRandomPosition = firstRandomPosition + randInt(1,(PSize-2));
  if (secondRandomPosition >= PSize)
    secondRandomPosition -= PSize;

  printf("Two positions exchanged: %d and %d. ", firstRandomPosition, secondRandomPosition);

  temp = currentSolution[firstRandomPosition];
  currentSolution[firstRandomPosition] = currentSolution[secondRandomPosition];
  currentSolution[secondRandomPosition] = temp;

  // Print again the solution after the exchange
  printf("Solution after exchange:\n");
  print_solution(currentSolution);
  */

  //Copy the solution into newSol
  long int *newSol = (long int *)malloc(PSize * sizeof(long int));
  for (int i = 0; i < PSize; i++){
    newSol[i] = currentSolution[i];
  }

  for (int i = 1; i <= ITERATIONS; i++){ //Loop the number of iterations
    printf("Iteration %d\n", i);

    newCost = computeCost(newSol);
    // Use of first improvement algorithm
    if (improvFlag == 0)
      firstImprovement(currentSolution, newSol, newCost, permutFlag);
    else
      bestImprovement(currentSolution, newSol);
    
  }
 

  /* Recompute cost of solution */
  /* There are some more efficient way to do this, instead of recomputing everything... */
  newCost = computeCost(currentSolution);
  printf("Cost of the solution after applying the algo: %d\n", newCost);

  if (newCost == cost)
    printf("Second solution is as good as first one\n");
  else if (newCost > cost)
    printf("Second solution is better than first one\n");
  else
    printf("Second solution is worse than first one\n");

  printf("Time elapsed since we started the timer: %g\n\n", elapsed_time(VIRTUAL));

  /* Free memory */
  free(currentSolution);
  free(newSol);

  return 0;
}
