#include "shared.c"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void dfs(WSP *wsp, Route *route) {
  // If route completed, check if best
  if (route->cities[wsp->size - 1] != 0) {
    if (wsp->cost == -1 || route->cost < wsp->cost) {
      wsp->cost = route->cost;
      for (int i = 0; i < wsp->size; i++) {
        wsp->cities[i] = route->cities[i];
      }
    }
    return;
  }

  // If worse than best, stop travelling
  if (wsp->cost != -1 && route->cost >= wsp->cost) {
    // printf("Worse\t");
    // routePrint(route);
    return;
  }

  // Keep travelling
  for (int destination = 1; destination < wsp->size; destination++) {
    // printf("Checking destination %i\n", destination);
    if (routeShouldVisit(route, destination)) {
      routeAdvance(wsp, route, destination);
      dfs(wsp, route);
      routeReturn(wsp, route, destination);
    }
  }
}

int main(int argc, char *argv[]) {
  /* The program receives 1 param */
  if (argc != 2) {
    printf("Modo de uso: %s <input.txt>\n", argv[0]);
    printf("\t<input.txt> es el problema a resolver\n");
    return 1;
  }
  char *input = argv[1];
  // WSP *wsp = wspInit(input);
  // Route *route = routeInit(wsp);

  int size, rank, dest, source, rc, count, tag = 1;
  char inmsg, outmsg = "x";
  MPI_Status Stat;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    dest = 1;
    source = 1;
    rc = MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    rc = MPI_Recv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
  } else if (rank == 1) {
    dest = 0;
    source = 0;
    rc = MPI_Recv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
    rc = MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
  }
  rc = MPI_Get_count(&Stat, MPI_CHAR, &count);
  printf("Task %d: Received %d char(s) from task %d with tag %d \n", rank,
         count, Stat.MPI_SOURCE, Stat.MPI_TAG);

  MPI_Finalize();

  // wspPrintRoute(wsp);
  // wspFree(wsp);
  return 0;
}
