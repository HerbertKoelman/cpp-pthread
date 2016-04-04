#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT([cpp-pthread],[m4_esyscmd_s(echo $(git describe --always))],[herbert.koelman@pmu.fr],[])
AC_LANG(C++)

AC_CONFIG_AUX_DIR([/usr/bin . bin])

AC_CANONICAL_BUILD

AC_PROG_INSTALL
AC_CHECK_PROG([RM],[rm],[rm -f])
AC_CHECK_PROG([DOXYGEN],[doxygen],[doxygen],[echo \"please install doxygen and re-run ./configure, or run it manualy doxygen \"])
AC_PROG_MKDIR_P

AC_MSG_CHECKING([whether target directory ./lib exists])
if test -e ./lib
then
  AC_MSG_RESULT([already there.])
else
  mkdir -p ./lib && AC_MSG_RESULT([./lib created.])
fi

HEADERS=""
for header in include/pthread/*.hpp include/pthread/*.h
do
  HEADERS="$HEADERS $header"
done
AC_SUBST(HEADERS,$HEADERS)

AC_CONFIG_SUBDIRS(src tests)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

