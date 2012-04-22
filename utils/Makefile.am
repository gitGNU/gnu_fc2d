noinst_libdir=$(libdir)
noinst_lib_LTLIBRARIES = libutils.la

utilsinclude_HEADERS = utils.h red-black.h

utilsincludedir = $(includedir)/fgameengine-@FGE_VERSION@/utils/

libutils_la_SOURCES = \
	utils.cpp \
	red-black.c \
	$(audioinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/utils/