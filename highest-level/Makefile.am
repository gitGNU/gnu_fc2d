noinst_lib_LTLIBRARIES = libhighest-level.la

highestinclude_HEADERS =  highest-level.h

highestlevelincludedir = $(includedir)/fgameengine-@FGE_REAL_VERSION@/highest-level/

libhighest_level_la_SOURCES = \
	highest-level.cpp \
	$(highestinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/highest-level/