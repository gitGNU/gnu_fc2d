# Process this file with autoconf to produce a configure script.

function F_CHECK_LIB() {
  if  (test -d "/usr/include/$2")
    m4_define([fge_cflags], [/usr/include/])
  else
    if  (test -d "/usr/local/include/$2")
      m4_define([fge_cflags], [-I/usr/local/include/])
    else
      AC_MSG_ERROR([$1 header directory not found. Read INSTALL for more info.])
    fi
  fi
}

m4_define([fge_major_version], [1])
m4_define([fge_minor_version], [0])
m4_define([fge_micro_version], [0])

m4_define([fge_real_version], 
          [fge_major_version.fge_minor_version.fge_micro_version])

m4_define([fge_version], 
          [fge_major_version.fge_minor_version])

AC_INIT([FGameEngine], fge_real_version, [https://sourceforge.net/p/fgc/tickets/new/], [FGameEngine], [https://sourceforge.net/p/fgc/wiki/Home/])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 no-define])

m4_define([glib_required_version], [2.16.1])
m4_define([python_required_version], [0.25.0])
m4_define([gtk_required_version], [0.25.0])
m4_define([openal_required_version], [0.25.0])

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
PKG_CHECK_MODULES( GLIB, glib-2.0 >= glib_required_version )
PKG_CHECK_MODULES( PYTHON, python >= python_required_version )
PKG_CHECK_MODULES( OPENAL, openal >= openal_required_version )



AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_HEADERS([auto-config.h])

F_CHECK_LIB IRRLICHT irrlicht

FGE_CFLAGS=fge_cflags
FGE_LIBS="-lirrlicht -lGL -lGLU -lXrandr -lXext -lX11 -Wall"

AC_SUBST(FGE_CFLAGS)
AC_SUBST(FGE_LIBS)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_INSTALL

AC_CONFIG_FILES([
Makefile
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
fgameengine.pc
config.h
])

LT_INIT
AC_PROG_LIBTOOL
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
