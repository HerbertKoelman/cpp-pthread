#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT([cpp-pthread],[m4_esyscmd_s(echo $(git describe --always --abbrev=0))],[herbert.koelman@me.com],[],[http://herbertkoelman.github.io/cpp-pthread/])
AC_LANG(C++)

AC_CONFIG_AUX_DIR([/usr/bin . bin])

AC_ARG_ENABLE([snapshot], AS_HELP_STRING([--enable-release],[build as release.]), [BUILD="SNAPSHOT"], [BUILD=$PACKAGE_VERSION])
AC_ARG_ENABLE([repository], AS_HELP_STRING([--enable-repository],[use the given name to locate the remote repository]), AC_MSG_NOTICE([repository set to $enable_repository]) )

if test ! -z "$enable_repository"
then
  AC_SUBST(REPOSITORY,"$enable_repository/$PACKAGE_NAME")
fi

AC_MSG_NOTICE([

    Build $PACKAGE_NAME version $BUILD
])
AC_SUBST(BUILD,$BUILD)

AC_CANONICAL_BUILD

AC_ARG_VAR([CODECHECK], [code quality control program])
AC_ARG_ENABLE([codecheck], \
  AS_HELP_STRING([--enable-codecheck=PATH], [set CODECHECK to run a specific program]), \
  CODECHECK=$enable_codecheck)

AC_PROG_INSTALL
AC_CHECK_PROG([RM],[rm],[rm -f])
AC_CHECK_PROG([DOXYGEN],[doxygen],[doxygen],[echo \"please install doxygen and re-run ./configure, or run it manualy doxygen \"])
AC_PROG_MKDIR_P
AC_PROG_LN_S
AC_CHECK_PROG([DEPLOY],[nexus-upload],[nexus-upload],[echo \"please install nexus-upload, make sure it\'s in your PATH and re-run ./configure\"])

# Okay, no cppcheck found, maybe we have sonar
if test -z "$CODECHECK" 
then
  AC_CHECK_PROG([CODECHECK],[sonar-runner],[sonar-runner -Dsonar.cfamily.library.directories=./include,/usr/include],[])
fi

# Okay, no cppcheck found, maybe we have sonar
if test -z "$CODECHECK" 
then
  AC_CHECK_PROG([CODECHECK],[cppcheck],[cppcheck --template='{file}:{line},{severity},{id}:\n    {message}\n' --enable=all -I ./include ./src ./include],[])
fi

AC_MSG_CHECKING([whether target directory ./lib exists])
if test -e ./lib
then
  AC_MSG_RESULT([already there.])
else
  mkdir -p ./lib && AC_MSG_RESULT([./lib created.])
fi

AC_CONFIG_SUBDIRS(src)

HEADERS=""
for header in include/pthread/*.hpp include/pthread/*.h
do
  HEADERS="$HEADERS $header"
done
AC_SUBST(HEADERS,$HEADERS)

AC_CONFIG_FILES([Makefile doxyfile sonar-project.properties])
AC_OUTPUT

