libutil=$(top_builddir)/utils/libutils.la

AM_LIBS=$(GLIB_LIBS) $(GTHREAD_LIBS)
AM_CFLAGS=$(GLIB_CFLAGS) $(GTHREAD_CFLAGS)
AM_CXXFLAGS=$(AM_CFLAGS)

noinst_LTLIBRARIES = libhigh-level.la

highlevelinclude_HEADERS = \
	high-level.h \
	threads.h

highlevelincludedir = \
	$(includedir)/@PACKAGE_NAME@-@FGE_VERSION@/high-level/

libhigh_level_la_SOURCES = \
	threads.c \
	$(highlevelinclude_HEADERS)

libhigh_level_la_LIBADD = \
	$(libutils)

libhigh_level_la_DEPENDENCIES = $(libhigh_level_la_LIBADD)

INCLUDES= \
	-I$(top_builddir)/