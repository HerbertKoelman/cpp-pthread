#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT([cpp-pthread],[1.0.0],[herbert.koelman@pmu.fr],[http://mut-mut-scmv1.dev.parimutuel.local/applications/cpp-pthread])

AC_CONFIG_AUX_DIR([/usr/bin . bin])

AC_CANONICAL_BUILD

AC_PROG_INSTALL
AC_CHECK_PROG([RM],[rm],[rm -f])
AC_PROG_MKDIR_P

AC_MSG_CHECKING([whether target directory ./lib exists])
if test -e ./lib
then
  AC_MSG_RESULT([already there.])
else
  mkdir -p ./lib && AC_MSG_RESULT([./lib created.])
fi

AC_MSG_CHECKING([whether target directory ./doc exists])
if test -e ./doc
then
  AC_MSG_RESULT([already there.])
else
  mkdir -p ./doc && AC_MSG_RESULT([./doc created.])
fi

AC_MSG_CHECKING([whether wording directory ./locale exists])
if test -e ./locale
then
  AC_MSG_RESULT([already there.])
else
  mkdir -p ./locale/en_US ./locale/fr_FR && AC_MSG_RESULT([./locale created.])
fi

AC_MSG_CHECKING([whether wording directory ./bin exists])
if test -e ./bin
then
  AC_MSG_RESULT([already there.])
else
  mkdir -p ./bin && AC_MSG_RESULT([./bin created.])
fi

AC_MSG_CHECKING([whether wording directory ./app exists])
if test -e ./app
then
  AC_MSG_RESULT([already there.])
else
  mkdir -p ./app && AC_MSG_RESULT([./app created.])
fi

AC_MSG_CHECKING([whether wording directory ./distrib exists])
if test -e ./distrib
then
  AC_MSG_RESULT([already there.])
else
  mkdir -p ./distrib && AC_MSG_RESULT([./distrib created.])
fi

HEADERS=""
for header in include/pthread/*.hpp include/pthread/*.h
do
  HEADERS="$HEADERS $header"
done
AC_SUBST(HEADERS,$HEADERS)

AC_CONFIG_SUBDIRS(src)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

