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
#include <string.h>

#include "utilities.h"
#include "instance.h"

long int Seed;


long int  **createMatrix (long int dim) {

  int k;
  long int **result = (long int **)calloc(dim,sizeof(long int *));

  for (k=0; k<dim; ++k) {
    result[k] = (long int *)calloc(dim, sizeof(long int));
  }

  return result;
}

void fatal (char *s) {
  fprintf (stderr, "%s\n", s);
  exit(1);
}

double ran01( long *idum ) {
/* 
      FUNCTION:      returns a pseudo-random number
      INPUT:         a pointer to the seed variable 
      OUTPUT:        a pseudo-random number uniformly distributed in [0,1]
      (SIDE)EFFECTS: changes the value of seed
*/
  long k;
  double ans;

  k =(*idum)/IQ;
  *idum = IA * (*idum - k * IQ) - IR * k;
  if (*idum < 0 ) *idum += IM;
  ans = AM * (*idum);
  return ans;
}


/* Return random integer in the (inclusive) range
 * [minimum, maximum]
 */
int randInt(int minimum, int maximum) {
  return ( (int)(ran01(&Seed)*(maximum - minimum + 1)) + minimum );
}  


long int * generate_random_vector(long int dim)
     /* Generates a random vector, quick and dirty */
{
   long int     *random_vector;
   int     i, help, node, tot_assigned = 0;
   double  rnd;

   random_vector = (long int *)malloc(dim * sizeof(long int));  

   if (!random_vector) {
       fatal("Error on random_vector malloc\n");
   }

   for ( i = 0 ; i < dim; i++) 
     random_vector[i] = i;

   for ( i = 0 ; i < dim ; i++ ) {
     /* find (randomly) an index for a free unit */ 
     rnd  = ran01 ( &Seed );
     node = (long int) (rnd  * (dim - tot_assigned)); 
     help = random_vector[i];
     random_vector[i] = random_vector[i+node];
     random_vector[i+node] = help;
     tot_assigned++;
   }

   return random_vector;

}
void exchange(long int* vector, int i, int j){
    /*Function to exchange the elements i and j of the vector
    No transpose function as transpose is just exchange with i=j+1 or j-1*/
    long int temp = vector[i];
    vector[i]=vector[j];
    vector[j]=temp;
}

void insert(long int* vector, int i, int j){
  /*Function to insert the element i in the position j of the vector*/
  long int temp = vector[i];
  if (i < j) {
    for (int k = i; k < j; k++){
      vector[k] = vector[k+1];
    }
  } else {
    for (int k = i; k > j; k--){
      vector[k] = vector[k-1];
    }
  }
  vector[j] = temp;
}

void statsToFile(char* FileName, int improvFlag, int permutFlag, int initFlag, int vndFlag, double timeTaken, int cost, int iterations){
  /*Function that sends stats to a file */
  FILE *file;

  file = fopen("raw_results/results.csv", "a");
  if (file == NULL) {
    fprintf(stderr, "Error opening file\n");
    return;
  }

  const char* exactFileName = strrchr(FileName, '/');
  if (exactFileName != NULL) {
    exactFileName++; // Move past the '/'
  } else {
    exactFileName = FileName; // No '/' found, use the whole FileName
  }

  //Format the file
  const char *improvStr = (improvFlag == 0) ? "first" : "best";
  const char *permutStr;
  switch(permutFlag) {
    case 0:
      permutStr = "exchange";
      break;
    case 1:
      permutStr = "transpose";
      break;
    case 2:
      permutStr = "insert";
      break;
    default:
      permutStr = "unknown";
  }
  const char *initStr = (initFlag == 0) ? "random" : "CW";
  switch(vndFlag) {
    case 0:
      break;
    case 1:
      permutStr = "VND1";
      break;
    case 2:
      permutStr = "VND2";
      break;
    default:
      permutStr = "unknown";
  }
  //save the file
  fprintf(file,"%s,%ld,%s,%s,%s,%lf,%d,%i\n",exactFileName ,PSize, improvStr, permutStr, initStr, timeTaken, cost, iterations);
  printf("Results successfully written to file\n");
  fclose(file);
}

void statsToFile2(char* FileName, int algoFlag, double timeTaken, int cost, int iterations){
  /*Function that sends stats to a file */
  FILE *file;

  file = fopen("raw_results/results2.csv", "a");
  if (file == NULL) {
    fprintf(stderr, "Error opening file\n");
    return;
  }

  const char* exactFileName = strrchr(FileName, '/');
  if (exactFileName != NULL) {
    exactFileName++; // Move past the '/'
  } else {
    exactFileName = FileName; // No '/' found, use the whole FileName
  }

  //Format the file
  const char *algoStr = (algoFlag == 0) ? "memetic" : "ILS";
  //save the file
  fprintf(file,"%s,%ld,%s,%lf,%d,%i\n",exactFileName ,PSize, algoStr, timeTaken, cost, iterations);
  printf("Results successfully written to file\n");
  fclose(file);
}

void statForPlot(char* Filename, long int* statCost, double* timeStats, int nbGeneration){
  FILE *file;
  char filepath[256];

  const char* exactFileName = strrchr(Filename, '/');
  if (exactFileName != NULL) {
    exactFileName++; // Move past the '/'
  } else {
    exactFileName = Filename; // No '/' found, use the whole FileName
  }

  sprintf(filepath, "raw_results/runtimes/runtime_%s.csv", exactFileName);
  file = fopen(filepath, "a");
  if (file == NULL) {
    fprintf(stderr, "Error opening file\n");
    return;
  }
  fprintf(file,"Generation,Time,Cost\n");
  for(int i = 0; i < nbGeneration;i++){fprintf(file,"%d,%f,%ld\n",i,timeStats[i],statCost[i]);}
  printf("Run-time statistics successfully written to file\n");
  fclose(file);
}