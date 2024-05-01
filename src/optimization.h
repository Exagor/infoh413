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
#ifndef _LO_H_
#define _LO_H_

extern long int **CostMat;

long long int computeCost ( long int *s );
long long int computeCostFast ( long int *s, int i, int j );
void createRandomSolution(long int *s);
void createCWSolution(long int *s);
int firstImprovement(long int * sol, long int * newsol, int cost, int permutFlag);
int bestImprovement(long int * sol, long int * newsol, int cost, int permutFlag);
int VND1(long int * sol, long int * newsol, int cost, int descent);
int VND2(long int * sol, long int * newsol, int cost, int descent);
int localSearch(long int* currentSolution);
void generateInitPop(long int **pop,int* costPop, int popSize);
void crossover(long int *parent1, long int *parent2, long int *child1, long int *child2);
void mutation(long int *sol);
void selectBest(long int **pop, int popSize, long int *bestSol);

#endif
