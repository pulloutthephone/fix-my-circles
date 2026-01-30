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
} display;

display
init_display (size_t rows, size_t cols)
{
  display disp = { .rows    = rows,
                   .cols    = cols,
                   .content = malloc (rows * (cols + 1) + 1) };
  return disp;
}

void
clear_display (display *disp)
{
  char *p = disp->content;
  for (size_t i = 0; i < disp->rows; i++)
    {
      for (size_t j = 0; j < disp->cols; j++)
        *p++ = ' ';
      *p++ = '\n';
    }
  *p = '\0';
}

void
show (display *disp)
{ // Debug function.
  for (char *p = disp->content; *p; p++)
    putchar (*p);
}

void
destroy_display (display *disp)
{
  free (disp->content);
  memset (disp, 0, sizeof (display));
}

typedef long coord;

coord
clamp_coord (coord v, coord lo, coord hi)
{
  return (v < lo ? lo : (v > hi ? hi : v));
}

typedef struct
{
  coord x;
  coord y;
} point;

typedef struct
{
  point pos;
  coord radius;
  coord x_stretch;
} circle;

#define DRAW_CHEESE(disp, circ) draw_circle ((disp), (circ), '#')
#define DRAW_HOLE(disp, circ)   draw_circle ((disp), (circ), ' ')
void
draw_circle (display *disp, circle *circ, char c)
{
  coord x  = circ->pos.x;
  coord y  = circ->pos.y;
  coord r  = circ->radius;
  coord xs = circ->x_stretch;

  if (r <= 0)
    return;

  // Calculating the bounding square that encapsulates the circle.  For this
  // test case, the clamping should not be needed since the circle is supposed
  // to be inside the display.  However, for my peace of mind, I'll keep this
  // to ensure that nothing bad happens.
  size_t rows = disp->rows;
  size_t cols = disp->cols;
  coord  rx   = r * xs + 1;
  coord  sx   = clamp_coord (x - rx, 0, cols);
  coord  ex   = clamp_coord (x + rx + 1, 0, cols);
  coord  sy   = clamp_coord (y - r, 0, rows);
  coord  ey   = clamp_coord (y + r + 1, 0, rows);

  for (coord i = sy; i < ey; i++)
    {
      for (coord j = sx; j < ex; j++)
        {
          coord dx = x - j;
          coord dy = y - i;
          if (dx * dx + dy * dy * xs * xs <= r * r * xs * xs)
            disp->content[i * (cols + 1) + j] = c;
        }
    }
}

void
swiss_cheese (display *disp, circle *circ, size_t n_holes)
{
  coord x  = circ->pos.x;
  coord y  = circ->pos.y;
  coord xs = circ->x_stretch;

  double r  = circ->radius;
  double hr = r / 3;
  double s  = r / sqrt (2) - hr - 0.5;

  if (s <= 0)
    return;
  
  // Adjust the square boundaries so the holes don't leak outside the circle.
  coord sx = ceil (x - s * xs);
  coord ex = floor (x + s * xs) + 1;
  coord sy = ceil (y - s);
  coord ey = floor (y + s) + 1;

  for (size_t i = 0; i < n_holes; i++)
    {
      circle hole = { .pos.x     = (rand () % (ex - sx)) + sx,
                      .pos.y     = (rand () % (ey - sy)) + sy,
                      .radius    = hr,
                      .x_stretch = xs };
      DRAW_HOLE (disp, &hole);
    }
}

void
random_tester (display *disp)
{
  clear_display (disp);

  // Circle parameters.
  size_t cr  = (rand () % 5) + 3; // [3 - 7]
  size_t cxs = 2;
  point  cp  = { .x = (rand () % (disp->cols - 2 * cr * cxs) + cr * cxs),
                 .y = (rand () % (disp->rows - 2 * cr) + cr) };
  
  circle circ = { .pos = cp, .radius = cr, .x_stretch = cxs };
  DRAW_CHEESE (disp, &circ);

  // Save the current display content for future comparison
  // against the user's result.
  size_t len = strlen (disp->content) + 1;
  char   input[len];
  char   expected[len];
  strcpy (expected, disp->content);

  swiss_cheese (disp, &circ, 4);
  strcpy (input, disp->content);

  // Send the damaged circle to the user.
  circle_mender (disp->content);

  // Check if the returned content is correct.
  cr_assert_str_eq (expected, disp->content,
                    "You haven't fixed my circle :(\nInput:\n%s\nOutput:\n"
                    "%s\nExpected:\n%s\nCan you try again?",
                    input, disp->content, expected);
}

#define NUM_RAND_TESTS 50
Test (circle, random_tests)
{
  srand (time (0));
  display disp = init_display (20, 40);
  
  for (size_t i = 0; i < NUM_RAND_TESTS; i++)
    random_tester (&disp);
  
  destroy_display (&disp);
}

void
fix_tester (const char *input, const char *expected)
{
  char content[strlen (input) + 1];
  strcpy (content, input);

  circle_mender (content);

  // Check if the returned content is correct.
  cr_assert_str_eq (expected, content,
                    "You haven't fixed my circle :(\nInput:\n%s\nOutput:\n"
                    "%s\nExpected:\n%s\nCan you try again?",
                    input, content, expected);
}

Test (circle, example1)
{
  const char *input    = "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "             #                          \n"
                         "         #########                      \n"
                         "        #### # ####                     \n"
                         "       ###       ###                    \n"
                         "        #### # ####                     \n"
                         "         #########                      \n"
                         "             #                          \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n";

  const char *expected = "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "             #                          \n"
                         "         #########                      \n"
                         "        ###########                     \n"
                         "       #############                    \n"
                         "        ###########                     \n"
                         "         #########                      \n"
                         "             #                          \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n";

  fix_tester (input, expected);
}

Test (circle, example2)
{
  const char *input    = "                       #                \n"
                         "                ###############         \n"
                         "              ###################       \n"
                         "            #######################     \n"
                         "           ############ ### ########    \n"
                         "          ##########           ######   \n"
                         "          #########             #####   \n"
                         "         #######               #######  \n"
                         "          #####             #########   \n"
                         "          ######          ###########   \n"
                         "           ######## ## #############    \n"
                         "            #######################     \n"
                         "              ###################       \n"
                         "                ###############         \n"
                         "                       #                \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n";

  const char *expected = "                       #                \n"
                         "                ###############         \n"
                         "              ###################       \n"
                         "            #######################     \n"
                         "           #########################    \n"
                         "          ###########################   \n"
                         "          ###########################   \n"
                         "         #############################  \n"
                         "          ###########################   \n"
                         "          ###########################   \n"
                         "           #########################    \n"
                         "            #######################     \n"
                         "              ###################       \n"
                         "                ###############         \n"
                         "                       #                \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n";

  fix_tester (input, expected);
}

Test (circle, example3)
{
  const char *input    = "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "              #                         \n"
                         "        #############                   \n"
                         "      #################                 \n"
                         "    ########### # #######               \n"
                         "   #########         #####              \n"
                         "   #######            ####              \n"
                         "  #######            ######             \n"
                         "   #####          ########              \n"
                         "   ######       ##########              \n"
                         "    ######## ############               \n"
                         "      #################                 \n"
                         "        #############                   \n"
                         "              #                         \n";

  const char *expected = "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "                                        \n"
                         "              #                         \n"
                         "        #############                   \n"
                         "      #################                 \n"
                         "    #####################               \n"
                         "   #######################              \n"
                         "   #######################              \n"
                         "  #########################             \n"
                         "   #######################              \n"
                         "   #######################              \n"
                         "    #####################               \n"
                         "      #################                 \n"
                         "        #############                   \n"
                         "              #                         \n";

  fix_tester (input, expected);
}
