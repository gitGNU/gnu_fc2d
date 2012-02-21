noinst_lib_LTLIBRARIES = libhigh-level.la

highlevelinclude_HEADERS = high-level.h

highlevelincludedir = $(includedir)/fgameengine-@FGE_REAL_VERSION@/high-level/

libhigh_level_la_SOURCES = \
	high-level.cpp \
	$(highlevelinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/high-level/