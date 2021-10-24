#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 2048
#define NT 8
#define MAX 30


int getNeighbors(int **grid, int i, int j) {
    int x, y, viz = 0;

    for(x = -1; x < 2; x++) {
        for(y = -1; y < 2; y++) {
            if((i+x) >= 0 && (i+x) < N && (j+y) >= 0 && (j+y) < N) {
                if(x != 0 || y != 0) viz += grid[i+x][j+y];
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

