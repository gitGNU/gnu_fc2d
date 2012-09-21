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

noinst_LTLIBRARIES = libutils.la

AM_LIBS=$(GLIB_LIBS)
AM_CFLAGS= $(GLIB_CFLAGS)
AM_CXXFLAGS=$(AM_CFLAGS)

utilsinclude_HEADERS = \
	utils.h \
	event-basis.h \
	data-connect.h \
	low-level.h \
	arguments.h \
	string.h

utilsincludedir = $(includedir)/$(PACKAGE_NAME)-$(FGE_VERSION)/utils/

libutils_la_SOURCES = \
	event-basis.c \
	data-connect.c \
	low-level.c \
	arguments.c \
	string.c \
	$(audioinclude_HEADERS)

INCLUDES= \
	-I$(top_builddir)
