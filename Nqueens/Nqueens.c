/* nqueens.c: (c) Arnold Meijster (a.meijster@rug.nl) */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Nqueens.h"

#define MAXQ 100

#define FALSE 0
#define TRUE  1

#define ABS(a) ((a) < 0 ? (-(a)) : (a))

int nqueens;      /* number of queens: global variable */
int queens[MAXQ]; /* queen at (r,c) is represented by queens[r] == c */

void initializeRandomGenerator() {
	/* this routine initializes the random generator. You are not
	* supposed to understand this code. You can simply use it.
	*/
	time_t t;
	srand((unsigned)time(&t));
}

/* Generate an initial position.
* If flag == 0, then for each row, a queen is placed in the first column.
* If flag == 1, then for each row, a queen is placed in a random column.
*/
void initiateQueens(int flag) {
	int q;
	for (q = 0; q < nqueens; q++) {
		queens[q] = (flag == 0 ? 0 : random() % nqueens);
	}
}

/* returns TRUE if position (row0,column0) is in
* conflict with (row1,column1), otherwise FALSE.
*/
int inConflict(int row0, int column0, int row1, int column1) {
	if (row0 == row1) return TRUE; /* on same row, */
	if (column0 == column1) return TRUE; /* column, */
	if (ABS(row0 - row1) == ABS(column0 - column1)) return TRUE;/* diagonal */
	return FALSE; /* no conflict */
}

/* returns TRUE if position (row,col) is in
* conflict with any other queen on the board, otherwise FALSE.
*/
int inConflictWithAnotherQueen(int row, int col) {
	int queen;
	for (queen = 0; queen < nqueens; queen++) {
		if (inConflict(row, col, queen, queens[queen])) {
			if ((row != queen) || (col != queens[queen])) return TRUE;
		}
	}
	return FALSE;
}

/* print configuration on screen */
void printState() {
	int row, column;
	printf("\n");
	for (row = 0; row < nqueens; row++) {
		for (column = 0; column < nqueens; column++) {
			if (queens[row] != column) {
				printf(".");
			}
			else {
				if (inConflictWithAnotherQueen(row, column)) {
					printf("Q");
				}
				else {
					printf("q");
				}
			}
		}
		printf("\n");
	}
}

/* move queen on row q to specified column, i.e. to (q,column) */
void moveQueen(int queen, int column) {
	if ((queen < 0) || (queen >= nqueens)) {
		fprintf(stderr, "Error in moveQueen: queen=%d "
			"(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
		exit(-1);
	}
	if ((column < 0) || (column >= nqueens)) {
		fprintf(stderr, "Error in moveQueen: column=%d "
			"(should be 0<=column<%d)...Abort.\n", column, nqueens);
		exit(-1);
	}
	queens[queen] = column;
}

/* returns TRUE if queen can be moved to position
* (queen,column). Note that this routine checks only that
* the values of queen and column are valid! It does not test
* conflicts!
*/
int canMoveTo(int queen, int column) {
	if ((queen < 0) || (queen >= nqueens)) {
		fprintf(stderr, "Error in canMoveTo: queen=%d "
			"(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
		exit(-1);
	}
	if (column < 0 || column >= nqueens) return FALSE;
	if (queens[queen] == column) return FALSE; /* queen already there */
	return TRUE;
}

/* returns the column number of the specified queen */
int columnOfQueen(int queen) {
	if ((queen < 0) || (queen >= nqueens)) {
		fprintf(stderr, "Error in columnOfQueen: queen=%d "
			"(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
		exit(-1);
	}
	return queens[queen];
}

/* returns the number of pairs of queens that are in conflict */
int countConflicts() {
	int cnt = 0;
	int queen, other;
	for (queen = 0; queen < nqueens; queen++) {
		for (other = queen + 1; other < nqueens; other++) {
			if (inConflict(queen, queens[queen], other, queens[other])) {
				cnt++;
			}
		}
	}
	return cnt;
}

/* evaluation function. The maximal number of queens in conflict
* can be 1 + 2 + 3 + 4 + .. + (nquees-1)=(nqueens-1)*nqueens/2.
* Since we want to do ascending local searches, the evaluation
* function returns (nqueens-1)*nqueens/2 - countConflicts().
*/
int evaluateState() {
	return (nqueens - 1)*nqueens / 2 - countConflicts();
}

/*************************************************************/

/* A very silly random search 'algorithm' */
#define MAXITER 1000
void randomSearch() {
	int queen, iter = 0;
	int optimum = (nqueens - 1)*nqueens / 2;

	while (evaluateState() != optimum) {
		printf("iteration %d: evaluation=%d\n", iter++, evaluateState());
		if (iter == MAXITER) break;  /* give up */
									 /* generate a (new) random state: for each queen do ...*/
		for (queen = 0; queen < nqueens; queen++) {
			int pos, newpos;
			/* position (=column) of queen */
			pos = columnOfQueen(queen);
			/* change in random new location */
			newpos = pos;
			while (newpos == pos) {
				newpos = random() % nqueens;
			}
			moveQueen(queen, newpos);
		}
	}
	if (iter < MAXITER) {
		printf("Solved puzzle. ");
	}
	printf("Final state is");
	printState();
}

/*************************************************************/


void hillClimbing() {
	int min=evaluateState();
	int x,y,z,i,j,change;
	while(change!=0){
		change=0;
		for( i=0;i<nqueens;i++){
			for( j=0;j<nqueens;j++){
				if(canMoveTo(i,j)){
					z=queens[i];
					queens[i]=j;
					if(evaluateState()>min){
						change++;
						min=evaluateState();
						x=i;
						y=j;
					}
					queens[i]=z;
				}	
			}
		}
		if(change!=0){
			queens[x]=y;
		}	
	}
	printState();
}

/*************************************************************/

void simulatedAnnealing() {

	printf("Implement the routine simulatedAnnealing() yourself!!\n");
}
/***************************************************************/

int inConflictWithAnotherQueen2(int *pop,int row, int col) {
	int queen;
	for (queen = 0; queen < nqueens; queen++) {
		if (inConflict(row, col, queen, pop[queen])) {
			if ((row != queen) || (col != pop[queen])) return TRUE;
		}
	}
	return FALSE;
}

void printState2(int *population) {
	int row, column;
	printf("\n");
	for (row = 0; row < nqueens; row++) {
		for (column = 0; column < nqueens; column++) {
			if (population[row] != column) {
				printf(".");
			}
			else {
				if (inConflictWithAnotherQueen2(population,row, column)) {
					printf("Q");
				}
				else {
					printf("q");
				}
			}
		}
		printf("\n");
	}
}


void initiateQueens2(int flag,int **pop) {
	int q;
	for (q = 0; q < nqueens; q++) {
		*pop[q] = (flag == 0 ? 0 : random() % nqueens);
	}
}

int countConflicts2(int *pop) {
	int cnt = 0;
	int queen, other;
	for (queen = 0; queen < nqueens; queen++) {
		for (other = queen + 1; other < nqueens; other++) {
			if (inConflict(queen, pop[queen], other, pop[other])) {
				cnt++;
			}
		}
	}
	return cnt;
}

 void selection(Pop *best,Pop *arr){
	best[0].fitness=999999;
	best[1].fitness=999999;
	int i;
	for(i=0;i<nqueens;i++){
		if(arr[i].fitness<best[0].fitness){
			best[0]=arr[i];
			//printf(" fitness 0: %d		fitness 1: %d\n",  best[0].fitness,best[1].fitness);
		}
	}
	for(i=0;i<nqueens;i++){
		//printf(" fitness 0: %d		fitness 1: %d\n",  best[0].fitness, best[1].fitness);
		if(arr[i].fitness<best[1].fitness&&arr[i].fitness>=best[0].fitness){
			best[1]=arr[i];
		}
		
	}
}

Pop reproduce(Pop *arr){
	Pop child;
	initializeRandomGenerator();
	int rand =3;

	int i;
	for(i=0;i<rand;i++){
	
		child.arr[i]=arr[0].arr[i];
	}
	for(i=rand;i<nqueens;i++){
		child.arr[i]=arr[1].arr[i];
	}
	child.fitness=countConflicts(child.arr);
	//printf(" %d",child.fitness);
	printState2(child.arr);
	return child;
}

Pop mutate(Pop child){
	int rand=random()%nqueens;
	int value=(random()%nqueens);
	child.arr[rand]=value;
	child.fitness=countConflicts(child.arr);
	return child;
}
int findBest(Pop *arr){
	int i,best=999999999;
	for (i=0;i<nqueens;i++){
		if(arr[i].fitness<best){
			best=arr[i].fitness;
		}
	}
	return best;
}
/*int *copy(int *arr){
	int i,*arr2;
	for(i=0;i<nqueens;i++){
		arr2[i]=arr[i];
	}
	return arr2;
}*/
void geneticAlgorithm(){
		printf(" 1");
		Pop population[nqueens];
		int i,j,iter=0,rand,best=999999999;
		
		initializeRandomGenerator();
		
		for(i=0;i<nqueens;i++){
			//initializeRandomGenerator();
			initiateQueens(1);
			population[i].fitness=countConflicts();
			for(j=0;j<nqueens;j++){
			population[i].arr[j]=queens[j];
			}
		}
			
			
		
		
		while(best!=0&&iter!=MAXITER){
			
			//printf("best  %d\n",best	);		
			Pop new[nqueens];
			for(i=0;i<nqueens;i++){
				iter++;
				Pop x[2];
				selection(x,population);
				Pop child = reproduce(x);
				rand=random()%nqueens;
				if(rand ==nqueens/4){
					child=mutate(child);
				}
				printState2(child.arr);
				new[i]=child;
			}
			best=findBest(new);
			for(i=0;i<nqueens;i++){
				population[i]=new[i];
			}
		}
		for(i=0;i<nqueens;i++){
			if(population[i].fitness==best){
				printState2(population[i].arr);
				return;
			}
		
	}
	//printState2(population[i].arr);
	return;
}



/**************************************************************/

int main(int argc, char *argv[]) {
	int algorithm;

	do {
		printf("Number of queens (1<=nqueens<%d): ", MAXQ);
		scanf("%d", &nqueens);
	} while ((nqueens < 1) || (nqueens > MAXQ));

	do {
		printf("Algorithm: (1) Random search  (2) Hill climbing  ");
		printf("(3) Simulated Annealing       (4) Genetic Algorithm ");
		scanf("%d", &algorithm);
	} while ((algorithm < 1) || (algorithm > 4));

	initializeRandomGenerator();

	initiateQueens(1);

	printf("\nInitial state:");
	printState();

	switch (algorithm) {
	case 1: randomSearch();       break;
	case 2: hillClimbing();       break;
	case 3: simulatedAnnealing(); break;
	case 4: geneticAlgorithm();	  break;
	}
	printf("evaluation: %d",evaluateState());

	return 0;
};

