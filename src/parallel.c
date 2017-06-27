#include "shared.c"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

enum Status { FINISHED, COST, NODE, DESTINATION, ROUTE };

void watchWork(WSP *wsp, int **status, MPI_Request **requests_cost) {
  int stop = 0;
  while (!stop) {
    stop = 1;
    for (int i = 1; i < wsp->size - 1; i++) {
      if (status[i][NODE] != 0) {
        int received_cost = 0;
        MPI_Test(requests_cost[1], &received_cost, NULL);
        if (received_cost) {
          int node = status[i][NODE];
          status[i][NODE] = 0;
          status[i][FINISHED] = 1;
          int destination = -1;
          for (int d = 1; d < wsp->size - 1; d++) {
            if (status[d][NODE] == 0 && !status[d][FINISHED]) {
              destination = d;
              break;
            }
          }
          if (destination != -1) {
            status[destination][NODE] = node;
            MPI_Irecv(&status[destination][COST], 1, MPI_INT, node, COST,
                      MPI_COMM_WORLD, requests_cost[node]);
            // MPI_Irecv(&routes[destination], wsp->size - 1, MPI_INT, node,
            // ROUTE,
            //           MPI_COMM_WORLD, requests_route[node]);
          }
          MPI_Send(&destination, 1, MPI_INT, node, DESTINATION, MPI_COMM_WORLD);
          // Check for cost if share with others
        };
      }
      if (stop && !status[i][FINISHED]) {
        stop = 0;
      }
    }
  }
  for (int i = 1; i < wsp->size - 1; i++) {
    printf("i %i f %i n %i c %i\n", i, status[i][FINISHED], status[i][NODE],
           status[i][COST]);
  }
  int min = -1;
  for (int i = 1; i < wsp->size - 1; i++) {
    int cost = status[i][COST];
    if (min == -1 || (cost < min && cost > 0)) {
      min = status[i][COST];
    }
  }
  printf("min cost: %i\n", min);
  for (int i = 1; i < wsp->size - 1; i++) {
    free(status[i]);
  }
  int nodes;
  MPI_Comm_size(MPI_COMM_WORLD, &nodes);
  for (int i = 1; i < nodes; i++) {
    if (i < wsp->size - 1) {
      free(requests_cost[i]);
    }
  }
  free(status);
  free(requests_cost);
  return;
};

void awaitWork(WSP *wsp, int rank) {
  int destination;
  MPI_Recv(&destination, 1, MPI_INT, 0, DESTINATION, MPI_COMM_WORLD, NULL);
  while (destination != -1) {
    Route *route = routeInit(wsp);
    routeAdvance(wsp, route, destination);
    dfs(wsp, route, rank);
    wspPrintRoute(wsp);
    MPI_Send(&wsp->cost, 1, MPI_INT, 0, COST, MPI_COMM_WORLD);
    routeFree(wsp, route);
    MPI_Recv(&destination, 1, MPI_INT, 0, DESTINATION, MPI_COMM_WORLD, NULL);
  }
};

void parallelize(char *input) {
  WSP *wsp = wspInit(input);
  int nodes, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &nodes);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    int **status = calloc(wsp->size - 1, sizeof(int *));
    MPI_Request **requests_cost = calloc(nodes, sizeof(MPI_Request *));
    for (int i = 1; i < wsp->size - 1; i++) {
      status[i] = calloc(3, sizeof(int));
      status[i][FINISHED] = 0;
      status[i][COST] = 0;
      status[i][NODE] = 0;
    }

    for (int node = 1; node < nodes; node++) {
      if (node == wsp->size - 1) {
        printf("Moar nodes than cities\n");
        break;
      }
      requests_cost[node] = malloc(sizeof(MPI_Request));
      status[node][NODE] = node;
      int destination = node;
      MPI_Irecv(&status[destination][COST], 1, MPI_INT, node, COST,
                MPI_COMM_WORLD, requests_cost[node]);
      MPI_Send(&destination, 1, MPI_INT, node, DESTINATION, MPI_COMM_WORLD);
    }
    watchWork(wsp, status, requests_cost);
  } else if (rank < wsp->size - 1) {
    awaitWork(wsp, rank);
  }
  wspFree(wsp);
};

int main(int argc, char *argv[]) {
  /* The program receives 1 param */
  if (argc != 2) {
    printf("Modo de uso: %s <input.txt>\n", argv[0]);
    printf("\t<input.txt> es el problema a resolver\n");
    return 1;
  }
  MPI_Init(&argc, &argv);

  parallelize(argv[1]);

  MPI_Finalize();
  return 0;
}
