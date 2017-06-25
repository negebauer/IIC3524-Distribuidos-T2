#include <stdio.h>

typedef struct Cell { int available; } Cell;

void cellPrint(Cell cell) { printf("available: %i\n", cell.available); };
