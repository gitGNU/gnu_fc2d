noinst_LTLIBRARIES = libutils.la

AM_LIBS=$(GLIB_LIBS)
AM_CFLAGS=$(GLIB_CFLAGS)

AM_LIBS+= $(SDL_LIBS)
AM_CFLAGS+= $(SDL_CFLAGS)

AM_LIBS+= $(X11_LIBS)
AM_CFLAGS+= $(X11_CFLAGS)

AM_CXXFLAGS=$(AM_CFLAGS)



utilsinclude_HEADERS = \
	utils.h \
	red-black.h \
	event-basis.h \
	fVector3.h \
	fevents.h

utilsincludedir = $(includedir)/fgameengine-@FGE_VERSION@/utils/

libutils_la_SOURCES = \
	utils.cpp \
	event-basis.c \
	red-black.c \
	fVector3.cpp \
	fevents.c \
	$(audioinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/utils/