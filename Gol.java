import java.time.Instant;

public class Gol implements Runnable {

  static final int NT = 8;
  static final int N = 2048;
  static final int M = N/NT;
  static final int MAX = 2000;
  
  private int soma;
  private int[][] grid;
  private int at;
  private int[][] newgrid = new int[M][N];

  public Gol(int soma, int[][] grid, int at) {
    this.soma = soma;
    this.grid = grid;
    this.at = at;
  }
  
  static int getNeighbors(int[][] grid, int x, int y) {
    int dx,dy;
    int viz = 0;
    
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
  
  static int getLivingCells(int[][] grid) {
    int i,j,v;
    
    v = 0;
    
    for(i = 0; i < N; i++) {
      for(j = 0; j < N; j++) {
        if(grid[i][j] == 1) v++;
      }
    }
    
    return v;
  }
  
  public int getSoma() {
    return soma;
  }
  
  public int[][] getNewgrid() {
    return newgrid;
  }

  public void run() {
    int i,j,n;
    
    //System.out.print(Thread.currentThread().getName() + "\n");
    
    for(i = 0; i < M; i++) {
      for(j = 0; j < N; j++) {
        n = getNeighbors(grid, i + at*M, j);
        if(n == 2) {
          newgrid[i][j] = grid[i + at*M][j];
          if(newgrid[i][j] == 1) soma++;
        } else if(n == 3) {
          newgrid[i][j] = 1;
          soma++;
        } else {
          newgrid[i][j] = 0;
        }
      }
    }
    
  }

  public static void main(String[] args) {
  
    long startTime = Instant.now().toEpochMilli();
    
    int soma;
    int h,i,j,k;
    String nome;
    
    int[][] grid = new int[N][N];
    for(i = 0; i < N; i++) {
      for(j = 0; j < N; j++) {
        grid[i][j] = 0;
      }
    }
    
    int[][] temp = new int[M][N];
    
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
    
    Gol[] gols = new Gol[NT];
    Thread[] threads = new Thread[NT];
    
    System.out.println("**Game of Life");
    System.out.println("Condição inicial: " + getLivingCells(grid));
    
    for(h = 0; h < MAX; h++) {
    
      soma = 0;
    
      for(i = 0; i < NT; i++) {
        nome = "Thread " + ((char)(65+i));
        gols[i] = new Gol(soma, grid, i);
        threads[i] = new Thread(gols[i],nome);
        threads[i].start();
      }
    
      for(i = 0; i < NT; i++) {
        try{
          threads[i].join();
        } catch(Exception ex) {
          System.out.println("Exception has " + "been caught" + ex);
        }
      }
    
      for(i = 0; i < NT; i++) {
        soma += gols[i].getSoma();
        temp = gols[i].getNewgrid();
        for(j = 0; j < M; j++) {
          for(k = 0; k < N; k++) {
            grid[j + i*M][k] = temp[j][k];
          }
        }
      }
      
      System.out.println("Geração " + (h + 1) + ":" + soma);
    
      /*for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
          System.out.print(grid[i][j]);
        }
        System.out.print("\n");
      }*/
    
    }
    
    long endTime = Instant.now().toEpochMilli();
    
    long timeElapsed = endTime - startTime;
    
    System.out.println("Tempo de execução: " + timeElapsed*1E-3 + "s");
    
  }
}
