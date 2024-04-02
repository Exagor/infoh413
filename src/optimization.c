/*  Heuristic Optimization assignment, 2015.
    Adapted by Jérémie Dubois-Lacoste from the ILSLOP implementation
    of Tommaso Schiavinotto:
    ---
    ILSLOP Iterated Lcaol Search Algorithm for Linear Ordering Problem
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

}

void firstImprovement(long int * sol, long int * newsol, int cost, int permutFlag){
  /*Return the solution with a better cost*/
  int newCost;
  if (permutFlag){
    //Generate all neighbour and evaluate the transpose
    for (int j = 0; j < PSize; j++){ //First element to transpose
      for (int k = j-1; k<=j+1; k++){ //Second element to transpose
        if (k > 0 && j != k && k < PSize){
          exchange(newsol, j, k); //Since transpose is an exchange
          //Compute the cost of the neighbour
          newCost = computeCost(newsol);
          if (newCost > cost){//If the first neighbour found is better, stop
            //Copy the new solution in the current solution
            for (int i = 0; i < PSize; i++){
              sol[i] = newsol[i];
            }
            return;
          }
        }
      }
    }
  }
  else if(!permutFlag){// Case exchange
    //Generate all neigbours and evaluate the exchange
    for (int j = 0; j < PSize; j++){ //First element to exchange
      for (int k = 0; k < PSize; k++){ //Second element to exchange
        if (j != k){
          exchange(newsol, j, k);
          //Compute the cost of the neighbour
          newCost = computeCost(newsol);
          if (newCost > cost){
            for (int i = 0; i < PSize; i++){
              sol[i] = newsol[i];
            }
            return;
          }
        }
      }
    }
  }
  else{// Case insert
    printf("lol\n");
  }
  
  return;
}


long int* bestImprovement(long int * sol, long int * newsol){
  //Enumerates all neighbours and chooses the best one


}

void exchange(long int* vector, int i, int j){
    /*Function to exchange the elements i and j of the vector
    No transpose function as transpose is just exchange with i=j+1*/
    long int temp = vector[i];
    vector[i]=vector[j];
    vector[j]=temp;
}

void insert(long int* vector, int i, int j){
    /*Function to insert the element i in the position j of the vector*/
    long int temp = vector[i];
    for (int k = i; k > j; k--){
        vector[k] = vector[k-1];
    }
    vector[j] = temp;
}

