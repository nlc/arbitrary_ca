#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define GRID_SIZE 512

void swap_ptrs(void **a, void **b) {
  void *temp = *a;
  *a = *b;
  *b = temp;
}

typedef unsigned char byte;

/* Rule is 512 bits */
typedef struct {
  byte _chunks[64];
} Rule;

Rule *rule_init_zero() {
  Rule *rule = (Rule *)malloc(sizeof(Rule));

  for(int i = 0; i < 64; ++i) {
    rule->_chunks[i] = 0;
  }

  return rule;
}

Rule *rule_init_rand() {
  Rule *rule = (Rule *)malloc(sizeof(Rule));

  for(int i = 0; i < 64; ++i) {
    rule->_chunks[i] = (byte)rand();
  }

  return rule;
}

Rule *rule_init_func(byte (*func)(int)) {
  Rule *rule = (Rule *)malloc(sizeof(Rule));

  for(int i = 0; i < 512; ++i) {
    if(i % 8 == 0) {
      rule->_chunks[i / 8] = 0;
    }

    rule->_chunks[i / 8] |= ((func(i) & 1) << (i % 8));
  }

  return rule;
}

void rule_print(Rule *rule) {
  for(int i = 63; i >= 0; --i) {
    printf("%02X", rule->_chunks[i]);
  }
  printf("\n");
}

byte rule_bit(Rule *rule, int index) {
  int chunk = index / 8;
  int bit = index % 8;

  return (rule->_chunks[chunk] >> bit) & 1;
}

typedef struct {
  byte cells[GRID_SIZE][GRID_SIZE];
} Grid;

Grid *grid_init_rand() {
  Grid *grid = (Grid *)malloc(sizeof(Grid));
  
  for(int i = 0; i < GRID_SIZE; ++i) {
    for(int j = 0; j < GRID_SIZE; ++j) {
      grid->cells[i][j] = (byte)(rand() & 1);
    }
  }

  return grid;
}

Grid *grid_init_zero() {
  Grid *grid = (Grid *)malloc(sizeof(Grid));
  
  for(int i = 0; i < GRID_SIZE; ++i) {
    for(int j = 0; j < GRID_SIZE; ++j) {
      grid->cells[i][j] = 0;
    }
  }

  return grid;
}

Grid *grid_init_func(byte (*func)(int x, int y)) {
  Grid *grid = (Grid *)malloc(sizeof(Grid));
  
  for(int i = 0; i < GRID_SIZE; ++i) {
    for(int j = 0; j < GRID_SIZE; ++j) {
      grid->cells[i][j] = (func(j, i) & 1);
    }
  }

  return grid;
}

int grid_neighborhood_state(Grid *grid, int coord_x, int coord_y) {
  int neighborhood = 0;
  int bit = 0;
  for(int i = -1; i < 2; ++i) {
    int x = (coord_x + i + GRID_SIZE) % GRID_SIZE;
    for(int j = -1; j < 2; ++j) {
      int y = (coord_y + j + GRID_SIZE) % GRID_SIZE;
      neighborhood |= ((grid->cells[x][y] & 1) << (bit++));
    }
  }

  return neighborhood;
}

void grid_apply_rule(Grid *buffer, Grid *grid, Rule *rule) {
  for(int i = 0; i < GRID_SIZE; ++i) {
    for(int j = 0; j < GRID_SIZE; ++j) {
      buffer->cells[i][j] = rule_bit(rule, grid_neighborhood_state(grid, i, j));
    }
  }
}

void grid_draw(FILE *output, Grid *buffer, Grid* grid, int iteration) {
  /* header */
  fprintf(output, "P1\n%d %d\n# iteration %d\n", GRID_SIZE, GRID_SIZE, iteration);

  /* data */
  for(int i = 0; i < GRID_SIZE; ++i) {
    for(int j = 0; j < GRID_SIZE; ++j) {
      fprintf(output, "%d ", grid->cells[i][j]);
    }
    fprintf(output, "\n");
  }
  fprintf(output, "\n");
}

byte cgol_rule_gen(int index) {
  int count = __builtin_popcount(index);
  if(count == 3 || ((count == 4) && ((index >> 4) & 1))) {
    return 1;
  }
  return 0;
}

byte test_rule_gen(int index) {
  /* mostly random, try to avoid low-count configurations */
  int count = __builtin_popcount(index);
  if(count < 2) {
    return 0;
  }
  /* cgol has a hamming density of 140 / 512 = 0.2734375 */
  /* return (rand() % 1000) < 273; */
  return (rand() % 1000) < 350;
}

byte wolfram30_rule_gen(int index) {
  return ((index < 30) && ((30 >> index) & 1)) || (index & (1 << 4));
}

byte wolfram30inplace_rule_gen(int index) {
  return ((index < 30) && (((30 << 3) >> index) & 1));
}

byte wolfram30_grid_gen(int x, int y) {
  return (y == 0 && x == GRID_SIZE / 2) & 1;
}

int main() {
  /* seed rng */
  struct timeval time;
  gettimeofday(&time, NULL);
  srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

  Rule *rule = rule_init_func(wolfram30inplace_rule_gen);
  Grid *grid = grid_init_rand(), *buffer = grid_init_zero();

  rule_print(rule);

  /* int index = 0; */
  /*printf("{\n");*/
  /* for(int i = 0; i < 8; ++i) { */
    /* char str[64];
    sprintf(str, "i_%02d.pbm", i);
    FILE *f = fopen(str, "w");
    fprintf(f, "P1\n8 8\n# i = %d\n", i); */
    /* printf("  {\n"); */
    /* for(int j = 0; j < 8; ++j) { */
      /* printf("    {"); */
      /* for(int k = 0; k < 8; ++k) { */
        /* fprintf(f, "%d ", rule_bit(rule, index++)); */
        /* printf("%d ", rule_bit(rule, index++));
      }
      printf("\n");
    }
    printf("\n"); */
    /* fclose(f); */
  /* } */
  /* printf("}\n"); */
  /* printf("\n"); */
  /* return 0; */

  /* for(int i = 50; i < 70; ++i) {
    printf("%d -> %d bits -> %d result\n", i, __builtin_popcount(i), cgol_rule_gen(i));
  }
  return 0; */

  char str[1024];
  for(int iteration = 0; iteration < GRID_SIZE; ++iteration) {
    grid_apply_rule(buffer, grid, rule);
    sprintf(str, "iteration_%04d.pbm", iteration);
    FILE *output = fopen(str, "w");
    grid_draw(output, buffer, grid, iteration);
    fclose(output);
    swap_ptrs((void **)&grid, (void **)&buffer);
  }

  free(rule);
  free(grid);
  free(buffer);

  return 0;
}
