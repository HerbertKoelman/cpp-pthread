#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT([cpp-pthread],[1.0.0],[herbert.koelman@pmu.fr])
AC_LANG(C++)
AC_CONFIG_SRCDIR([without-cpp11-pthread-tests.cpp])
AC_CONFIG_AUX_DIR([/usr/bin ../ ../bin])

# Checks for programs.
AC_PROG_CXX([xlC_r xlC g++ c++ gcc cl KCC CC cxx cc++ aCC ])
AC_PROG_CC([xlc xlc_r gcc cl cc])
AC_CHECK_PROG([AR],[ar],[ar -rv])
AC_CHECK_PROG([MV],[mv],[mv],[echo "no mv command found"])
AC_PROG_RANLIB

CFLAGS="-I ../include -I ./"
LDFLAGS="$LDFLAGS -L ../lib"

AC_CHECK_SIZEOF([long])
if test $ac_cv_sizeof_long == "8" 
then
  AC_SUBST(BITS,"64")
  BITS=64
else
  AC_MSG_RESULT([using 32 bits (default)])
  AC_SUBST(BITS,"32")
  BITS=32
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
    CPPFLAGS="-qlanglvl=extended0x:decltype:static_assert:rvaluereferences:rvaluereferences -qsourcetype=c++ -O $CPPFLAGS"
    CFLAGS="-bh:5 -qmaxmem=-1 -q$BITS $CFLAGS"
    CXXFLAGS="-O"
    LDFLAGS="$LDFLAGS -brtl -bmaxdata:0x80000000"
    AC_MSG_RESULT([using large memory model (maxdata:0x80000000), optimising level 2 (-O),using run-time link method (-brtl)])
  ;;
  g++ | gcc)
    CPPFLAGS="-x c++ -std=c++11 -frtti $CPPFLAGS"
    CXXFLAGS="-O2"
    AC_MSG_RESULT([yes])
  ;;
  *)
  AC_MSG_RESULT(none found.)
esac

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create],[],[AC_ERROR([missing pthread library.])])
AC_CHECK_LIB([cpp-pthread], [cpp_pthread_version],[],[AC_ERROR([missing cpp-thread library.])])

# Checks for header files.
#AC_HEADER_STDC
#AC_CHECK_HEADERS([nl_types.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
#AC_C_CONST
#AC_C_INLINE
#AC_HEADER_TIME

# Checks for library functions.
#AC_TYPE_SIGNAL

# Checks for typedefs, structures, and compiler characteristics.
#AC_HEADER_STDBOOL

# Checks for library functions.
#AC_FUNC_ERROR_AT_LINE
#AC_FUNC_MALLOC
AC_CHECK_FUNCS([gettimeofday])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

