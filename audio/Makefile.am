noinst_LTLIBRARIES = libaudio.la

AM_LIBS=$(GLIB_LIBS)
AM_CFLAGS=$(GLIB_CFLAGS)
AM_CXXFLAGS=$(AM_CFLAGS)

audioincludedir = $(includedir)/@PACKAGE_NAME@-@FGE_VERSION@/audio/

audioinclude_HEADERS = \
	audio.h \
	alsa.h \
	basic.h

libaudio_la_SOURCES = \
	audio.cpp \
	alsa.c \
	basic.c \
	$(audioinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/audio/