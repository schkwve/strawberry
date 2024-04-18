AC_INIT([Strawberry], [0.1.0], [schkwve@gmail.com], [strawberry], [])

AM_SILENT_RULES([yes])

# Keep some clutter out of here
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# foreign keeps some junk out of this tree
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects foreign])

# Checks for compilers.
AC_PROG_CC

# Put configuration results in config.h
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile src/strawberryd/Makefile])
AC_OUTPUT
