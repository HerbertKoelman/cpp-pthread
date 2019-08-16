#!/usr/bin/env bash

# does actual build.
#
cmake_build(){

  echo "[ -d $cmake_build_dir ] && ( cd $cmake_build_dir && cmake $cmake_args .. && make $make_args )"
  [ -d $cmake_build_dir ] && ( cd $cmake_build_dir && cmake $cmake_args .. && make $make_args )

}

# build command usage message
#
usage(){
    echo "usage: `basename $0` [-G] [-S] [-T <build_type>]"
    echo
    echo "Build this module."
    echo
    echo "-S setup things to run SONAR"
    echo "-G setup things to compile with GCOV options and libs"
    echo "-T build type (Release, Debug, ...)"
    echo

    exit 99
}

git_current_branch=`git rev-parse --abbrev-ref HEAD -- | head -1`

if [ $git_current_branch == "master" ] 
then
  cmake_build_type="-DCMAKE_BUILD_TYPE=Release"
else
  cmake_build_type="-DCMAKE_BUILD_TYPE=Debug"
  cmake_build_dir=cmake-build
fi

make_args="all test"

while getopts "SGT:" option
do
 case $option in
    S) cmake_sonar_option="-DSONAR=yes" ; cmake_gcov_option="-DGCOV=yes" ; make_args="code_quality";;
    G) cmake_gcov_option="-DGCOV=yes" ;;
    T) cmake_build_type="-DCMAKE_BUILD_TYPE=$OPTARG" ;;
    *) usage ;;                     # display usage and exit
 esac
done

cmake_args="$cmake_build_type $cmake_gcov_option $cmake_sonar_option"

echo "##############################################################################"
echo "#"
echo "# Project: cpp-pthread"
echo "# Build date: `date`"
echo "# Build directory: $cmake_build_dir"
echo "# Build options: $cmake_args"
echo "# GIT current branch: [$git_current_branch]"
echo "#"
echo "##############################################################################"

[ -d $cmake_build_dir ] && (rm -Rf $cmake_build_dir/* && cmake_build ) || ( mkdir $cmake_build_dir && cmake_build )
