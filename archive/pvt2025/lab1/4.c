#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void broadcast_test(int message_size) {
  int rank, commsize;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Request send_request, recv_request;
  // MPI_Status status;

  char *sbuf = (char *)malloc(message_size);
  char *rbuf = (char *)malloc(message_size * commsize);
  if (sbuf != NULL) {
    for (int i = 0; i < message_size / sizeof(int); i++) {
      sbuf[i] = i;
    }
  }

  double start_time, end_time;
  start_time = MPI_Wtime();
  for (int i = 0; i < commsize; i++) {
    MPI_Isend(sbuf, message_size, MPI_CHAR, i, 0, MPI_COMM_WORLD,
              &send_request);
    MPI_Irecv(rbuf, message_size, MPI_CHAR, i, 0, MPI_COMM_WORLD,
              &recv_request);
  }

  for (int i = 0; i < commsize; i++) {
    MPI_Wait(&send_request, MPI_STATUS_IGNORE);
    MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  end_time = MPI_Wtime();
  if (rank == 0) {
    printf("Message size: %d bytes, Time taken: %f seconds\n", message_size,
           end_time - start_time);
  }

  // printf("Rank %d received data: %s\n", rank, rbuf);
  free(sbuf);
  free(rbuf);
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  //
  broadcast_test(1024);
  broadcast_test(1024 * 1024);
  //
  MPI_Finalize();
  return 0;
}
