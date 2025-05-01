#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define putchar(c) putc ((c), stdout)

#define RATIO  2
#define LINES  20
#define COLS   LINES * RATIO

#define RADIUS 10

int
main (int argc, char **argv)
{
  srand(time(NULL));
  for (size_t i = 0; i < LINES; i++)
    {
      for (size_t j = 0; j < COLS; j++)
        {
          // Calculate the distance
          double x    = (j - (COLS - 1) / 2.0) / (double)RATIO;
          double y    = i - (LINES - 1) / 2.0;
          double dist = sqrt (x * x + y * y);
          putchar (dist <= RADIUS ? '#' : ' ');
        }
      putchar ('\n');
    }
}
