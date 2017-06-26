#include "shared.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void solve(WSP *wsp) {
  Route *route = routeInit(wsp, NULL);
  printf("cost %i\n", route->cost);
  printf("size %i\n", route->size);
  printf("visited first %i\n", route->visited[0]->number);
  printf("tour first %i\n", route->tour[0]->number);
  wsp->route = route;

  // while (!route)
  //
  //   dfs(wsp, route);
  // if (!wsp->route || route->cost < wsp->route->cost) {
  //   wsp->route = route;
  // } else {
  //   free(route);
  // }
};

int main(int argc, char *argv[]) {
  /* The program receives 1 param */
  if (argc != 2) {
    printf("Modo de uso: %s <input.txt>\n", argv[0]);
    printf("\t<input.txt> es el problema a resolver\n");
    return 1;
  }
  char *input = argv[1];
  WSP *wsp = wspInit(input);

  solve(wsp);
  printf("Cheapest %i\n", wsp->route->cost);

  wspFree(wsp);
  return 0;
}
