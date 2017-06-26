#include "shared.c"
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
  WSP *wsp = wspInit(input);
  Route *route = routeInit(wsp);

  dfs(wsp, route);

  wspPrintRoute(wsp);
  wspFree(wsp);
  return 0;
}
