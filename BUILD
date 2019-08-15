#!/bin/sh

cmake_build_type="-DCMAKE_BUILD_TYPE=Debug"
cmake_gcov_option="-DGCOV=yes"
cmake_sonar_option="-DSONAR=yes"

cmake_args="$cmake_build_type $cmake_gcov_option $cmake_sonar_option"

cmake_build_dir=cmake-build

build(){

  [ -d $cmake_build_dir ] && ( cd $cmake_build_dir && cmake $cmake_args .. && make all test doxygen )

}

[ -d $cmake_build_dir ] && (rm -Rf $cmakje_build_dir/* && build ) || ( mkdir $cmake_build_dir && build )

