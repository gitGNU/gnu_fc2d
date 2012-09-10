AUTOMAKE_OPTIONS = gnu
ACLOCAL_AMFLAGS = -I m4

AM_LIBS= $(GLIB_LIBS)
AM_CFLAGS= $(GLIB_CFLAGS)
AM_CXXFLAGS= $(AM_CFLAGS)

libutils=$(top_builddir)/libfc2d/utils/libutils.la

lib_LTLIBRARIES = libfc2d-0.1.la

fc2dinclude_HEADERS = fc2d.h auto-config.h config.h
fc2dincludedir = $(includedir)/fc2d-0.1/

libfc2d_0_1_la_SOURCES = fc2d.c $(fc2dinclude_HEADERS)

libfc2d_0_1_la_LIBADD = \
	$(libutils)

DIST_SUBDIRS = \
	utils

SUBDIRS = $(DIST_SUBDIRS)

libfc2d_0_1_la_DEPENDENCIES = $(libfc2d_0_1_la_LIBADD)

INCLUDES= \
	-I$(top_builddir)
