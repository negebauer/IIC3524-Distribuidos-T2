#include <stdio.h>
#include <stdlib.h>

struct Route {
  int *cities;
  int *visited;
  int size;
  int cost;
};

struct WSP {
  int size;
  int **roads;
  int *cities;
  int cost;
};

typedef struct Route Route;
typedef struct WSP WSP;

// Important stuff

void routePrintDebug(WSP *wsp, Route *route) {
  printf(" visited: ");
  for (int i = 0; i < wsp->size; i++) {
    printf(" %i", route->visited[i]);
  }
  printf("\n");
  printf(" cities:  ");
  for (int i = 0; i < wsp->size; i++) {
    printf(" %i", route->cities[i]);
  }
  printf("\n");
};

int routeShouldVisit(Route *route, int destination) {
  return route->visited[destination] == 0;
};

void routeAdvance(WSP *wsp, Route *route, int destination) {
  int origin = route->cities[route->size - 1];
  int cost = wsp->roads[origin][destination];
  route->cities[route->size] = destination;
  route->visited[destination] = 1;
  route->cost += cost;
  route->size += 1;

  printf("Travel from %i to %i cost %i total %i\n", origin, destination, cost,
         route->cost);
  routePrintDebug(wsp, route);
};

void routeReturn(WSP *wsp, Route *route, int destination) {
  int origin = route->cities[route->size - 2];
  int cost = wsp->roads[origin][destination];
  route->cities[route->size - 1] = 0;
  route->visited[destination] = 0;
  route->cost -= cost;
  route->size -= 1;

  printf("Return from %i to %i cost %i total %i\n", destination, origin, cost,
         route->cost);
  routePrintDebug(wsp, route);
};

// Init stuff

Route *routeInit(WSP *wsp) {
  Route *route = malloc(sizeof(Route));
  *route = (Route){.size = 1, .cost = 0};
  route->cities = calloc(wsp->size, sizeof(int *));
  route->cities[0] = 0;
  route->visited = calloc(wsp->size, sizeof(int *));
  route->visited[0] = 1;
  for (int i = 1; i < wsp->size; i++) {
    route->cities[i] = 0;
    route->visited[i] = 0;
  }
  return route;
};

WSP *wspInit(char *input) {
  printf("Reading file %s\n", input);
  FILE *file = fopen(input, "r");
  int size;
  fscanf(file, "%i", &size);
  printf("Allocating wsp\n");
  WSP *wsp = malloc(sizeof(WSP));
  *wsp = (WSP){.size = size, .cost = -1};

  wsp->cities = calloc(size, sizeof(int));
  wsp->roads = calloc(size, sizeof(int *));
  for (int origin = 0; origin < size; origin++) {
    wsp->roads[origin] = calloc(size, sizeof(int));
  }
  for (int origin = 0; origin < size; origin++) {
    for (int destination = origin + 1; destination < size; destination++) {
      int cost;
      fscanf(file, "%i", &cost);
      printf("Allocating road from city %i to %i cost %i\n", origin,
             destination, cost);
      wsp->roads[origin][destination] = cost;
      wsp->roads[destination][origin] = cost;
    }
  }
  fclose(file);
  return wsp;
};

// Print stuff

void routePrint(Route *route) {
  printf("Route cost %i\n\t", route->cost);
  for (int i = 0; i < route->size; i++) {
    printf("%i ", route->cities[i]);
  }
  printf("\n");
};

void wspPrint(WSP *wsp) {
  printf("WSP\n");
  printf("size %i\n", wsp->size);
  printf("cost %i\n", wsp->cost);
  printf("Cities\n");
  for (int destination = 0; destination < wsp->size; destination++) {
    printf("\t%i", destination);
  }
  for (int origin = 0; origin < wsp->size; origin++) {
    printf("\n%i", origin);
    for (int destination = 0; destination < wsp->size; destination++) {
      printf("\t%i", wsp->roads[origin][destination]);
    }
  }
  printf("\nRoute\n");
  for (int i = 0; i < wsp->size; i++) {
    printf("%i\t", wsp->cities[i]);
  }
  printf("\n");
};

// Free stuff

void routeFree(WSP *wsp, Route *route) {
  free(route->cities);
  free(route->visited);
  free(route);
};

void wspFree(WSP *wsp) {
  for (int origin = 0; origin < wsp->size; origin++) {
    free(wsp->roads[origin]);
  }
  free(wsp->cities);
  free(wsp->roads);
  free(wsp);
};
