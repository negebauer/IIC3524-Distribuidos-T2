#include <stdio.h>
#include <stdlib.h>

struct Road {
  struct City *city1;
  struct City *city2;
  int cost;
};

struct City {
  int number;
  int visited;
  struct Road **roads;
};

struct WSP {
  int size;
  int cheapest;
  struct City **cities;
};

typedef struct Road Road;
typedef struct City City;
typedef struct WSP WSP;

WSP *wspInit(char *input) {
  FILE *file = fopen(input, "r");
  int size;
  fscanf(file, "%i", &size);
  WSP *wsp = malloc(sizeof(WSP));
  *wsp = (WSP){.cheapest = -1, .size = size};

  wsp->cities = calloc(size, sizeof(City));
  for (int number = 0; number < size; number++) {
    printf("Allocating city %i\n", number);
    City *city = malloc(sizeof(City));
    *city = (City){.number = number, .visited = 0};
    city->roads = calloc(size - 1, sizeof(Road));
    wsp->cities[number] = city;
  }

  for (int number = 0; number < size - 1; number++) {
    for (int connection = number; connection < size - 1; connection++) {
      City *city1 = wsp->cities[number];
      City *city2 = wsp->cities[connection + 1];
      int cost;
      fscanf(file, "%i", &cost);
      printf("Cost from city %i to %i is %i\n", number, connection + 1, cost);
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
  printf("cheapest %i\n", wsp->cheapest);
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
  free(wsp);
};
