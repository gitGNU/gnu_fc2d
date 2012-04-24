libutil=$(top_builddir)/utils/libutils.la

AM_LIBS=$(GLIB_LIBS) $(SDL_LIBS)
AM_CFLAGS=$(GLIB_CFLAGS) $(SDL_CFLAGS)
AM_CXXFLAGS=$(AM_CFLAGS)

noinst_LTLIBRARIES = libhigh-level.la

highlevelinclude_HEADERS = high-level.h fthreads.h

highlevelincludedir = $(includedir)/fgameengine-@FGE_VERSION@/high-level/

libhigh_level_la_SOURCES = \
	high-level.cpp \
	fthreads.c \
	$(highlevelinclude_HEADERS)

libhigh_level_la_LIBADD = \
	$(libutils)

libhigh_level_la_DEPENDENCIES = $(libhigh_level_la_LIBADD)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/high-level/