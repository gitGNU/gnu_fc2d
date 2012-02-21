libdir = $(libdir)/fgameengine-$(FGE_VERSION)/

lib_LTLIBRARIES = libaudio.la

audioinclude_HEADERS = audio.h

audioincludedir = $(includedir)/fgameengine-@FGE_REAL_VERSION@/audio/
libaudio_la_SOURCES = \
	audio.cpp \
	$(audioinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/audio/