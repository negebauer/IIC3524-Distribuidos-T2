#include <stdio.h>
#include <stdlib.h>

struct CityNode {
  struct CityNode *next;
  struct City *city;
};

struct CityStack {
  struct CityNode *first;
  struct CityNode *last;
};

struct Route {
  int cost;
  struct CityStack *visited;
  struct Road **roads;
  struct CityStack *tour;
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

typedef struct CityNode CityNode;
typedef struct CityStack CityStack;
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
  exit(1);
};

void cityStackPush(CityStack *cityStack, City *city) {
  CityNode *node = malloc(sizeof(CityNode));
  node->city = city;
  node->next = cityStack->first;
  cityStack->first = node;
};

void cityStackBottom(CityStack *cityStack, City *city) {
  CityNode *node = malloc(sizeof(CityNode));
  node->city = city;
};

CityNode *cityStackPop(CityStack *cityStack) {
  if (!cityStack->first->city) {
    return NULL;
  }
  CityNode *node = cityStack->first;
  cityStack->first = node->next;
  return node;
};

CityStack *cityStackInit() {
  CityStack *stack = malloc(sizeof(CityStack));
  CityNode *node = malloc(sizeof(CityNode));
  stack->first = node;
  stack->last = node;
  return stack;
};

Route *routeInit(WSP *wsp) {
  Route *route = malloc(sizeof(Route));
  *route = (Route){.cost = 0};
  route->visited = cityStackInit();
  cityStackPush(route->visited, wsp->cities[0]);
  route->tour = malloc(sizeof(CityStack));
  for (int number = 1; number < wsp->size - 1; number++) {
    cityStackPush(route->tour, wsp->cities[number]);
  }
  return route;
};

void routeFree(Route *route) {
  free(route->visited->first);
  free(route->visited->last);
  free(route->visited);
  free(route->tour->first);
  free(route->tour->last);
  free(route->tour);
  free(route->roads);
  free(route);
};

int routeCompleted(Route *route) {
  if (!route->tour->first) {
    return 1;
  }
  return 0;
};

// void routeVisitDestination(WSP *wsp, Route *route, int destination) {
//   City *origin = route->visited[route->size - 1];
//   City *visit = route->tour[destination];
//   Road *road = roadFromCityToCity(wsp, origin, visit);
//   route->cost += road->cost;
//   route->roads[route->size - 1] = road;
//   route->visited[route->size] = visit;
//   route->size++;
//   routeTourPop(wsp, route);
// };

void routeGoBack(Route *route, City *city) {
  cityStackPop(route->visited);
  cityStackBottom(route->tour, city);
};

City *routeAdvance(WSP *wsp, Route *route) {
  CityNode *destination_node = cityStackPop(route->tour);
  if (!destination_node) {
    return NULL;
  }
  City *origin = route->visited->first->city;
  City *destination = destination_node->city;
  Road *road = roadFromCityToCity(wsp, origin, destination);
  route->cost += road->cost;
  cityStackPush(route->visited, destination);
  return destination;
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
