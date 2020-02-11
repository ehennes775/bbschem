AC_INIT([bbsch],[0.0],[http://github.com/ehennes775/bbsch/issues],[bbsch],[http://github.com/ehennes775/bbsch])
AC_PREREQ([2.64])

AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_PROG_CC

GLIB_GSETTINGS

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    ])

PKG_CHECK_MODULES([DEPS], [
    glib-2.0 >= 2.40
    gtk+-3.0 >= 3.10
    ])

AC_OUTPUT

