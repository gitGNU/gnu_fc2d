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

libfc2d=$(top_builddir)/libfc2d/libfc2d-0.1.la

# what flags you want to pass to the C compiler & linker

AM_LDFLAGS= $(GLIB_LIBS) $(GTHREAD_LIBS)
AM_CFLAGS= $(GLIB_CFLAGS) $(GTHREAD_CFLAGS)
AM_CXXFLAGS= $(AM_CFLAGS)

bin_PROGRAMS = fc2d-0.1

fc2d_0_1_SOURCES = \
	fc2d.c fc2d.h

fc2d_0_1_LDADD = $(libfc2d)
fc2d_0_1_DEPENDENCIES = $(libfc2d)

INCLUDES = \
	-I$(top_builddir)

bin_SCRIPTS=\
	fc2d-config\
	fc2d-run

install-exec-local: 
	@SYMLINK@ $(DESTDIR)/$(bindir)/fc2d-0.1 $(DESTDIR)/$(bindir)/fc2d

install-exec-am: install-binPROGRAMS install-exec-local