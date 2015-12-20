        #                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([estlc-front], [0.1], [estlc@riaqn.com])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_CONFIG_SRCDIR([syntaxAnalyzer.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CXX
AM_PROG_AR
AC_PROG_CC
LT_INIT

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.

AC_SUBST([AM_CXXFLAGS], ["-g3 -O0 -ggdb -Wall -Wextra -Werror -std=c++11"])
AC_SUBST([AM_CPPFLAGS], [])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
