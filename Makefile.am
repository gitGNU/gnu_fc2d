AUTOMAKE_OPTIONS = gnu
ACLOCAL_AMFLAGS = -I m4

noinst_DATA = \
    AUTHORS \
    COPYING \
    INSTALL \
    README \
    NEWS \
    THANKS \
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


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = fc2d.pc

DIST_SUBDIRS = \
	libfc2d\
	data \
	doc \
	bin

SUBDIRS = $(DIST_SUBDIRS)