# Process this file with autoconf to produce a configure script.

m4_define([fge_major_version], [0])
m4_define([fge_minor_version], [1])
m4_define([fge_micro_version], [01])

m4_define([fge_real_version], 
          [fge_major_version.fge_minor_version.fge_micro_version])

m4_define([fge_version], 
          [fge_major_version.fge_minor_version])

m4_define([fge_make_version], 
          [0_1])

AC_INIT([fc2d], fge_real_version, [https://sourceforge.net/p/fgc/tickets/new/], [fc2d], [http://fgc.sf.net])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 no-define])

AC_CONFIG_MACRO_DIR([m4])

m4_define([glib_required_version], [2.16.1])
m4_define([gthread_required_version], [2.30.0])

FGE_VERSION=fge_version
FGE_REAL_VERSION=fge_real_version
FGE_MAJOR_VERSION=fge_major_version
FGE_MINOR_VERSION=fge_minor_version
FGE_MICRO_VERSION=fge_micro_version
FGE_MAKE_VERSION=fge_make_version

AC_SUBST(FGE_VERSION)
AC_SUBST(FGE_MAKE_VERSION)
AC_SUBST(FGE_REAL_VERSION)
AC_SUBST(FGE_MAJOR_VERSION)
AC_SUBST(FGE_MINOR_VERSION)
AC_SUBST(FGE_MICRO_VERSION)

#check for libraries

PKG_CHECK_MODULES( GLIB, glib-2.0 >= glib_required_version )
PKG_CHECK_MODULES( GTHREAD, gthread-2.0 >= gthread_required_version,
                   AC_DEFINE(HAVE_THREAD, [1], [Is 1 if have thread support 0 otherwise]), 
                   AC_DEFINE(HAVE_THREAD, [0], [Is 1 if have thread support 0 otherwise])
                 )

AC_HEADER_TIME

AC_CONFIG_HEADERS([libfc2d/auto-config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_INSTALL

#Code copied from GNU FDISK "configure.ac"
AC_MSG_CHECKING([how to create links])
rm -f test00 test01
touch test00
ln -s test00 test01 || :
SYMLINK=""
SYMLINKFROM="fdisk"
if test -L test01; then
    SYMLINK="ln -sf"
else
    SYMLINKFROM="${sbindir}/fdisk"
    rm -f test01
    ln test00 test01 || :
    if test -e test01; then
        SYMLINK="ln"
    else
        SYMLINK="cp"
    fi
fi
AC_MSG_RESULT($SYMLINK)
AC_SUBST(SYMLINK)
AC_SUBST(SYMLINKFROM)
rm -f test00 test01

AC_CONFIG_FILES([
Makefile
doc/Makefile
bin/fc2d-run
bin/fc2d-config
data/modules/Makefile
data/Makefile
fc2d.pc
libfc2d/config.h
libfc2d/high-level/Makefile
libfc2d/utils/Makefile
bin/Makefile
libfc2d/Makefile
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
