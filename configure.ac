dnl Process this file with autoconf to produce a configure script.

## configure autoconf
AC_PREREQ([2.68])
AC_INIT([libfcd], [0.0], [justin.r.cutler@gmail.com])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([fcd.c])
AC_CONFIG_HEADERS([config.h])

## configure automake
AM_INIT_AUTOMAKE([1.10 foreign -Wall -Werror])
# enable silent rules (if available)
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

## check for programs
AC_PROG_CC
LT_INIT

## configure libtool versioning
AC_SUBST(LT_CURRENT, 0)
AC_SUBST(LT_REVISION, 0)
AC_SUBST(LT_AGE, 0)

## check for libraries

## checks for header files

## check for typedefs, structures, and compiler characteristics

## check for library functions

## generate output
AC_SUBST([ac_aux_dir])
AC_CONFIG_FILES([\
  Makefile \
])
AC_OUTPUT
