noinst_libdir=$(libdir)
noinst_lib_LTLIBRARIES = libhighest-level.la

highestlevelinclude_HEADERS =  highest-level.h

highestlevelincludedir = $(includedir)/fgameengine-@FGE_VERSION@/highest-level/

libhighest_level_la_SOURCES = \
	highest-level.cpp \
	$(highestlevelinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/highest-level/