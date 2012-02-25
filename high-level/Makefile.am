noinst_libdir=$(libdir)

noinst_lib_LTLIBRARIES = libhigh-level.la

highlevelinclude_HEADERS = high-level.h fthreads.h

highlevelincludedir = $(includedir)/fgameengine-@FGE_VERSION@/high-level/

libhigh_level_la_SOURCES = \
	high-level.cpp \
	fthreads.c \
	$(highlevelinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/high-level/