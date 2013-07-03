/* pi_transform.c */

#include <stdio.h>

#include "pilot.h"

pi_affine *pi_transform_init_scale(pi_affine *m, double scale) {
  unsigned x, y;
  for (y = 0; y <= pMAX; y++)
    for (x = 0; x <= pMAX; x++)
      m->m[x][y] = (x == y) ? scale : 0.0;
  return m;
}

pi_affine *pi_transform_init(pi_affine *m) {
  return pi_transform_init_scale(m, 1.0);
}

pi_affine *pi_transform_append(pi_affine *out, const pi_affine *a, const pi_affine *b) {
  unsigned x, y, r;
  pi_affine tmp;

  for (y = 0; y <= pMAX; y++) {
    for (x = 0; x <= pMAX; x++) {
      tmp.m[x][y] = 0;
      for (r = 0; r <= pMAX; r++)
        tmp.m[x][y] += a->m[r][y] * b->m[x][r];
    }
  }

  *out = tmp;
  return out;
}

pi_affine *pi_transform_invert(pi_affine *out, const pi_affine *m) {
  /* http://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform */

  pi_affine tmp;

  double s0 = m->m[0][0] * m->m[1][1] - m->m[1][0] * m->m[0][1];
  double s1 = m->m[0][0] * m->m[1][2] - m->m[1][0] * m->m[0][2];
  double s2 = m->m[0][0] * m->m[1][3] - m->m[1][0] * m->m[0][3];
  double s3 = m->m[0][1] * m->m[1][2] - m->m[1][1] * m->m[0][2];
  double s4 = m->m[0][1] * m->m[1][3] - m->m[1][1] * m->m[0][3];
  double s5 = m->m[0][2] * m->m[1][3] - m->m[1][2] * m->m[0][3];

  double c5 = m->m[2][2] * m->m[3][3] - m->m[3][2] * m->m[2][3];
  double c4 = m->m[2][1] * m->m[3][3] - m->m[3][1] * m->m[2][3];
  double c3 = m->m[2][1] * m->m[3][2] - m->m[3][1] * m->m[2][2];
  double c2 = m->m[2][0] * m->m[3][3] - m->m[3][0] * m->m[2][3];
  double c1 = m->m[2][0] * m->m[3][2] - m->m[3][0] * m->m[2][2];
  double c0 = m->m[2][0] * m->m[3][1] - m->m[3][0] * m->m[2][1];

  double det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
  if (det == 0) return NULL;

  double id = 1.0 / det;

  tmp.m[0][0] = (m->m[1][1] * c5 - m->m[1][2] * c4 + m->m[1][3] * c3) * id;
  tmp.m[0][1] = (-m->m[0][1] * c5 + m->m[0][2] * c4 - m->m[0][3] * c3) * id;
  tmp.m[0][2] = (m->m[3][1] * s5 - m->m[3][2] * s4 + m->m[3][3] * s3) * id;
  tmp.m[0][3] = (-m->m[2][1] * s5 + m->m[2][2] * s4 - m->m[2][3] * s3) * id;

  tmp.m[1][0] = (-m->m[1][0] * c5 + m->m[1][2] * c2 - m->m[1][3] * c1) * id;
  tmp.m[1][1] = (m->m[0][0] * c5 - m->m[0][2] * c2 + m->m[0][3] * c1) * id;
  tmp.m[1][2] = (-m->m[3][0] * s5 + m->m[3][2] * s2 - m->m[3][3] * s1) * id;
  tmp.m[1][3] = (m->m[2][0] * s5 - m->m[2][2] * s2 + m->m[2][3] * s1) * id;

  tmp.m[2][0] = (m->m[1][0] * c4 - m->m[1][1] * c2 + m->m[1][3] * c0) * id;
  tmp.m[2][1] = (-m->m[0][0] * c4 + m->m[0][1] * c2 - m->m[0][3] * c0) * id;
  tmp.m[2][2] = (m->m[3][0] * s4 - m->m[3][1] * s2 + m->m[3][3] * s0) * id;
  tmp.m[2][3] = (-m->m[2][0] * s4 + m->m[2][1] * s2 - m->m[2][3] * s0) * id;

  tmp.m[3][0] = (-m->m[1][0] * c3 + m->m[1][1] * c1 - m->m[1][2] * c0) * id;
  tmp.m[3][1] = (m->m[0][0] * c3 - m->m[0][1] * c1 + m->m[0][2] * c0) * id;
  tmp.m[3][2] = (-m->m[3][0] * s3 + m->m[3][1] * s1 - m->m[3][2] * s0) * id;
  tmp.m[3][3] = (m->m[2][0] * s3 - m->m[2][1] * s1 + m->m[2][2] * s0) * id;

  *out = tmp;
  return out;
}

pi_point *pi_transform_apply(pi_point *out, const pi_point *p, const pi_affine *m) {
  unsigned x, y;

  for (y = 0; y < pMAX; y++) {
    out->p[y] = m->m[pMAX][y];
    for (x = 0; x < pMAX; x++)
      out->p[y] += p->p[x] * m->m[x][y];
  }
  return out;
}

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
