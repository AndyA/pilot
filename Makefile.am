DIST_SUBDIRS = m4 t

# EXTRA_DIST = jd_path.h jd_utf8.h

SUBDIRS = . t

ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libpilot.la

include_HEADERS = pilot.h

bin_PROGRAMS = flyer

libpilot_la_SOURCES = pilot.c pilot.h

flyer_SOURCES = flyer.c utils.c utils.h
flyer_LDADD = libpilot.la

test: all
	cd t && $(MAKE) test

