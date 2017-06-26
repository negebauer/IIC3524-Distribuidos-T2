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
  struct Route *route;
};

typedef struct Route Route;
typedef struct WSP WSP;

Route *routeInit(WSP *wsp) {
  Route *route = malloc(sizeof(Route));
  *route = (Route){.size = 1, .cost = 0};
  route->cities = calloc(wsp->size, sizeof(int *));
  route->cities[0] = 0;
  route->visited = calloc(wsp->size, sizeof(int *));
  route->visited[0] = 1;
  for (int i = 1; i < wsp->size; i++) {
    route->cities[i] = -1;
  }
  return route;
};

void routeFree(WSP *wsp, Route *route) {
  free(route->cities);
  free(route->visited);
  free(route);
};

WSP *wspInit(char *input) {
  printf("Reading file %s\n", input);
  FILE *file = fopen(input, "r");
  int size;
  fscanf(file, "%i", &size);
  printf("Allocating wsp\n");
  WSP *wsp = malloc(sizeof(WSP));
  *wsp = (WSP){.size = size};

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

void wspPrint(WSP *wsp) {
  printf("WSP\n");
  printf("size %i\n", wsp->size);
  if (wsp->route) {
    printf("cost %i\n", wsp->route->cost);
  }
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
  printf("\n");
};

void wspFree(WSP *wsp) {
  for (int origin = 0; origin < wsp->size; origin++) {
    free(wsp->roads[origin]);
  }
  routeFree(wsp, wsp->route);
  free(wsp->roads);
  free(wsp);
};
