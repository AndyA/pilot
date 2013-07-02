TESTBIN = basic camera transform

TESTPERL = 

# EXTRA_DIST = commands.json

noinst_PROGRAMS = wrap $(TESTBIN)

noinst_LTLIBRARIES = libtest-support.la

libtest_support_la_SOURCES = tap.c tap.h framework.c framework.h jd_test.c jd_test.h

LDADD = libtest-support.la ../libpilot.la

wrap_LDADD =
wrap_LDFLAGS =
wrap_CFLAGS =

test: $(TESTBIN)
	prove -e ./wrap $(TESTBIN) $(TESTPERL)

valgrind: $(TESTBIN)
	prove -e 'valgrind -q' $(addprefix ./,$(TESTBIN))
