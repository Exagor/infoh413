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
    return 0;
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
  int *costRow = malloc(PSize * sizeof(int));

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
  long int* reset = malloc(PSize * sizeof(long int));
  // memcpy(reset, sol, PSize * sizeof(long int));
  for (int l = 0; l < PSize; l++){ //Keep instance of initial solution
    reset[l] = sol[l];
  }
  for (int l = 0; l < PSize; l++){
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

