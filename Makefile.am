AUTOMAKE_OPTIONS = gnu
ACLOCAL_AMFLAGS = -I m4

libhighlevel= $(top_builddir)/high-level/libhigh-level.la
libhighestlevel= $(top_builddir)/highest-level/libhighest-level.la
libaudio = $(top_builddir)/libaudio/libaudio.la
libvideo = $(top_builddir)/libvideo/libvideo.la
libphysics = $(top_builddir)/libphysics/libphysics.la

noinst_DATA = \
    AUTHORS \
    ChangeLog \
    COPYING \
    INSTALL \
    LICENSE \
    THANKS \
    NEWS \
    README \
    autogen.sh

EXTRA_DIST = \
	$(noinst_DATA)

DISTCLEANFILES = \
	aclocal.m4 \
	compile \
    config.guess \
	config.h.in \
    config.sub \
	configure \
    ltmain.sh \
	depcomp \
    install-sh \
    missing \
    Makefile.in \
    autom4te.cache/*

DIST_SUBDIRS = \
	audio \
	data \
	high-level\
	highest-level \
	physics

SUBDIRS = $(DIST_SUBDIRS)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = fgameengine.pc

#FGameEngine - Library
#======================

#AM_CFLAGS=`cs-config --cflags`
#AM_CXXFLAGS=`cs-config --cflags`
#AM_LDFLAGS=`cs-config --libs`

lib_LTLIBRARIES = libfgameengine-@FGE_VERSION@.la

fgameengineinclude_HEADERS = fge.h config.h auto-config.h
fgameengineincludedir = $(includedir)/fgameengine-@FGE_VERSION@/

libfgameengine_1_0_la_SOURCES = $(fgameengineinclude_HEADERS) fge.cpp

libfgameengine_1_0_la_LIBADD = \
	$(libhighlevel) \
	$(libhighestlevel) \
	$(libaudio) \
	$(libvideo) \
	$(libphysics)
	

libfgameengine_1_0_la_DEPENDENCIES = $(libfgameengine_1_0_la_LIBADD)

INCLUDES= \
	-I$(top_builddir) \
	-I$(top_builddir)/high-level/ \
-	-I$(top_builddir)/highest-level/ \
-	-I$(top_builddir)/physics/ \
	-I$(top_builddir)/audio/ \
	-I$(top_builddir)/video