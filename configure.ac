# Process this file with autoconf to produce a configure script.

m4_define([fge_major_version], [1])
m4_define([fge_minor_version], [0])
m4_define([fge_micro_version], [0])

#For releases only
##################
#m4_define([fge_real_version], 
#          [fge_major_version.fge_minor_version.fge_micro_version])
#
#m4_define([fge_version], 
#          [fge_major_version.fge_minor_version])
##################

m4_define([fge_real_version], 
          [SVN])

m4_define([fge_version], 
          [SVN])

AC_INIT([FDiamondEngine], fge_real_version, [https://sourceforge.net/p/fgc/tickets/new/], [FDiamondEngine], [http://fgc.sf.net])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 no-define])

m4_define([glib_required_version], [2.16.1])
m4_define([python_required_version], [0.25.0])
m4_define([gtk_required_version], [0.25.0])
m4_define([openal_required_version], [0.25.0])
m4_define([gl_required_version], [0.25.0])
m4_define([glu_required_version], [0.25.0])
m4_define([sdl_required_version], [0.25.0])
m4_define([gthread_required_version], [0.25.0])
m4_define([x11_required_version], [0.25.0])
m4_define([libvcodec_required_version], [0.25.0])
m4_define([libvformat_required_version], [0.25.0])
m4_define([alsa_required_version], [0.25.0])

FGE_VERSION=fge_version
FGE_REAL_VERSION=fge_real_version
FGE_MAJOR_VERSION=fge_major_version
FGE_MINOR_VERSION=fge_minor_version
FGE_MICRO_VERSION=fge_micro_version

AC_SUBST(FGE_VERSION)
AC_SUBST(FGE_REAL_VERSION)
AC_SUBST(FGE_MAJOR_VERSION)
AC_SUBST(FGE_MINOR_VERSION)
AC_SUBST(FGE_MICRO_VERSION)

#check for libraries

PKG_CHECK_MODULES( ALSA, alsa >= alsa_required_version, 
				   AC_DEFINE(HAVE_ALSA, [1], [Check for ALSA]),
				   AC_DEFINE(HAVE_ALSA, [0], [Check for ALSA])
)

PKG_CHECK_MODULES( GLIB, glib-2.0 >= glib_required_version )
PKG_CHECK_MODULES( GTHREAD, gthread-2.0 >= gthread_required_version )
PKG_CHECK_MODULES( PYTHON, python >= python_required_version, 
				   AC_DEFINE(HAVE_PYTHON, [1], [Check for PYTHON]), 
				   AC_DEFINE(HAVE_PYTHON, [0], [Check for PYTHON]) )

PKG_CHECK_MODULES( GL, gl >= gl_required_version, 
				   AC_DEFINE(HAVE_GL, [1], [Check for GL]),
				   AC_DEFINE(HAVE_GL, [0], [Check for GL])
)
PKG_CHECK_MODULES( GLU, glu >= glu_required_version, AC_DEFINE(HAVE_GLU, [1], [Check for GLU]),
	AC_DEFINE(HAVE_GLU, [0], [Check for GLU])
)
PKG_CHECK_MODULES( SDL, sdl >= sdl_required_version, AC_DEFINE(HAVE_SDL, [1], [Check for SDL]),
	AC_DEFINE(HAVE_SDL, [0], [Check for SDL])
)
PKG_CHECK_MODULES( X11, x11 >= x11_required_version, AC_DEFINE(HAVE_X11, [1], [Check for X11]),
	AC_DEFINE(HAVE_X11, [0], [Check for X11])
)
PKG_CHECK_MODULES( LIBAVCODEC, libavcodec >= libavcodec_required_version, 
				   AC_DEFINE(HAVE_LIBAVCODEC, [1], [Check for LIBAVCODEC]), 
				   AC_DEFINE(HAVE_LIBAVCODEC, [0], [Check for LIBAVCODEC]) )

PKG_CHECK_MODULES( LIBAVFORMAT, libavformat >= libavformat_required_version, 
				   AC_DEFINE(HAVE_LIBAVFORMAT, [1], [Check for LIBAVFORMAT]), 
				   AC_DEFINE(HAVE_LIBAVFORMAT, [0], [Check for LIBAVFORMAT]) )

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_HEADERS([auto-config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_INSTALL

AC_CONFIG_FILES([
Makefile
bin/Makefile
doc/Makefile
net/Makefile
bin/fde-run
bin/fde-config
audio/Makefile
data/Makefile
data/images/Makefile
data/images/logos/Makefile
data/audio/music/Makefile
data/audio/Makefile
data/audio/sound/Makefile
high-level/Makefile
highest-level/Makefile
physics/Makefile
utils/Makefile
video/Makefile
fdiamondengine.pc
config.h
])

LT_INIT
AC_PROG_LIBTOOL
AC_REPLACE_FUNCS
AC_PREREQ([2.56])
AC_OUTPUT

echo "
	${PACKAGE} ${VERSION}
	~~~~~~~~~~~~~

	Configuration Info:
		prefix:                ${prefix}
		datarootdir:           ${datarootdir}
		datadir:               ${datadir}
		exec_prefix:           ${exec_prefix}
		libdir:                ${libdir}
		CFLAGS:                ${CFLAGS}
"
