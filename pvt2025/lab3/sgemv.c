#include <inttypes.h>
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 28000

void get_chunk(int a, int b, int commsize, int rank, int *lb, int *ub) {
  int total_rows = b - a + 1;
  int rows_per_process = total_rows / commsize;
  int remainder = total_rows % commsize;

  *lb = a + rank * rows_per_process + (rank < remainder ? rank : remainder);
  *ub = *lb + rows_per_process + (rank < remainder ? 1 : 0) - 1;
}

void sgemv(float *a, float *b, float *c, int m, int n) {
  int commsize, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int lb, ub;
  get_chunk(0, m - 1, commsize, rank, &lb, &ub);
  int nrows = ub - lb + 1;

  for (int i = 0; i < nrows; i++) {
    c[lb + i] = 0.0f;
    for (int j = 0; j < n; j++) {
      c[lb + i] += a[(lb + i) * n + j] * b[j];
    }
  }

  if (rank == 0) {
    MPI_Gather(MPI_IN_PLACE, nrows, MPI_FLOAT, c, nrows, MPI_FLOAT, 0,
               MPI_COMM_WORLD);
  } else {
    MPI_Gather(c + lb, nrows, MPI_FLOAT, NULL, 0, MPI_FLOAT, 0, MPI_COMM_WORLD);
  }
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int commsize, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int m = N;
  int n = N;

  float *a = malloc(sizeof(float) * m * n);
  float *b = malloc(sizeof(float) * n);
  float *c = malloc(sizeof(float) * m);

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      a[i * n + j] = i + 1;
    }
  }
  for (int j = 0; j < n; j++) {
    b[j] = j + 1;
  }
  double time = MPI_Wtime();

  sgemv(a, b, c, m, n);

  time = MPI_Wtime() - time;

  if (rank == 0) {
    printf("%d\t%.6f\n", commsize, time);
  }
  free(a);
  free(b);
  free(c);
  MPI_Finalize();
  return 0;
}
