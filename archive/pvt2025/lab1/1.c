#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

int main(int argc, char **argv) {
  int rank, commsize, len, tag1 = 1, tag2 = 2;
  char host[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(host, &len);
  // ...
  char inbuf_prev[512], inbuf_next[512], outbuf_prev[512], outbuf_next[512];
  int prev = (rank + commsize - 1) % commsize;
  int next = (rank + 1) % commsize;

  int sizes[3] = {1, 1024, 1024 * 1024};
  char *sendbuf = NULL, *recvbuf = NULL;

  for (int i = 0; i < 3; i++) {
    int msg_size = sizes[i];

    sendbuf = (char *)malloc(msg_size);
    recvbuf = (char *)malloc(msg_size);
    memset(sendbuf, rank, msg_size);

    MPI_Barrier(MPI_COMM_WORLD);

    double start_time = 0.0;
    if (rank == 0) {
      start_time = MPI_Wtime();
    }

    for (int step = 0; step < commsize - 1; step++) {
      MPI_Request reqs[2];
      MPI_Isend(sendbuf, msg_size, MPI_CHAR, next, tag1, MPI_COMM_WORLD,
                &reqs[0]);
      MPI_Irecv(recvbuf, msg_size, MPI_CHAR, prev, tag1, MPI_COMM_WORLD,
                &reqs[1]);
      MPI_Waitall(2, reqs, stats);
      memcpy(sendbuf, recvbuf, msg_size);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
      double end_time = MPI_Wtime();
      printf("Message size: %d bytes, Time taken: %f seconds\n", msg_size,
             end_time - start_time);
    }

    free(sendbuf);
    free(recvbuf);
  }
  snprintf(outbuf_prev, NELEMS(outbuf_prev), "Hello, prev. I am %d of %d on %s",
           rank, commsize, host);
  snprintf(outbuf_next, NELEMS(outbuf_next), "Hello, next. I am %d of %d on %s",
           rank, commsize, host);
  MPI_Isend(outbuf_prev, NELEMS(outbuf_prev), MPI_CHAR, prev, tag1,
            MPI_COMM_WORLD, &reqs[0]);
  MPI_Isend(outbuf_next, NELEMS(outbuf_next), MPI_CHAR, next, tag2,
            MPI_COMM_WORLD, &reqs[1]);
  MPI_Irecv(inbuf_prev, NELEMS(inbuf_prev), MPI_CHAR, prev, tag2,
            MPI_COMM_WORLD, &reqs[2]);
  MPI_Irecv(inbuf_next, NELEMS(inbuf_next), MPI_CHAR, next, tag1,
            MPI_COMM_WORLD, &reqs[3]);
  MPI_Waitall(4, reqs, stats);
  printf("[%d] Msg from %d (prev): '%s'\n", rank, stats[2].MPI_SOURCE,
         inbuf_prev);
  printf("[%d] Msg from %d (next): '%s'\n", rank, stats[3].MPI_SOURCE,
         inbuf_next);
  // ...
  MPI_Finalize();
  return 0;
}