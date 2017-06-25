#include "shared.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
  /* The program receives 1 param */
  if (argc != 2) {
    printf("Modo de uso: %s <input.txt>\n", argv[0]);
    printf("\t<input.txt> es el problema a resolver\n");
    return 1;
  }

  printf("Hello, world! I am secuential\n");
  Cell cell1 = (Cell){.available = 0};
  Cell *cell2 = malloc(sizeof(Cell));
  *cell2 = (Cell){.available = 1};

  cellPrint(cell1);
  cellPrint(*cell2);

  return 0;
}
