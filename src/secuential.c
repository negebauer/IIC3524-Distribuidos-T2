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
  char *input = argv[1];
  WSP *wsp = wspInit(input);

  // Solve

  wspFree(wsp);
  return 0;
}
