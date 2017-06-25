#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
  /* The program receives 1 param */
  if (argc != 2) {
    printf("Modo de uso: %s <input.txt>\n", argv[0]);
    printf("\t<input.txt> es el problema a resolver\n");
    return 1;
  }

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf("Hello, world! I am process %d of %d\n", rank, size);
  MPI_Finalize();

  return 0;
}
