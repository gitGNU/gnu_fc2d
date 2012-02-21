# Process this file with autoconf to produce a configure script.

m4_define([fge_major_version], [1])
m4_define([fge_minor_version], [0])
m4_define([fge_micro_version], [0])

m4_define([fge_real_version], 
          [fge_major_version.fge_minor_version.fge_micro_version])

m4_define([fge_version], 
          [fge_major_version.fge_minor_version])

AC_INIT([FGameEngine], fge_real_version, [https://sourceforge.net/apps/trac/fge/newticket], [FGameEngine], [http://fgc.sf.net])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 no-define])

m4_define([glib_required_version], [2.16.1])
m4_define([python_required_version], [0.25.0])

FGE_VERSION=fge_version
FGE_REAL_VERSION=fge_real_version
FGE_MAJOR_VERSION=fge_major_version
FGE_MINOR_VERSION=fge_minor_version
FGE_MICRO_VERSION=fge_micro_version

FGE_DATA=$datadir/fge-$FGE_REAL_VERSION/

AC_SUBST(FGE_VERSION)
AC_SUBST(FGE_REAL_VERSION)
AC_SUBST(FGE_MAJOR_VERSION)
AC_SUBST(FGE_MINOR_VERSION)
AC_SUBST(FGE_MICRO_VERSION)

#check for libraries
PKG_CHECK_MODULES( GLIB, glib-2.0 >= glib_required_version )
PKG_CHECK_MODULES( PYTHON, python >= python_required_version )

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_HEADERS([auto-config.h])

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
high-level/Makefile
highest-level/Makefile
physics/Makefile
fgameengine.pc
config.h
])
LT_INIT
AC_PROG_LIBTOOL

AC_OUTPUT

echo "
	${PACKAGE} ${VERSION}
	~~~~~~~~~~~~~

	Configuration Info:
		prefix:                ${prefix}
		datarootdir:           ${datarootdir}
		datadir:               ${datadir}
		fge_data:             ${FGE_DATA}
		exec_prefix:           ${exec_prefix}
		libdir:                ${libdir}
		CFLAGS:                ${CFLAGS}
"
