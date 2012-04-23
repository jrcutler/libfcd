dnl Process this file with autoconf to produce a configure script.

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
LINUX="no"
AS_CASE([$host],
  [*-*-linux*],  [LINUX="yes"],
  [*-*-darwin*], [MACOSX="yes"],
  [*-*-win*],    [WINDOWS="yes" ; ac_cv_func_malloc_0_nonnull=yes],
  [*-*-mingw*],  [WINDOWS="yes" ; ac_cv_func_malloc_0_nonnull=yes],
  []
)
AM_CONDITIONAL([LINUX], [ test "x$LINUX" = "xyes" ])
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
AS_IF([test "x$LINUX" = "xyes" ],
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
