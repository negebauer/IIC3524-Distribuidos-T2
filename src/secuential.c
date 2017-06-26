#include "shared.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void dfs(WSP *wsp, Route *route) {
  // If route completed, check if best
  if (route->size == wsp->size) {
    if (!wsp->route || route->cost < wsp->route->cost) {
      wsp->route = route;
    }
    return;
  }

  // If worse than best, stop travelling
  if (wsp->route && route->cost >= wsp->route->cost) {
    return;
  }

  // Keep travelling
  for (int destination = 1; destination < wsp->size; destination++) {
    if (!route->visited[destination]) {
      int origin = route->size - 1;
      int cost = wsp->roads[origin][destination];
      route->visited[destination] = 1;
      route->cost += cost;
      route->cities[route->size] = origin;
      route->size += 1;
      printf("Travelling %i to %i\n", origin, destination);
      dfs(wsp, route);
      route->cities[route->size] = -1;
      route->visited[destination] = 0;
      route->cost -= cost;
      route->size -= 1;
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
  WSP *wsp = wspInit(input);
  wspPrint(wsp);

  Route *route = routeInit(wsp);
  dfs(wsp, route);

  wspPrint(wsp);
  wspFree(wsp);
  return 0;
}
