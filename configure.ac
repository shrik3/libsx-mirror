# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([libsx], [2.08], [opengrads-supplibs@lists.sourceforge.net])
AC_CONFIG_AUX_DIR([m4])
AM_INIT_AUTOMAKE
AC_REVISION([$Id: configure.ac,v 1.8 2007/08/28 21:43:20 pertusus Exp $])
AC_CONFIG_HEADER([config.h])

# soname version for libtool
LIBSX_VERSION='0:0:0'
AC_SUBST(LIBSX_VERSION)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
dnl AC_PROG_RANLIB
AC_PROG_LIBTOOL

# Checks for libraries.
dnl AC_PATH_XTRA
dnl LIBS="$LIBS $X_PRE_LIBS $X_LIBS"
dnl AC_CHECK_LIB(X11, main)
dnl AC_CHECK_LIB(Xt, main)
dnl AC_CHECK_LIB(Xext, main)
dnl AC_CHECK_LIB(Xmu, main)
dnl AC_CHECK_LIB(Xaw, main)
dnl LIBS="$LIBS $X_EXTRA_LIBS"
GA_CHECK_XAW([
  LIBS="$LIBS $XAW_LIBS $XAW_XLIBS"
  LDFLAGS="$LDFLAGS $X_LIBS"
  CPPFLAGS="$CPPFLAGS $X_CFLAGS $XAW_CFLAGS"
  if test z"$ac_pkgconfig_xaw" != 'zno'; then
    pkgconfig_X_libs="Requires: $ac_pkgconfig_xaw"
  else
    pkgconfig_X_libs="Libs: $X_LIBS $XAW_LIBS $XAW_XLIBS"
  fi
  AC_SUBST(pkgconfig_X_libs)
], [AC_MSG_WARN([No Xaw implementation found])])
# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_HEADER_STDC
AC_FUNC_STRTOD
AC_CHECK_DECLS([strcasecmp, lstat, strdup])

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 src/dialogs/Makefile
		 freq/Makefile
		 libsx.pc
		])

dnl Darwin: need -fnested-functions
case $host_os in
  darwin*)
    if test "$GCC" = yes; then
      CFLAGS="$CFLAGS -fnested-functions "
    fi
    ;;
esac

AC_OUTPUT

