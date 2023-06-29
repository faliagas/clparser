# configure.ac - autoconf script.
# Copyright (C) 2017-23 apostol <apostol.faliagas@gmail.com>
# Process this file with autoconf to produce a configure script.
# Refs.
# [1] www.gnu.org/savannah-checkouts/gnu/automake/manual/html_node/Local-Macros.html
# [2] www.gnu.org/software/autoconf-archive/ax_cxx_compile_stdcxx.html
# Rev. Tue 27 Jun 2023 12:15:50 PM EEST

AC_INIT([clparser], [1.0.1], [apostol.faliagas@gmail.com], [clparser])
AM_INIT_AUTOMAKE([subdir-objects -Wall])
AC_CONFIG_MACRO_DIRS([m4])
AC_PROG_CXX
AM_PROG_AR
AX_CXX_COMPILE_STDCXX([11])
LT_INIT

AC_ARG_ENABLE([examples],
[AS_HELP_STRING([--enable-examples], [compile examples (yes/no)])],
[case "${enableval}" in
  yes) examples=true ;;
  no)  examples=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-examples]) ;;
esac], [examples=false])
AM_CONDITIONAL([COMPILE_EXAMPLES], [test x$examples = xtrue])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([compile-clparser-examples], [chmod +x compile-clparser-examples])

AC_OUTPUT
