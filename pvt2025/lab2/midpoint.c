#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

const double eps = 1E-6;
const int n0 = 10;

const double a = 0.1;
const double b = 1;

double func(double x) { return (x == 0) ? 1.0 : log(1 + x) / x; }

int main(int argc, char **argv) {
  int commsize, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int n = n0, k;
  double sq[2], delta = 1;
  double start_time, end_time, total_time, average_time;
  start_time = MPI_Wtime();

  for (k = 0; delta > eps; n *= 2, k ^= 1) {
    int points_per_proc = n / commsize;
    int lb = rank * points_per_proc;
    int ub = (rank == commsize - 1) ? (n - 1) : (lb + points_per_proc - 1);
    double h = (b - a) / n;
    double s = 0.0;

    for (int i = lb; i <= ub; i++)
      s += func(a + h * (i + 0.5));

    MPI_Allreduce(&s, &sq[k], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    sq[k] *= h;

    if (n > n0)
      delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
  }

  end_time = MPI_Wtime() - start_time;
  MPI_Reduce(&end_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    average_time = total_time / commsize;
    printf("%d\t%f\n", commsize, average_time);
    printf("Result: %.12f; Runge rule: EPS %e, n %d; Time taken: %f\n", sq[k],
           eps, n / 2, average_time);
  }

  MPI_Finalize();
  return 0;
}