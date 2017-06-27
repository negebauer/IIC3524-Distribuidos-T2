#include "shared.c"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 4

void dfs(WSP *wsp, Route *route, int rank) {
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
      dfs(wsp, route, rank);
      routeReturn(wsp, route, destination);
    }
  }
}

enum Tag { DESTINATION = 0, STATUS, ROUTE };

enum Status { FINISHED = 0, COST, NODE };

void watchWork(WSP *wsp, int **status, int **routes) {
  int stop = 1;
  for (int i = 0; i < wsp->size - 1; i++) {
    if (status[i][NODE] != 0 && status[i][FINISHED]) {
      int node = status[i][NODE];
      int destination = -1;
      for (int d = 0; d < wsp->size - 1; d++) {
        if (status[i][NODE] == 0 && !status[i][FINISHED]) {
          destination = d;
          break;
        }
      }
      printf("destination %i for %i\n", destination, node);
      MPI_Send(&destination, 1, MPI_INT, node, DESTINATION, MPI_COMM_WORLD);
      if (destination != -1) {
        MPI_Irecv(status[i], 3, MPI_INT, node, STATUS, MPI_COMM_WORLD, NULL);
        MPI_Irecv(routes[i], wsp->size - 1, MPI_INT, node, ROUTE,
                  MPI_COMM_WORLD, NULL);
      }
      // Check for cost if share with others
    };
    if (stop && !status[i][FINISHED]) {
      stop = 0;
    }
  }
  if (stop) {
    for (int i = 0; i < wsp->size - 1; i++) {
      free(status[i]);
      free(routes[i]);
    }
    free(status);
    free(routes);
    return;
  }
  watchWork(wsp, status, routes);
};

void awaitWork(WSP *wsp, int rank) {
  int destination;
  MPI_Recv(&destination, 1, MPI_INT, 0, DESTINATION, MPI_COMM_WORLD, NULL);
  if (destination == -1) {
    printf("rank %i stop\n", rank);
    return;
  }
  printf("start rank %i destination %i\n", rank, destination);
  Route *route = routeInit(wsp);
  routeAdvance(wsp, route, destination);
  dfs(wsp, route, rank);
  printf("finish rank %i destination %i\n", rank, destination);
  int status[3] = {1, wsp->cost, rank};
  MPI_Send(&status, 3, MPI_INT, 0, STATUS, MPI_COMM_WORLD);
  MPI_Send(route->cities, wsp->size - 1, MPI_INT, 0, ROUTE, MPI_COMM_WORLD);
  routeFree(wsp, route);
  awaitWork(wsp, rank);
};

void parallelize(WSP *wsp) {
  int MAX_NODES = 1;
  int nodes, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &nodes);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    int **status = calloc(wsp->size - 1, sizeof(int *));
    int **routes = calloc(wsp->size - 1, sizeof(int *));
    for (int i = 1; i < wsp->size - 1; i++) {
      status[i] = calloc(3, sizeof(int));
      status[i][FINISHED] = 0;
      status[i][COST] = 0;
      status[i][NODE] = 0;
      routes[i] = calloc(wsp->size - 1, sizeof(int));
    }

    for (int node = 1; node < nodes; node++) {
      if (node >= wsp->size - 1) {
        printf("Moar nodes than cities\n");
        break;
      }
      if (node > MAX_NODES) {
        printf("Only one node now\n");
        break;
      }
      status[node][NODE] = node;
      int i = node;
      MPI_Send(&i, 1, MPI_INT, node, DESTINATION, MPI_COMM_WORLD);
      MPI_Irecv(status[i], 3, MPI_INT, node, STATUS, MPI_COMM_WORLD, NULL);
      MPI_Irecv(routes[i], wsp->size - 1, MPI_INT, node, ROUTE, MPI_COMM_WORLD,
                NULL);
    }
    watchWork(wsp, status, routes);
  } else {
    awaitWork(wsp, rank);
  }
};

int main(int argc, char *argv[]) {
  /* The program receives 1 param */
  if (argc != 2) {
    printf("Modo de uso: %s <input.txt>\n", argv[0]);
    printf("\t<input.txt> es el problema a resolver\n");
    return 1;
  }
  MPI_Init(&argc, &argv);
  char *input = argv[1];
  WSP *wsp = wspInit(input);

  parallelize(wsp);

  wspFree(wsp);
  MPI_Finalize();
  return 0;
}
