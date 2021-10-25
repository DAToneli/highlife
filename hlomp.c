#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 2048
#define NT 8
#define MAX 2000


int getNeighbors(int **grid, int x, int y) {
    int dx, dy, viz = 0;

    for(dx = -1; dx < 2; dx++) {
        for(dy = -1; dy < 2; dy++) {
            if((x + dx) >= 0 && (x + dx) < N && (y + dy) >= 0 && (y + dy) < N) {
            	if(dx != 0 || dy != 0) viz += grid[x + dx][y + dy];
            } else if((x + dx) == -1) {
            	if((y + dy) >= 0 && (y + dy) < N) viz += grid[N - 1][y + dy];
            	else if((y + dy) == -1) viz += grid[N - 1][N - 1];
            	else viz += grid[N - 1][0];
            } else if((x + dx) == N) {
            	if((y + dy) >= 0 && (y + dy) < N) viz += grid[0][y + dy];
            	else if((y + dy) == -1) viz += grid[0][N - 1];
            	else viz += grid[0][0];
            } else if((y + dy) == -1) {
            	if((x + dx) >= 0 && (x + dx) < N) viz += grid[x + dx][N - 1];
            	else if((x + dx) == -1) viz += grid[N - 1][N - 1];
            	else viz += grid[0][N - 1];
            } else {
            	if((x + dx) >= 0 && (x + dx) < N) viz += grid[x + dx][0];
            	else if((x + dx) == -1) viz += grid[N - 1][0];
            	else viz += grid[0][0];
            }
        }
    }

    return viz;
}

int getLivingCells(int **grid) {
    int i, j, k = 0;

    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            if(grid[i][j] == 1) k++;
        }
    }

    return k;
}

int main(int argc, char* argv[])
{
    int i, j, k, n, soma;
    double start, end;
    
    start = omp_get_wtime();
    
    int  **grid = malloc(N*sizeof(int*));
    grid[0] = malloc(N*N*sizeof(int));
    for(i = 1; i < N; i++) grid[i] = grid[0] + i*N;
    
    int  **newgrid = malloc(N*sizeof(int*));
    newgrid[0] = malloc(N*N*sizeof(int));
    for(i = 1; i < N; i++) newgrid[i] = newgrid[0] + i*N;
    
    for(i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            grid[i][j] = 0;
        }
    }

    grid[1][2] = 1;
    grid[2][3] = 1;
    grid[3][1] = 1;
    grid[3][2] = 1;
    grid[3][3] = 1;

    grid[10][31] = 1;
    grid[10][32] = 1;
    grid[11][30] = 1;
    grid[11][31] = 1;
    grid[12][31] = 1;
    
    printf("**HighLife \n");
    printf("Condição inicial: %d \n", getLivingCells(grid));
    
    for(k = 0; k < MAX; k++) {
    	soma = 0;
    	#pragma omp parallel private(i,j,n) num_threads(NT) reduction (+:soma)
   	{
   	    //printf("%d\n",omp_get_thread_num());
	    #pragma omp for
		for(i = 0; i < N; i++) {
	    	    for(j = 0; j < N; j++) {
			n = getNeighbors(grid,i,j);
		        if(n == 2) {
			    newgrid[i][j] = grid[i][j];
			    if(newgrid[i][j] == 1) soma++;
		        } else if(n == 3 || n == 6) {
			    newgrid[i][j] = 1;
			    soma++;
		        } else {
			    newgrid[i][j] = 0;
		        }
	         }
	     }
        }
        printf("Geração %d: %d \n", k + 1, soma);
        for(i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                grid[i][j] = newgrid[i][j];
            }
        }
    }
    
    end = omp_get_wtime();
    
    printf("Tempo de execução: %f s. \n", end-start);

    return 0;
}

