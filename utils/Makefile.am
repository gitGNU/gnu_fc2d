noinst_LTLIBRARIES = libutils.la

AM_LIBS=$(GLIB_LIBS)
AM_CFLAGS=$(GLIB_CFLAGS)
AM_CXXFLAGS=$(AM_CFLAGS)

utilsinclude_HEADERS = utils.h red-black.h event-basis.h

utilsincludedir = $(includedir)/fgameengine-@FGE_VERSION@/utils/

libutils_la_SOURCES = \
	utils.cpp \
	event-basis.c \
	red-black.c \
	$(audioinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/utils/