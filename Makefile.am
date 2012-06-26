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
    THANKS \
    NEWS \
    README \
    autogen.sh \
    TODO

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


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = fdiamondengine.pc

lib_LTLIBRARIES = libFDiamondEngine-SVN.la

fdiamondengineinclude_HEADERS = fge.h config.h auto-config.h
fdiamondengineincludedir = $(includedir)/@PACKAGE_NAME@-@FGE_VERSION@/

libFDiamondEngine_SVN_la_SOURCES = fge.cpp $(fdiamondengineinclude_HEADERS)

libFDiamondEngine_SVN_la_LIBADD = \
$(libhighlevel) \
$(libhighestlevel) \
$(libaudio) \
$(libvideo) \
$(libphysics) \
$(libutils)

DIST_SUBDIRS = \
	high-level\
	net \
	video\
	audio \
	data \
	highest-level \
	physics \
	utils \
	doc \
	bin

SUBDIRS = $(DIST_SUBDIRS)

libFDiamondEngine_SVN_la_DEPENDENCIES = $(libFDiamondEngine_SVN_la_LIBADD)

INCLUDES= \
	-I$(top_builddir)
