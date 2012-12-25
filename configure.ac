dnl Process this file with autoconf to produce a configure script.

# Copyright (C) 2012 Justin R. Cutler
#
# libfcd is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libfcd is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with libfcd.  If not, see <http://www.gnu.org/licenses/>.

## configure autoconf
AC_PREREQ([2.68])
AC_INIT([libfcd], [0.0], [justin.r.cutler@gmail.com])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([lib/fcd_common.c])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_HOST

## configure automake
AM_INIT_AUTOMAKE([1.10 foreign -Wall -Werror])
# enable silent rules (if available)
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
# handle host-specific details
MACOS="no"
WINDOWS="no"
LIBUSB="no"
AS_CASE([$host],
  [*-*-linux*],   [LIBUSB="yes"],
  [*-*-freebsd*], [LIBUSB="yes"],
  [*-*-darwin*],  [MACOSX="yes"],
  [*-*-win*],     [WINDOWS="yes" ; ac_cv_func_malloc_0_nonnull=yes],
  [*-*-mingw*],   [WINDOWS="yes" ; ac_cv_func_malloc_0_nonnull=yes],
  []
)
AM_CONDITIONAL([LIBUSB], [ test "x$LIBUSB" = "xyes" ])
AM_CONDITIONAL([MACOSX], [ test "x$MACOSX" = "xyes" ])
AM_CONDITIONAL([WINDOWS], [ test "x$WINDOWS" = "xyes" ])

## handle command line options
AC_ARG_ENABLE([warnings],
  [AS_HELP_STRING([--enable-warnings],
    [Enable extra verbose warnings (disabled by default)])])
AS_IF([test "x$enable_warnings" = "xyes"], [AX_CFLAGS_WARN_ALL])

## check for programs
AC_PROG_CC
LT_INIT

## configure libtool versioning
AC_SUBST(LT_CURRENT, 0)
AC_SUBST(LT_REVISION, 0)
AC_SUBST(LT_AGE, 0)

## check for libraries
AS_IF([test "x$LIBUSB" = "xyes" ],
  [PKG_CHECK_MODULES(LIBUSB, libusb-1.0 >= 1.0)
  # suppress libusb header warnings (as needed)
  AS_IF([test "x$enable_warnings" = "xyes"],
    [LIBUSB_CFLAGS=$(echo "${LIBUSB_CFLAGS}" | sed -e 's|-I/|-isystem/|')])])

## checks for header files
AC_CHECK_HEADERS([getopt.h limits.h])

## check for typedefs, structures, and compiler characteristics
AC_C_INLINE
AC_C_BIGENDIAN
AC_TYPE_INT16_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
gl_EOVERFLOW

## check for library functions
AC_CHECK_FUNCS([getopt_long memset strdup strtoul])
AC_FUNC_MALLOC
AX_SHORT_SLEEP

## generate output
AC_SUBST([ac_aux_dir])
AC_CONFIG_FILES([\
  libfcd.pc \
  Makefile \
])
AC_OUTPUT
