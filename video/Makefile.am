noinst_LTLIBRARIES = libvideo.la

AM_LIBS=$(GLIB_LIBS)
AM_CFLAGS=$(GLIB_CFLAGS)

AM_LIBS+= $(SDL_LIBS)
AM_CFLAGS+= $(SDL_CFLAGS)

AM_LIBS+= $(X11_LIBS)
AM_CFLAGS+= $(X11_CFLAGS)

AM_CXXFLAGS= $(AM_CFLAGS)

videoinclude_HEADERS = \
	video.h \
	window.h \
	render.h \
	widgets.h \
	image.h \
	mesh.h \
	videofile.h

videoincludedir = $(includedir)/@PACKAGE_NAME@-@FGE_VERSION@/video/

libvideo_la_SOURCES = \
	video.cpp \
	window.c\
	render.c\
	image.c \
	mesh.c \
	videofile.c
	$(audioinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/video/