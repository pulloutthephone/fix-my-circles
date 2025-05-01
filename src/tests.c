#include <criterion/criterion.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
  size_t rows;
  size_t cols;
} disp_size;

double
distance (double x1, double y1, double x2, double y2)
{
  double dx = x1 - x2;
  double dy = y1 - y2;
  return sqrt (dx * dx + dy * dy);
}

char *
circle_generator (const disp_size dim, const size_t circle_radius)
{
  const size_t  ratio = dim.cols / dim.rows;
  char         *field = malloc (dim.rows * (dim.cols + 1) + 1);

  size_t ind = 0;
  for (size_t i = 0; i < dim.rows; i++)
    {
      for (size_t j = 0; j < dim.cols; j++)
        {
          double x = (j - (dim.cols - 1) / 2.0) / ratio;
          double y = i - (dim.rows - 1) / 2.0;
          double d = sqrt (x * x + y * y);
          field[ind++] = d <= circle_radius ? '#' : ' ';
        }
      field[ind++] = '\n';
    }
  return field[ind] = '\0', field;
}

void
swiss_cheese (char *field, const disp_size dim, size_t hole_count,
              size_t hole_max_radius, size_t circle_radius)
{
  const size_t ratio = dim.cols / dim.rows;

  srand (time (NULL));
  for (size_t h = 0; h < hole_count; h++)
    {
      size_t hole_radius = 1 + rand () % hole_max_radius;

      // Center of the hole
      double c_x = (rand () % dim.cols - (dim.cols - 1) / 2.0) / ratio;
      double c_y = rand () % dim.rows - (dim.rows - 1) / 2.0;

      size_t ind = 0;
      for (size_t i = 0; i < dim.rows; i++)
        {
          for (size_t j = 0; j < dim.cols; j++)
            {
              double x = (j - (dim.cols - 1) / 2.0) / ratio;
              double y = i - (dim.rows - 1) / 2.0;

              // Only remove point if inside hole and strictly inside circle
              if (distance (x, y, 0, 0) < (circle_radius - 1)
                  && distance (x, y, c_x, c_y) <= hole_radius)
                field[ind] = ' ';
              ind++;
            }
          ind++;
        }
    }
}

void
test_generator (const disp_size dim)
{
  const size_t circle_radius = 18;
  
  char *field = circle_generator (dim, circle_radius);
  swiss_cheese (field, dim, 20, circle_radius / 4, circle_radius);

  printf ("%s", field);
  free (field);
}

Test (basic, swiss_cheese_circle_example)
{
  const disp_size dim = { .rows = 40, .cols = dim.rows * 2 };
  test_generator (dim);
}
