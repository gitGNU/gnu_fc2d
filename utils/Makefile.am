noinst_LTLIBRARIES = libutils.la

AM_LIBS=$(GLIB_LIBS)
AM_CFLAGS=$(GLIB_CFLAGS)

AM_LIBS+= $(SDL_LIBS)
AM_CFLAGS+= $(SDL_CFLAGS)

AM_LIBS+= $(ALSA_LIBS)
AM_CFLAGS+= $(ALSA_CFLAGS)

AM_CXXFLAGS=$(AM_CFLAGS)



utilsinclude_HEADERS = \
	utils.h \
	red-black.h \
	event-basis.h \
	Vector2.h\
	Vector3.h \
	events.h \
	data-connect.h

utilsincludedir = $(includedir)/@PACKAGE_NAME@-@FGE_VERSION@/utils/

libutils_la_SOURCES = \
	utils.cpp \
	event-basis.c \
	red-black.c \
	Vector2.c \
	Vector3.cpp \
	events.c \
	data-connect.c\
	$(audioinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/utils/