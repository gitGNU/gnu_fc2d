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
