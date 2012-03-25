noinst_libdir=$(libdir)
noinst_lib_LTLIBRARIES = libaudio.la

audioincludedir = $(includedir)/fgameengine-@FGE_VERSION@/audio/

audioinclude_HEADERS = \
	audio.h \
	alsa.h

libaudio_la_SOURCES = \
	audio.cpp \
	alsa.c \
	$(audioinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/audio/