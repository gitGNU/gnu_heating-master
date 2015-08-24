AC_INIT([heating-master], [1.0.1], [David.Rabel@NoreSoft.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_CONFIG_MACRO_DIR([m4macros])

AC_PROG_CC
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11
PKG_CHECK_MODULES([GTKMM], [gtkmm-3.0])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
