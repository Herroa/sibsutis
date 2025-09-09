#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_PARTICLES 100
#define NUM_STEPS 100
#define DT 0.001

typedef struct {
  double x, y, z;
  double vx, vy, vz;
  double fx, fy, fz;
} Particle;

void compute_forces(Particle *particles, int start, int end,
                    int num_particles) {
  for (int i = start; i < end; i++) {
    particles[i].fx = particles[i].fy = particles[i].fz = 0.0;
    for (int j = 0; j < num_particles; j++) {
      if (i != j) {
        double dx = particles[j].x - particles[i].x;
        double dy = particles[j].y - particles[i].y;
        double dz = particles[j].z - particles[i].z;
        double r2 = dx * dx + dy * dy + dz * dz;
        if (r2 > 1e-12) {
          double inv_r = 1.0 / sqrt(r2);
          double inv_r3 = inv_r * inv_r * inv_r;
          particles[i].fx += dx * inv_r3;
          particles[i].fy += dy * inv_r3;
          particles[i].fz += dz * inv_r3;
        }
      }
    }
  }
}

void update_positions(Particle *particles, int start, int end, double dt) {
  for (int i = start; i < end; i++) {
    particles[i].x += particles[i].vx * dt + 0.5 * particles[i].fx * dt * dt;
    particles[i].y += particles[i].vy * dt + 0.5 * particles[i].fy * dt * dt;
    particles[i].z += particles[i].vz * dt + 0.5 * particles[i].fz * dt * dt;

    particles[i].vx += particles[i].fx * dt;
    particles[i].vy += particles[i].fy * dt;
    particles[i].vz += particles[i].fz * dt;
  }
}

int main(int argc, char **argv) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  Particle *particles = malloc(NUM_PARTICLES * sizeof(Particle));
  int local_start = rank * (NUM_PARTICLES / size);
  int local_end = (rank + 1) * (NUM_PARTICLES / size);

  if (rank == 0) {
    for (int i = 0; i < NUM_PARTICLES; i++) {
      particles[i].x = drand48();
      particles[i].y = drand48();
      particles[i].z = drand48();
      particles[i].vx = drand48() - 0.5;
      particles[i].vy = drand48() - 0.5;
      particles[i].vz = drand48() - 0.5;
      particles[i].fx = particles[i].fy = particles[i].fz = 0.0;
    }
  }
  MPI_Bcast(particles, NUM_PARTICLES * sizeof(Particle), MPI_BYTE, 0,
            MPI_COMM_WORLD);

  double total_time = -MPI_Wtime();

  for (int step = 0; step < NUM_STEPS; step++) {
    compute_forces(particles, local_start, local_end, NUM_PARTICLES);
    MPI_Allgather(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, particles,
                  NUM_PARTICLES / size * sizeof(Particle), MPI_BYTE,
                  MPI_COMM_WORLD);
    update_positions(particles, local_start, local_end, DT);
    if (rank == 0) {
      double kinetic_energy = 0.0;
      for (int i = 0; i < NUM_PARTICLES; i++) {
        kinetic_energy += 0.5 * (particles[i].vx * particles[i].vx +
                                 particles[i].vy * particles[i].vy +
                                 particles[i].vz * particles[i].vz);
      }
      printf("step %d, kinetic energy: %f\n", step, kinetic_energy);
    }
  }

  total_time += MPI_Wtime();
  double max_time;
  MPI_Reduce(&total_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    printf("total time: %f\n", max_time);
  }

  free(particles);
  MPI_Finalize();
  return 0;
}
