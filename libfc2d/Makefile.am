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

AM_LIBS= $(GLIB_LIBS)
AM_CFLAGS= $(GLIB_CFLAGS)
AM_CXXFLAGS= $(AM_CFLAGS)

libutils=$(top_builddir)/libfc2d/utils/libutils.la

lib_LTLIBRARIES = libfc2d-0.1.la

fc2dinclude_HEADERS = fc2d.h auto-config.h config.h
fc2dincludedir = $(includedir)/fc2d-0.1/

libfc2d_0_1_la_SOURCES = fc2d.c $(fc2dinclude_HEADERS)

libfc2d_0_1_la_LIBADD = \
	$(libutils)

DIST_SUBDIRS = \
	utils

SUBDIRS = $(DIST_SUBDIRS)

libfc2d_0_1_la_DEPENDENCIES = $(libfc2d_0_1_la_LIBADD)

INCLUDES= \
	-I$(top_builddir)
