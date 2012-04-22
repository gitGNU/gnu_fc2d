noinst_LTLIBRARIES = libvideo.la

videoinclude_HEADERS = video.h

videoincludedir = $(includedir)/fgameengine-@FGE_VERSION@/video/

libvideo_la_SOURCES = \
	video.cpp \
	$(audioinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/video/