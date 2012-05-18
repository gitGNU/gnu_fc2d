AUTOMAKE_OPTIONS = gnu
ACLOCAL_AMFLAGS = -I m4

libhighlevel=$(top_builddir)/high-level/libhigh-level.la
libhighestlevel=$(top_builddir)/highest-level/libhighest-level.la
libaudio=$(top_builddir)/audio/libaudio.la
libvideo=$(top_builddir)/video/libvideo.la
libphysics=$(top_builddir)/physics/libphysics.la
libutils=$(top_builddir)/utils/libutils.la

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
	high-level\
	net \
	video\
	audio \
	data \
	highest-level \
	physics \
	utils \
	bin \
	doc

SUBDIRS = $(DIST_SUBDIRS)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = fgameengine.pc

#FGameEngine - Library
#======================

#AM_CFLAGS=`cs-config --cflags`
#AM_CXXFLAGS=`cs-config --cflags`
#AM_LDFLAGS=`cs-config --libs`

lib_LTLIBRARIES = libfgameengine-1.0.la

fgameengineinclude_HEADERS = fge.h config.h auto-config.h
fgameengineincludedir = $(includedir)/fgameengine-@FGE_VERSION@/

libfgameengine_1_0_la_SOURCES = fge.cpp $(fgameengineinclude_HEADERS)

libfgameengine_1_0_la_LIBADD = \
$(libhighlevel) \
$(libhighestlevel) \
$(libaudio) \
$(libvideo) \
$(libphysics) \
$(libutils)
	

libfgameengine_1_0_la_DEPENDENCIES = $(libfgameengine_1_0_la_LIBADD)

INCLUDES= \
	-I$(top_builddir)