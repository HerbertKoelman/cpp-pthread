#!/usr/bin/env bash

# does actual build.
#
cmake_build(){

  echo "cd cmake-build && cmake $cmake_args .. && make $make_args "
  [ -d cmake-build ] && ( cd cmake-build && cmake $cmake_args .. && make $make_args )

}

# build command usage message
#
usage(){
    echo "usage: `basename $0` [-G] [-S] [-T <build_type>]"
    echo
    echo "Build this module."
    echo
    echo "-S setup things to run SONAR"
    echo "-G setup things to compile with coverage options and libs"
    echo "-T build type (Release, Debug, ...)"
    echo

    exit 99
}

set_current_branch(){
    if [ -z "$TRAVIS_BRANCH" ]
    then
      current_branch=`git rev-parse --abbrev-ref HEAD -- | head -1`
    else
      if [ -z "$TRAVIS_TAG" ] 
      then
        [ -z "$TRAVIS_PULL_REQUEST_BRANCH" ] && current_branch=$TRAVIS_BRANCH || current_branch=$TRAVIS_PULL_REQUEST_BRANCH
      else
        current_branch="master"
      fi
    fi
}

set_current_branch

if [ "$current_branch" == "master" ] 
then
  cmake_build_type="-DCMAKE_BUILD_TYPE=Release"
else
  cmake_build_type="-DCMAKE_BUILD_TYPE=Debug"
fi

make_args="all test"

while getopts "SGT:" option
do
 case $option in
    S) cmake_sonar_option="-DSONAR=yes" ; cmake_gcov_option="-DCOVERAGE=yes" ; make_args="code-quality";;
    G) cmake_gcov_option="-DCOVERAGE=yes" ; make_args="$make_args coverage";;
    T) cmake_build_type="-DCMAKE_BUILD_TYPE=$OPTARG" ;;
    *) usage ;;                     # display usage and exit
 esac
done

cmake_args="$cmake_build_type $cmake_gcov_option $cmake_sonar_option"

echo "##############################################################################"
echo "#"
[ ! -z "$TRAVIS_BRANCH" ] && echo -e "# Running on Travis (TRAVIS_BRANCH: $TRAVIS_BRANCH, TRAVIS_TAG: $TRAVIS_TAG, TRAVIS_PULL_REQUEST_BRANCH: $TRAVIS_PULL_REQUEST_BRANCH)\n#"
echo "# Project: cpp-pthread"
echo "# Build date: `date`"
echo "# Build directory: cmake-build"
echo "# Build options: $cmake_args"
echo "# GIT current branch: [$current_branch]"
echo "#"
echo "##############################################################################"

[ -d cmake-build ] && (rm -Rf cmake-build/* && cmake_build ) || ( mkdir cmake-build && cmake_build )

