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