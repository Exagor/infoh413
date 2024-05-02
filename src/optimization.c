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
#include <values.h>
#include <string.h>

#include "optimization.h" 
#include "instance.h"
#include "utilities.h"

#ifdef __MINGW32__
#include <float.h>
#define MAX_FLOAT FLT_MAX
#else
#define MAX_FLOAT MAXFLOAT
#endif


long int **CostMat;


long long int computeCost (long int *s ) {
    int h,k;
    long long int sum;
    
    /* Diagonal value are not considered */
    for (sum = 0, h = 0; h < PSize; h++ ) 
	for ( k = h + 1; k < PSize; k++ )
	    sum += CostMat[s[h]][s[k]];
    return(sum);
}

long long int computeCostFast(long int *s, int i, int j){
    // Use of properties to make it faster
    //Idea : take the two elements that are moved and compute the difference in cost of the two
    //Don't work
    long long int delta = 0;
    int row1 = 0, row2 = 0;
    for (int k = 0; k < PSize; k++){ //Calculate initial cost
        if (k > i){
            row1 += CostMat[s[i]][s[k]];
        }
        if (k > j){
            row2 += CostMat[s[j]][s[k]];
        }
    }
    //Calculate the difference in cost
    
    delta += CostMat[s[i]][s[j]] - CostMat[s[j]][s[i]];
    return delta;
}

void createRandomSolution(long int *s) {
    // The solution is a vector of permutation of the indexes of the problem
    int j; 
    long int *random;

    random = generate_random_vector(PSize);
    for ( j = 0 ; j < PSize ; j++ ) {
      s[j] = random[j];
    }
    free ( random );
}

void createCWSolution(long int *s){
  /*Search for the lines with the greatest impact on the sum and assemble the solution*/
  long int sum, maxSum;
  int maxIndex;
  int *costRow = (int*)malloc(PSize * sizeof(int));

  // Calculate the cost of each row and add them in costRow in the order
  for (int i = 0; i < PSize; i++) {
    sum = 0;
    for (int j = 0; j < PSize; j++) {
      sum += CostMat[i][j];
    }
    costRow[i] = sum;
  }

  // Add the row number from the biggest to smallest in s
  for(int rowNb = 0; rowNb < PSize; rowNb++){
    maxSum = costRow[0];
    maxIndex = 0;
    for (int i = 1; i < PSize; i++) {
      if (costRow[i] > maxSum) {
        maxSum = costRow[i];
        maxIndex = i;
      }
    }
    s[rowNb] = maxIndex;
    costRow[maxIndex] = -1;
  }

}

int firstImprovement(long int * sol, long int * newsol, int cost, int permutFlag){
  /*Return the solution with a better cost*/
  int newCost;
  for (int l = 0; l < PSize; l++){
    newsol[l] = sol[l];
  }
  if (permutFlag == 1){
    //Generate all neighbour and evaluate the transpose
    for (int j = 0; j < PSize; j=j+2){ //First element to transpose
      for (int k = j-1; k <= j+1; k++){ //Second element to transpose
        if (k > 0 && j != k && k < PSize){
          exchange(newsol, j, k); //Since transpose is an exchange
          //Compute the cost of the neighbour
          newCost = computeCost(newsol);
          if (newCost > cost){//If the first neighbour found is better, stop
            //Copy the new solution in the current solution
            // memcpy(sol, newsol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              sol[l] = newsol[l];
            }
            cost = newCost;
          }
          else{
            exchange(newsol, j, k);
          }

        }
      }
    }
  }
  else if(permutFlag == 0){// Case exchange
    //Generate all neigbours and evaluate the exchange
    for (int j = 0; j < PSize; j++){ //First element to exchange
      for (int k = PSize -1; k >= j; k--){ //Second element to exchange
        if (j != k){
          exchange(newsol, j, k);
          newCost = computeCost(newsol);
          if (newCost > cost){
            // memcpy(sol, newsol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              sol[l] = newsol[l];
            }
            cost = newCost;
          }
          else{
            exchange(newsol, j, k); //If the neighbour is not better, we cancel the exchange
          }
        }
      }
    }
  }
  else if (permutFlag==2){// Case insert
     //Generate all neigbours and evaluate the insert
    for (int j = 0; j < PSize; j++){ //First element to insert
      for (int k = 0; k < PSize; k++){ //Second element where to insert
        if (j != k){
          insert(newsol, j, k);
          newCost = computeCost(newsol);
          if (newCost > cost){
            // memcpy(sol, newsol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              sol[l] = newsol[l];
            }
            cost = newCost;
          }
          else{
            // memcpy(newsol, sol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              newsol[l] = sol[l];
            }
          }
        }
      }
    }
  }
  
  return cost;
}


int bestImprovement(long int * sol, long int * newsol, int cost, int permutFlag){
  //Enumerates all neighbours and chooses the best one
  int newCost, bestCost;
  bestCost = cost;
  long int* reset = (long int*) malloc(PSize * sizeof(long int));
  // memcpy(reset, sol, PSize * sizeof(long int));
  for (int l = 0; l < PSize; l++){ //Keep instance of initial solution
    reset[l] = sol[l];
  }
  for (int l = 0; l < PSize; l++){ //Initialize the new solution
    newsol[l] = sol[l];
  }

  if (permutFlag == 1){//Case transpose
    for (int j = 0; j < PSize; j=j+2){ //First element to transpose
      for (int k = j-1; k <= j+1; k++){ //Second element to transpose
        if (k > 0 && j != k && k < PSize){
          exchange(newsol, j, k);
          newCost = computeCost(newsol);
          if (newCost > bestCost){//If the neighbour is better, actualize the best cost
            //Copy the new solution in the current solution
            // memcpy(sol, newsol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              sol[l] = newsol[l];
            }
            bestCost = newCost;
          }
          exchange(newsol, j, k); //we reset anyway the solution
        }
      }
    }
  }
  else if(permutFlag == 0){// Case exchange
    //Generate all neigbours and evaluate the exchange
    for (int j = 0; j < PSize; j++){ //First element to exchange
      for (int k = j; k < PSize; k++){ //Second element to exchange
        if (j != k){
          exchange(newsol, j, k);
          newCost = computeCost(newsol);
          if (newCost > bestCost){
            // memcpy(sol, newsol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              sol[l] = newsol[l];
            }
            bestCost = newCost;
          }
          exchange(newsol, j, k);
        }
      }
    }
  }
  else if (permutFlag==2){// Case insert
     //Generate all neigbours and evaluate the insert
    for (int j = 0; j < PSize; j++){ //First element to insert
      for (int k = 0; k < PSize; k++){ //Second element where to insert
        if (j != k){
          insert(newsol, j, k);
          newCost = computeCost(newsol);
          if (newCost > bestCost){
            // memcpy(sol, newsol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              sol[l] = newsol[l];
            }
            bestCost = newCost;
          }
          // memcpy(newsol, sol, PSize * sizeof(long int));
          for (int l = 0; l < PSize; l++){
            newsol[l] = reset[l];
          }
        }
      }
    }
  }
  return bestCost;
}

int VND1(long int * sol, long int * newsol, int cost, int descent){
  int newCost = cost;
  if (descent == 0){//transpose
    for (int j = 0; j < PSize; j=j+2){
      for (int k = j-1; k <= j+1; k++){
        if (k > 0 && j != k && k < PSize){
          exchange(newsol, j, k); 
          newCost = computeCost(newsol);
          if (newCost > cost){
            //Copy the new solution in the current solution
            // memcpy(sol, newsol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              sol[l] = newsol[l];
            }
            cost = newCost;
          }
          else{
            exchange(newsol, j, k);
          }
        }
      }
    }
  }
  else if (descent==1){ //exchange
    for (int j = 0; j < PSize; j++){
      for (int k = PSize -1; k >= j; k--){
        if (j != k){
          exchange(newsol, j, k);
          newCost = computeCost(newsol);
          if (newCost > cost){
            // memcpy(sol, newsol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              sol[l] = newsol[l];
            }
            cost = newCost;
          }
          else{
            exchange(newsol, j, k); //If the neighbour is not better, we cancel the exchange
          }
        }
      }
    }
  }
  else if (descent==2){//insert
    for (int j = 0; j < PSize; j++){
      for (int k = 0; k < PSize; k++){
        if (j != k){
          insert(newsol, j, k);
          newCost = computeCost(newsol);
          if (newCost > cost){
            // memcpy(sol, newsol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              sol[l] = newsol[l];
            }
            cost = newCost;
          }
          else{
            // memcpy(newsol, sol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              newsol[l] = sol[l];
            }
          }
        }
      }
    }
  }
    
  return cost;
}

int VND2(long int * sol, long int * newsol, int cost, int descent){
  int newCost = cost;
  if (descent == 0){//transpose
    for (int j = 0; j < PSize; j=j+2){
      for (int k = j-1; k <= j+1; k++){
        if (k > 0 && j != k && k < PSize){
          exchange(newsol, j, k); 
          newCost = computeCost(newsol);
          if (newCost > cost){
            //Copy the new solution in the current solution
            // memcpy(sol, newsol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              sol[l] = newsol[l];
            }
            cost = newCost;
          }
          else{
            exchange(newsol, j, k);
          } 
        }
      }
    }
  }
  else if (descent==2){ //exchange
    for (int j = 0; j < PSize; j++){
      for (int k = PSize -1; k >= j; k--){
        if (j != k){
          exchange(newsol, j, k);
          newCost = computeCost(newsol);
          if (newCost > cost){
            // memcpy(sol, newsol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              sol[l] = newsol[l];
            }
            cost = newCost;
          }
          else{
            exchange(newsol, j, k); //If the neighbour is not better, we cancel the exchange
          }
        }
      }
    }
  }
  else if (descent==1){//insert
    for (int j = 0; j < PSize; j++){
      for (int k = 0; k < PSize; k++){
        if (j != k){
          
          insert(newsol, j, k);
          newCost = computeCost(newsol);
          if (newCost > cost){
            // memcpy(sol, newsol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              sol[l] = newsol[l];
            }
            cost = newCost;
          }
          else{
            // memcpy(newsol, sol, PSize * sizeof(long int));
            for (int l = 0; l < PSize; l++){
              newsol[l] = sol[l];
            }
          }
        }
      }
    }
  }
    
  return cost;
}

int localSearch(long int* currentSolution){
  // Local search algorithm based on the VND2
  long int* newSol = (long int*) malloc(PSize * sizeof(long int));
  for (int i = 0; i < PSize; i++){//copy the current solution in new solution
    newSol[i] = currentSolution[i];
  }
  int newCost = computeCost(currentSolution);
  int prevCost = newCost;
  for (int i = 1; i <= 500; i++){
      int descent = 0;
      while (descent < 3){
        newCost = VND2(currentSolution, newSol, newCost, descent);
        if (newCost > prevCost){
          descent = 0;
          prevCost = newCost;
        }
        else{
          descent++;
        }
      }
      if (newCost == prevCost){
          break;
        }
    }
  free(newSol);
  return newCost;
}

void generateInitPop(long int **pop, int* costPop, int popSize){
  for (int i = 0; i < popSize; i++){ 
    createRandomSolution(pop[i]);
    costPop[i] = localSearch(pop[i]);
  }
}


void crossover(long int **pop, int popNb, long int* offspring){
  int a,b,par,indB;
  //Reset offspringwith values to detect elements not filled
  for (int i = 0; i < PSize; i++){
    offspring[i] = -1;
  }
  printf("1\n");
  //Crossover operator (CX operator)
  int parent1=randInt(0, popNb-1);
  int parent2=0;
  while(parent1==parent2){
    parent2=randInt(0, popNb-1);
  }
  //Keep elements in common
  for (int i = 0; i < PSize; i++){
    if (pop[parent1][i] == pop[parent2][i]){
      offspring[i] = pop[parent1][i];
    }
  }
  printf("2\n");
  for (int i = 0; i < PSize; i++){
    if (offspring[i] == -1){
      par = randInt(0,1); //Select random parent
      if (par == 0){
        a=pop[parent1][i];
        b=pop[parent2][i];
        offspring[i] = a;
        indB = 0;
        while (indB < PSize){
          if (pop[parent1][indB] == b){
            offspring[indB] = b;
            break;
          }
          indB++;
        }
      }
      else{
        a=pop[parent2][i];
        b=pop[parent1][i];
        offspring[i] = a;
        indB = 0;
        while (indB < PSize){
          if (pop[parent1][indB] == b){
            offspring[indB] = b;
            break;
          }
          indB++;
        }
      }
    }
  } 
}

void mutation(long int ** pop, int popSize, int *costPop){
  //Mutation operator
  int ind = randInt(0, popSize-1); //choose random individual
  // Mutation by exchange
  int a = randInt(0, PSize-1);
  int b = randInt(0, PSize-1);
  exchange(pop[ind], a, b);
  costPop[ind] = localSearch(pop[ind]);
}

void selectBestPop(long int** pop, int* costPop, int popSize, long int** offsprings, int* costOff, int nbCrossover){
  //Select the best solution in the population and offsprings
  long int** newPop = (long int**) malloc(popSize * sizeof(long int*));
  for(int i = 0; i < popSize; i++){
    newPop[i] = (long int*) malloc(PSize * sizeof(long int));
  }

  // Sort the costPop array in ascending order
  for (int i = 0; i < popSize; i++) {
    int minIndex = i;
    for (int j = i + 1; j < popSize; j++) {
      if (costPop[j] < costPop[minIndex]) {
        minIndex = j;
      }
    }
    // Swap the current element with the minimum element
    int tempCost = costPop[i];
    costPop[i] = costPop[minIndex];
    costPop[minIndex] = tempCost;
    // Swap the corresponding individual in pop array
    long int* tempInd = pop[i];
    pop[i] = pop[minIndex];
    pop[minIndex] = tempInd;
  }
  printf("sort costPop\n");
  //Sort the costOff array in ascending order
  for (int i = 0; i < nbCrossover; i++) {
    int minIndex = i;
    for (int j = i + 1; j < nbCrossover; j++) {
      if (costOff[j] < costOff[minIndex]) {
        minIndex = j;
      }
    }
    // Swap the current element with the minimum element
    int tempCost = costOff[i];
    costOff[i] = costOff[minIndex];
    costOff[minIndex] = tempCost;
    // Swap the corresponding individual in offsprings array
    long int* tempInd = offsprings[i];
    offsprings[i] = offsprings[minIndex];
    offsprings[minIndex] = tempInd;
  }
  printf("sort costOff\n");
  // Add the best individuals from pop and offsprings to newPop
  for (int i = 0; i < popSize - nbCrossover; i++) { //Fill first with best pop
    for (int j = 0; j < PSize; j++){
      newPop[i][j] = pop[i][j];
    }
  }
  int j = 0;
  for (int i = 0; i < nbCrossover; i++) { //Add the best offsprings or pop
    if (costPop[popSize - nbCrossover + i] < costOff[j]){
      for(int k = 0; k < PSize; k++){
        newPop[popSize - nbCrossover + i][k] = offsprings[j][k];
      }
      j++;
      i--;
    }
    else{
      for(int k = 0; k < PSize; k++){
        newPop[popSize - nbCrossover + i][k] = pop[popSize - nbCrossover + i][k];
      }
    }
  }
  // Copy newPop back to pop array
  for (int i = 0; i < popSize; i++) {
    for (int j = 0; j < PSize; j++){
      pop[i][j] = newPop[i][j];
    }
  }
  // Update costPop array with the costs of the new individuals
  for (int i = 0; i < popSize; i++) {
    costPop[i] = localSearch(pop[i]);
  }
  //free
  for(int i = 0; i < popSize; i++) {
    free(newPop[i]);
  }
  free(newPop);
}

