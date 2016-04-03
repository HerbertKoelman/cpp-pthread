#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT([cpp-pthread],[1.0.0],[herbert.koelman@pmu.fr])
AC_LANG(C++)
AC_CONFIG_SRCDIR([thread.cpp])
AC_CONFIG_AUX_DIR([/usr/bin ../ ../bin])
AC_CONFIG_HEADERS([../include/pthread/config.h])
AC_DEFINE_UNQUOTED(CPP_THREAD_VERSION, ["version: $PACKAGE_VERSION - branch: <branch> - compiled on `uname -sv`."], [pthread C++ wrapper.])

# Checks for programs.
AC_PROG_CXX([xlC_r xlC gcc cl KCC CC cxx cc++ aCC c++ g++])
AC_PROG_CC([xlc xlc_r gcc cl cc])
AC_CHECK_PROG([AR],[ar],[ar -rv])
AC_CHECK_PROG([MV],[mv],[mv],[echo "no mv command found"])
AC_CHECK_PROG([BEAUTIFIER],[uncrustify],[uncrustify -l CPP --replace -c ../crust.cfg *.bsr *.bcl *.cpp *.hpp],[echo \"install crustify to run beautifier\"])
AC_PROG_RANLIB

CXXFLAGS="$CXXFLAGS -I ../include -I ./"
LDFLAGS="$LDFLAGS -L ../lib"

AC_CHECK_SIZEOF([long])
if test $ac_cv_sizeof_long == "8" 
then
  AC_SUBST(BITS,"64")
else
  AC_MSG_RESULT([using 32 bits (default)])
  AC_SUBST(BITS,"32")
fi

OBJECTS=""
for src in *.cpp
do
  obj=`basename $src .cpp`.o
  OBJECTS="$OBJECTS $obj"
done
AC_SUBST(OBJECTS,$OBJECTS)

AC_MSG_CHECKING([for specific $CXX compiler options])
case "$CXX" in
  xlC_r | xlC)
	CXXFLAGS="-bh:5 -qlanglvl=extended0x:decltype:static_assert::rvaluereferences:rvaluereferences -qwarn0x  -qsourcetype=c++ -O $CXXFLAGS"
  AC_MSG_RESULT([yes])
  ;;
  g++ | gcc)
	CXXFLAGS="-x c++ -std=c++11 -frtti $CXXFLAGS "
  AC_MSG_RESULT([yes])
  ;;
  *)
  AC_MSG_RESULT(none found.)
esac

CFLAGS=$CXXFLAGS
CPPFLAGS="$CPPFLAGS $CFLAGS"

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create],[],[AC_ERROR([missing pthread library.])])

# Checks for header files.
#AC_HEADER_STDC
AC_CHECK_HEADERS([nl_types.h stdlib.h string.h unistd.h])

AC_CHECK_HEADER([mutex],  [AC_DEFINE([HAVE_CPP11_MUTEX])])
AC_CHECK_HEADER([chrono], [AC_DEFINE([HAVE_CPP11_CHRONO])])
AC_CHECK_HEADER([thread], [AC_DEFINE([HAVE_CPP11_THREAD])])

# Checks for typedefs, structures, and compiler characteristics.
#AC_C_CONST
#AC_C_INLINE
#AC_HEADER_TIME

# Checks for library functions.
#AC_TYPE_SIGNAL

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
#AC_FUNC_ERROR_AT_LINE
#AC_FUNC_MALLOC
AC_CHECK_FUNCS([gettimeofday])

LDFLAGS="$LDFLAGS $LIBS"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

