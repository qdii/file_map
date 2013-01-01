#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([file_map], [1.0], [victor.lavaud@gmail.com])
AM_INIT_AUTOMAKE
AC_LANG(C++)
AC_CONFIG_SRCDIR([unittests.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h unistd.h cstring.h])
AC_CHECK_HEADERS([UnitTest++/UnitTest++.h])
AC_CHECK_HEADERS([sys/mman.h], ,[AC_MSG_WARN([mman.h was not found on your computer. Parsing will be slower.])])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MMAP
AC_CHECK_FUNCS([munmap])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
