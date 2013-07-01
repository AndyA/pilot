/* flyer.c */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <bluetooth/bluetooth.h>
#include <cwiid.h>

static cwiid_mesg_callback_t cwiid_callback;

/*cwiid_err_t err;*/

static void err_handler(cwiid_wiimote_t *wiimote, const char *s, va_list ap) {
  if (wiimote) printf("%d:", cwiid_get_id(wiimote));
  else printf("-1:");
  vprintf(s, ap);
  printf("\n");
}

static void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count,
                           union cwiid_mesg mesg[], struct timespec *timestamp) {
  int i, j;
  int valid_source;
  (void) timestamp;
  (void) wiimote;

  for (i = 0; i < mesg_count; i++) {
    switch (mesg[i].type) {
    case CWIID_MESG_STATUS:
    case CWIID_MESG_IR:
      printf("IR Report: ");
      valid_source = 0;
      for (j = 0; j < CWIID_IR_SRC_COUNT; j++) {
        if (mesg[i].ir_mesg.src[j].valid) {
          valid_source = 1;
          printf("(%d,%d) ", mesg[i].ir_mesg.src[j].pos[CWIID_X],
                 mesg[i].ir_mesg.src[j].pos[CWIID_Y]);
        }
      }
      if (!valid_source) {
        printf("no sources detected");
      }
      printf("\n");
      break;
    default:
      printf("Unknown Report");
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  cwiid_wiimote_t *wiimote; /* wiimote handle */
  bdaddr_t bdaddr;  /* bluetooth device address */
  int exit = 0;

  cwiid_set_err(err_handler);

  /* Connect to address given on command-line, if present */
  if (argc > 1) {
    str2ba(argv[1], &bdaddr);
  }
  else {
    bdaddr = *BDADDR_ANY;
  }

  /* Connect to the wiimote */
  printf("Put Wiimote in discoverable mode now (press 1+2)...\n");

  if (!(wiimote = cwiid_open(&bdaddr, 0))) {
    fprintf(stderr, "Unable to connect to wiimote\n");
    return -1;
  }

  if (cwiid_set_mesg_callback(wiimote, cwiid_callback)) {
    fprintf(stderr, "Unable to set message callback\n");
  }

  if (cwiid_enable(wiimote, CWIID_FLAG_MESG_IFC)) {
    fprintf(stderr, "Error enabling messages\n");
  }

  if (cwiid_set_rpt_mode(wiimote, CWIID_RPT_IR)) {
    fprintf(stderr, "Error setting report mode\n");
  }

  while (!exit) {
    switch (getchar()) {
    default:
      fprintf(stderr, "invalid option\n");
    }
  }

  if (cwiid_close(wiimote)) {
    fprintf(stderr, "Error on wiimote disconnect\n");
    return -1;
  }

  return 0;
}

