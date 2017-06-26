#include <stdio.h>
#include <stdlib.h>

struct Route {
  int cost;
  int size;
  struct City **visited;
  struct Road **roads;
  struct City **tour;
};

struct Road {
  struct City *city1;
  struct City *city2;
  int cost;
};

struct City {
  int number;
  struct Road **roads;
};

struct WSP {
  int size;
  struct Route *route;
  struct City **cities;
};

typedef struct Route Route;
typedef struct Road Road;
typedef struct City City;
typedef struct WSP WSP;

Road *roadFromCityToCity(WSP *wsp, City *city1, City *city2) {
  for (int number = 0; number < wsp->size - 1; number++) {
    Road *road = city1->roads[number];
    if ((road->city1 == city1 && road->city2 == city2) ||
        (road->city2 == city1 && road->city1 == city2)) {
      return road;
    }
  }
  printf("ERROR NO ROUTE %i to %i\n", city1->number, city2->number);
  return city1->roads[0];
};

Route *routeInit(WSP *wsp, Route *previous) {
  int size = wsp->size - 1;
  Route *route = malloc(sizeof(Route));
  *route = (Route){.cost = 0, .size = 1};
  route->visited = calloc(size + 1, sizeof(City *));
  route->tour = calloc(size, sizeof(City *));
  route->visited[0] = wsp->cities[0];
  route->roads = calloc(size, sizeof(Road *));
  if (previous) {
    route->size = previous->size;
    route->cost = previous->cost;
    route->tour = previous->tour;
    for (int number = 0; number < previous->size - 1; number++) {
      route->visited[number + 1] = previous->visited[number + 1];
      route->roads[number] = previous->roads[number];
    }
  } else {
    for (int number = 0; number < size; number++) {
      route->tour[number] = wsp->cities[number + 1];
    }
  }
  return route;
};

void routeFree(Route *route) {
  free(route->visited);
  free(route->tour);
  free(route->roads);
  free(route);
};

void routeVisitCityTroughRoad(Route *route, City *city, Road *road) {
  route->cost += road->cost;
  route->roads[route->size - 1] = road;
  route->visited[route->size] = city;
  route->size++;
};

int routeCompleted(WSP *wsp, Route *route) {
  if (route->size == wsp->size) {
    return 1;
  }
  return 0;
};

void routePopTour(WSP *wsp, Route *route) {
  int destinations = wsp->size - route->size;
  for (int number = 0; number < destinations; number++) {
    route->tour[number] = route->tour[number + 1];
  }
};

void routeVisitDestination(WSP *wsp, Route *route, int destination) {
  City *origin = route->visited[route->size - 1];
  City *visit = route->tour[destination];
  Road *road = roadFromCityToCity(wsp, origin, visit);
  route->cost += road->cost;
  route->roads[route->size - 1] = road;
  route->visited[route->size] = visit;
  route->size++;
  routePopTour(wsp, route);
};

WSP *wspInit(char *input) {
  printf("Reading file %s\n", input);
  FILE *file = fopen(input, "r");
  int size;
  fscanf(file, "%i", &size);
  printf("Allocating wsp\n");
  WSP *wsp = malloc(sizeof(WSP));
  *wsp = (WSP){.size = size};

  wsp->cities = calloc(size, sizeof(City *));
  for (int number = 0; number < size; number++) {
    printf("Allocating city %i\n", number);
    City *city = malloc(sizeof(City));
    *city = (City){.number = number};
    city->roads = calloc(size - 1, sizeof(Road *));
    wsp->cities[number] = city;
  }

  for (int number = 0; number < size - 1; number++) {
    for (int connection = number; connection < size - 1; connection++) {
      City *city1 = wsp->cities[number];
      City *city2 = wsp->cities[connection + 1];
      int cost;
      fscanf(file, "%i", &cost);
      printf("Allocating road from city %i to %i cost %i\n", number,
             connection + 1, cost);
      Road *road = malloc(sizeof(Road));
      *road = (Road){.city1 = city1, .city2 = city2, .cost = cost};
      city1->roads[connection] = road;
      city2->roads[connection - number] = road;
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
  for (int number = 0; number < wsp->size; number++) {
    City *city = wsp->cities[number];
    printf("City %i\n", city->number);
    for (int connection = 0; connection < wsp->size - 1; connection++) {
      Road *road = city->roads[connection];
      printf("\tCity %i to %i costs %i\n", road->city1->number,
             road->city2->number, road->cost);
    }
  }
};

void wspFree(WSP *wsp) {
  for (int i = 0; i < wsp->size; i++) {
    City *city = wsp->cities[i];
    printf("Free city %i\n", city->number);
    for (int connection = city->number; connection < wsp->size - 1;
         connection++) {
      Road *road = city->roads[connection];
      printf("Free road %i to %i\n", road->city1->number, road->city2->number);
      free(road);
    }
    free(city->roads);
    free(city);
  }
  free(wsp->cities);
  routeFree(wsp->route);
  free(wsp);
};
