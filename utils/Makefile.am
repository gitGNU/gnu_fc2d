noinst_libdir=`pwd .`
noinst_lib_LTLIBRARIES = libutils.la

videoinclude_HEADERS = utils.h red-black.h

videoincludedir = $(includedir)/fgameengine-@FGE_VERSION@/utils/

libutils_la_SOURCES = \
	utils.cpp \
	red-black.c \
	$(audioinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/utils/