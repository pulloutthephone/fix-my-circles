#include <criterion/criterion.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void circle_mender (char *content);

typedef struct
{
  size_t rows;
  size_t cols;
  char *content;
} display_t;

display_t
init_display (size_t rows, size_t cols)
{
  display_t display = { .rows    = rows,
                        .cols    = cols,
                        .content = malloc (rows * (cols + 1) + 1) };
  return display;
}

void
clear_display (display_t *display)
{
  char *p = display->content;
  for (size_t i = 0; i < display->rows; i++)
    {
      for (size_t j = 0; j < display->cols; j++)
        *p++ = ' ';
      *p++ = '\n';
    }
  *p = '\0';
}

void
show (display_t *display)
{
  for (char *p = display->content; *p; p++)
    putchar (*p);
}

void
destroy_display (display_t *display)
{
  free (display->content);
  memset (display, 0, sizeof (display_t));
}
typedef long coord_t;

coord_t
clamp_coord (coord_t v, coord_t lo, coord_t hi)
{
  return (v < lo ? lo : (v > hi ? hi : v));
}

typedef struct
{
  coord_t x;
  coord_t y;
} point_t;

typedef struct
{
  point_t pos;
  coord_t radius;
  coord_t x_stretch;
} circle_t;

#define DRAW_CHEESE(display, circle) draw_circle ((display), (circle), '#')
#define DRAW_HOLE(display, circle)   draw_circle ((display), (circle), ' ')
void
draw_circle (display_t *display, circle_t *circle, char c)
{
  coord_t x  = circle->pos.x;
  coord_t y  = circle->pos.y;
  coord_t r  = circle->radius;
  coord_t xs = circle->x_stretch;

  // Calculating the bounding square that encapsulates the circle.  For this
  // test case, the clamping should not be needed since the circle is supposed
  // to be inside the display.  However, for my peace of mind, I'll keep this
  // to ensure that nothing bad happens.
  coord_t rx = r * xs + 1;
  coord_t sx = clamp_coord (x - rx, 0, display->cols);
  coord_t ex = clamp_coord (x + rx + 1, 0, display->cols);
  coord_t sy = clamp_coord (y - r, 0, display->rows);
  coord_t ey = clamp_coord (y + r + 1, 0, display->rows);

  for (coord_t i = sy; i < ey; i++)
    for (coord_t j = sx; j < ex; j++)
      {
        coord_t dx = x - j;
        coord_t dy = y - i;
        if ((dx * dx / (xs * xs) + dy * dy) <= r * r)
          display->content[i * (display->cols + 1) + j] = c;
      }
}

void
swiss_cheese (display_t *display, circle_t *circle, size_t n_holes)
{
  coord_t x  = circle->pos.x;
  coord_t y  = circle->pos.y;
  coord_t xs = circle->x_stretch;

  // Compute parameters of square that fits inside the circle.
  double r    = circle->radius;
  double s    = r / sqrt (2) - 1;
  double hr   = r / 4;
  double xoff = hr * xs;
  double yoff = hr;
  
  // Adjust the square boundaries so the holes don't leak outside the circle.
  coord_t sx = floor (x - s * xs + xoff);
  coord_t ex = ceil (x + s * xs - xoff);
  coord_t sy = floor (y - s + yoff);
  coord_t ey = ceil (y + s - yoff);

  for (size_t i = 0; i < n_holes; i++)
    {
      circle_t hole = { .pos.x     = (rand () % (ex - sx + 1)) + sx,
                        .pos.y     = (rand () % (ey - sy + 1)) + sy,
                        .radius    = hr,
                        .x_stretch = xs };
      DRAW_HOLE (display, &hole);
    }
}

void
tester (display_t *display)
{
  clear_display (display);

  // Circle parameters.
  size_t  cr  = (rand () % 5) + 4; // [4 - 8]
  size_t  cxs = 2;
  point_t cp  = { .x = (rand () % (display->cols - 2 * cr * cxs) + cr * cxs),
                  .y = (rand () % (display->rows - 2 * cr) + cr) };
  
  circle_t circle = { .pos = cp, .radius = cr, .x_stretch = cxs };
  DRAW_CHEESE (display, &circle);

  // Save the current display content pointer and copy it for future comparison
  // against the user's result.
  char *org_content_ptr = display->content;
  char *expected        = malloc (strlen (display->content) + 1);
  strcpy (expected, display->content);

  swiss_cheese (display, &circle, 4);
  show (display);

  // Send the damaged circle to the user.
  circle_mender (display->content);
  show (display);

  // Check if the pointer returned by the user is still pointing to the same
  // block of memory.
  cr_assert_eq (org_content_ptr, display->content,
                "Pointer is not longer pointing to the same memory");
  // Check if the returned content is correct.
  cr_assert_str_eq (display->content, expected,
                    "You haven't fixed my circle :(\nCan you try again?");

  free (expected);
}

#define NUM_RAND_TESTS 1000
Test (circle, random_tests)
{
  srand (time (0));
  display_t display = init_display (20, 40);
  
  for (size_t i = 0; i < NUM_RAND_TESTS; i++)
    tester (&display);
  
  destroy_display (&display);
}
