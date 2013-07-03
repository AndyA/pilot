/* transform.t */

#include <math.h>

#include "framework.h"
#include "tap.h"

#include "jd_pretty.h"
#include "pilot.h"

#define EPSILON 0.001

static int eq_matrix(pi_affine *a, pi_affine *b, double eps) {
  unsigned x, y;
  for (y = 0; y <= pMAX; y++)
    for (x = 0; x <= pMAX; x++)
      if (fabs(a->m[x][y] - b->m[x][y]) >= eps)
        return 0;
  return 1;
}

static void dump_matrix(pi_affine *m) {
  unsigned x, y;
  fprintf(stderr, "\n");
  for (y = 0; y <= pMAX; y++) {
    for (x = 0; x <= pMAX; x++) {
      fprintf(stderr, "%s", x == 0 ? "# " : ", ");
      fprintf(stderr, "%5.2f", m->m[x][y]);
    }
    fprintf(stderr, "\n");
  }
}

static void test_transform(void) {
  {
    pi_affine a, b;

    pi_transform_init(&a);
    pi_transform_invert(&b, &a);

    if (!ok(eq_matrix(&a, &b, EPSILON), "inverse of identity == identity")) {
      dump_matrix(&a);
      dump_matrix(&b);
    }
  }

  {
    pi_affine a, b;

    pi_transform_init_scale(&a, 2.0);
    pi_transform_init_scale(&b, 4.0);
    pi_transform_append(&a, &a, &a);

    if (!ok(eq_matrix(&b, &a, EPSILON), "scale 2 * scale 2 == scale 4")) {
      dump_matrix(&b);
      dump_matrix(&a);
    }

    pi_transform_invert(&a, &a);
    pi_transform_init_scale(&b, 0.25);

    if (!ok(eq_matrix(&b, &a, EPSILON), "inverse of scale")) {
      dump_matrix(&b);
      dump_matrix(&a);
    }
  }
}

void test_main(void) {
  scope {
    test_transform();
  }
}

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
