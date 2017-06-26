#include "shared.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// void dfs(WSP *wsp, Route *route) {
//   // If route completed, check if best
//   if (wsp->route) {
//     printf("Min cost %i route %i\n", wsp->route->cost, route->cost);
//   } else {
//     printf("Route cost %i\n", route->cost);
//   }
//
//   // If worse than best, stop travelling
//   if (wsp->route && route->cost >= wsp->route->cost) {
//     routeFree(route);
//     return;
//   }
//
//   // Lets keep travelling
//   while (!routeCompleted(route)) {
//     City *city = routeAdvance(wsp, route);
//     dfs(wsp, route);
//     routeGoback(route, city);
//   };
//
//   if (!wsp->route || route->cost < wsp->route->cost) {
//     wsp->route = route;
//   } else {
//     routeFree(route);
//   }
//   printf("Finished route\n");
// };

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

  // Route *route = routeInit(wsp);
  // dfs(wsp, route);
  // printf("Cheapest %i\n", wsp->route->cost);

  wspFree(wsp);
  return 0;
}
