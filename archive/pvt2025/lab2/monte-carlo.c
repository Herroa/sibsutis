#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

const double area = 0.5;
const int n = 1E+7;

double func(double x, double y) { return exp(2 * (x + y)); }

double getrand() { return (double)rand() / RAND_MAX; }

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int rank, commsize;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);

  double start_time, end_time, total_time, average_time;
  srand(rank);
  double s = 0;
  start_time = MPI_Wtime();

  for (int i = rank; i < n; i += commsize) {
    double x = getrand();
    double y = getrand() * (1 - x);

    s += func(x, y);
  }

  double gsum = 0.0;
  MPI_Reduce(&s, &gsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  end_time = MPI_Wtime() - start_time;
  MPI_Reduce(&end_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    double res = area * gsum / n;
    average_time = total_time / commsize;
    printf("%d\t%f\n", commsize, average_time);
    printf("Result: %.12f, n %d; Time taken: %f\n", res, n, average_time);
  }

  MPI_Finalize();
  return 0;
}
