# GNU FC2D - A two time dimensional programing language.
# Copyright (C) 2012  Fabio J. Gonzalez <gonzalfj@ymail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

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