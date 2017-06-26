#include "shared.c"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 4

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

void parallel(WSP *wsp, int rank, Route *route) {
  int send_destinations[wsp->size - 1];
  for (int destination = 0; destination < wsp->size - 1; destination++) {
    send_destinations[destination] = destination + 1;
  }
  int count = 1;
  int source = 0;
  int recv_destination;
  MPI_Scatter(send_destinations, count, MPI_INT, &recv_destination, count,
              MPI_INT, source, MPI_COMM_WORLD);

  if (rank >= wsp->size - 1) {
    printf("rank: %i no work\n", rank);
    return;
  }

  routeAdvance(wsp, route, recv_destination);
  dfs(wsp, route);

  printf("rank: %i\n", rank);
  wspPrintRoute(wsp);
};

int main(int argc, char *argv[]) {
  /* The program receives 1 param */
  if (argc != 2) {
    printf("Modo de uso: %s <input.txt>\n", argv[0]);
    printf("\t<input.txt> es el problema a resolver\n");
    return 1;
  }
  int rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  char *input = argv[1];
  WSP *wsp = wspInit(input);
  Route *route = routeInit(wsp);

  parallel(wsp, rank, route);

  if (rank == 0) {
    wspPrintRoute(wsp);
  }
  wspFree(wsp);
  MPI_Finalize();
  return 0;
}
