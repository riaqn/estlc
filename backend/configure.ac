#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([estlc-back], [0.1], [estlc@riaqn.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/codegen.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
${CXXFLAGS=""}
AC_PROG_CXX
AM_PROG_AR
LT_INIT


# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_SUBST([AM_CXXFLAGS], ["-g3 -O0 -ggdb -Wall -Wextra -Werror"])
AC_SUBST([AM_CPPFLAGS], [])


AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile])


AC_OUTPUT
