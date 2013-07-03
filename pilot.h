/* pilot.h */

#ifndef PILOT_H_
#define PILOT_H_

enum { pX, pY, pZ, pMAX };

typedef struct {
  double p[pMAX];
} pi_point;

typedef struct {
  pi_point a, b;
} pi_vector;

typedef struct {
  double m[pMAX + 1][pMAX + 1];
} pi_affine;

typedef struct {
  pi_vector primary; /* the wiimote's primary (long axis) */
  pi_vector normal;  /* normal to the camera's horiz plane */
} pi_camera;

/* pi_transform */

pi_affine *pi_transform_init(pi_affine *m);
pi_affine * pi_transform_init_scale(pi_affine *m, double scale);
pi_affine *pi_transform_append(pi_affine *out, const pi_affine *a, const pi_affine *b);
pi_affine *pi_transform_invert(pi_affine *out, const pi_affine *m);
pi_point *pi_transform_apply(pi_point *out, const pi_point *p, const pi_affine *m);

#endif

/* vim:ts=2:sw=2:sts=2:et:ft=c
 */
