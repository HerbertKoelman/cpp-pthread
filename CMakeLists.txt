# Name: C++ threading library
# Creation date: lundi 24 septembre 2018, 10:39:35 (UTC+0200)
# Description: C++ threading library that mimics the C++11 standard implementation.
#
cmake_minimum_required(VERSION 3.11)

project(
  cpp-pthread
  VERSION 1.8.0
  DESCRIPTION "Simple C++ wrapper to pthread functions (${GIT_LOG})")

option(BUILD_TESTS "enable/disable tests (default is enabled)" ON)
option(GCOV "Activate GCOV options")

if ( GCOV )
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(STATUS "Setting GCOV compiler options")
    add_compile_options(--coverage)
  else()
    message(SEND_ERROR "The GCOV option is only supported when using GNU...")
  endif()
endif()

find_package(GTestExt   PATHS cmake)

# main targets --------------------------------------------------
#
if( CMAKE_BUILD_TYPE MATCHES Release )
  if( GIT_LOG )
    add_definitions( -DCPP_PTHREAD_VERSION="${PROJECT_VERSION} - ${GIT_LOG}")
  else()
    add_definitions( -DCPP_PTHREAD_VERSION="${PROJECT_VERSION}")
  endif()
else()
  if( GIT_LOG )
    add_definitions( -DCPP_PTHREAD_VERSION="${PROJECT_VERSION}-${GIT_LOG}-SNAPSHOT")
  else()
    add_definitions( -DCPP_PTHREAD_VERSION="${PROJECT_VERSION}-SNAPSHOT")
  endif()
endif()

configure_file(src/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h)

# project's public headers
include_directories(include src)

set(PTHREAD_SOURCE_CODE
        src/config.h
        src/condition_variable.cpp
        src/exceptions.cpp
        src/lock_guard.cpp
        src/pthread.cpp
        src/read_write_lock.cpp
        src/thread.cpp
        src/mutex.cpp
        )

set(CMAKE_CXX_STANDARD 11)

add_library(cpp-pthread-static STATIC ${PTHREAD_SOURCE_CODE})
target_link_libraries(cpp-pthread-static pthread)
set_target_properties(cpp-pthread-static PROPERTIES OUTPUT_NAME cpp-pthread)

add_library(cpp-pthread-shared SHARED ${PTHREAD_SOURCE_CODE})

target_link_libraries(cpp-pthread-shared pthread)
set_target_properties(cpp-pthread-shared PROPERTIES OUTPUT_NAME cpp-pthread)

# Testing -------------------------------------------------------
#

if (GTestExt_FOUND AND BUILD_TESTS)
  enable_testing()
  message(STATUS "Adding project's unit tests (in ./tests)...")
  add_subdirectory(tests)
endif()

# doxygen -------------------------------------------------------
#
find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)
if (Doxygen_FOUND)

  set(DOXYGEN_PROJECT_NUMBER ${CPP_PTHREAD_VERSION})
  set(DOXYGEN_EXAMPLE_PATH tests)
  set(DOXYGEN_EXTRACT_ALL yes)
  set(DOXYGEN_PROJECT_BRIEF ${PROJECT_DESCRIPTION})
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
  doxygen_add_docs(doxygen README.md src include COMMENT "generate on-line documentation")

endif()

# install -------------------------------------------------------
#
install( TARGETS cpp-pthread-static cpp-pthread-shared DESTINATION lib )
install( DIRECTORY include DESTINATION include COMPONENT Devel)
install( DIRECTORY ${PROJECT_BINARY_DIR}/html/ DESTINATION doc/cpp-pthread COMPONENT Documentation)

# CPACK ---------------------------------------------------------
#

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "Herbert Koelman")
set(CPACK_PACKAGE_VENDOR "Urbix Software")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

include(CPack)
