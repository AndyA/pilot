/* flyer.c */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <bluetooth/bluetooth.h>
#include <cwiid.h>
#include <jd_pretty.h>

#include "pilot.h"
#include "utils.h"

static cwiid_mesg_callback_t cwiid_callback;

static void err_handler(cwiid_wiimote_t *wiimote, const char *s, va_list ap) {
  if (wiimote) printf("%d:", cwiid_get_id(wiimote));
  else printf("-1:");
  vprintf(s, ap);
  printf("\n");
}

static void ir_report(const struct cwiid_ir_src *ir) {
  for (unsigned i = 0; i < CWIID_IR_SRC_COUNT; i++) {
    printf("%s", i == 0 ? "\r" : ", ");
    if (ir[i].valid)
      printf("[%4d, %4d, %2d]", ir[i].pos[CWIID_X], ir[i].pos[CWIID_Y], ir[i].size);
    else
      printf("[              ]");
  }
}

static void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count,
                           union cwiid_mesg mesg[], struct timespec *timestamp) {
  (void) timestamp;
  (void) wiimote;

  for (int i = 0; i < mesg_count; i++) {
    switch (mesg[i].type) {
    case CWIID_MESG_IR:
      ir_report(&(mesg[i].ir_mesg.src[0]));
      break;
    default:
      break;
    }
  }
}

static jd_var *load_string(jd_var *out, FILE *f) {
  char buf[512];
  size_t got;

  jd_set_empty_string(out, 10000);
  while (got = fread(buf, 1, sizeof(buf), f), got)
    jd_append_bytes(out, buf, got);
  return out;
}

static jd_var *load_json(jd_var *out, FILE *f) {
  jd_var json = JD_INIT;
  jd_from_json(out, load_string(&json, f));
  jd_release(&json);
  return out;
}

static jd_var *load_config(jd_var *out, const char *file) {
  FILE *f;
  if (f = fopen(file, "r"), !f) die("Can't read %s: %m\n", file);
  load_json(out, f);
  fclose(f);
  return out;
}

static void open_worker(void *ctx) {
  (void) ctx;
}

static void runloop(void) {
  bdaddr_t bdaddr;
  cwiid_wiimote_t *wiimote;

  cwiid_set_err(err_handler);

  /*  if (argc > 1)*/
  /*    str2ba(argv[1], &bdaddr);*/
  /*  else*/
  /*    bdaddr = *BDADDR_ANY;*/

  printf("Press 1+2 on Wiimote\n");

  if (!(wiimote = cwiid_open(&bdaddr, 0)))
    die("Can't connect to wiimote");

  if (cwiid_set_mesg_callback(wiimote, cwiid_callback))
    die("Can't set callback");

  if (cwiid_enable(wiimote, CWIID_FLAG_MESG_IFC))
    die("Can't enable messages");

  if (cwiid_set_rpt_mode(wiimote, CWIID_RPT_IR))
    die("Can't set report mode");

  getchar();

  if (cwiid_close(wiimote))
    die("Error on disconnect");
}

int main(int argc, char *argv[]) {
  if (argc != 2) die("Syntax: flyer <config.json>");
  scope {
    jd_var *config = load_config(jd_nv(), argv[1]);
    jd_printf("%lJ\n", config);
  }

  return 0;
}

