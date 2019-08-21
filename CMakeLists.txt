# Name: C++ threading library
# Creation date: lundi 24 septembre 2018, 10:39:35 (UTC+0200)
# Description: C++ threading library that mimics the C++11 standard implementation.
#
cmake_minimum_required(VERSION 3.11)

project(
        cpp-pthread
        VERSION 1.10.0
        DESCRIPTION "Simple C++ wrapper of the standard POSIX pthread C library.")

if (CMAKE_BUILD_TYPE MATCHES Release)
    if (GIT_LOG)
        set(CPP_PTHREAD_VERSION "${PROJECT_VERSION} - ${GIT_LOG}")
    else ()
        set(CPP_PTHREAD_VERSION "${PROJECT_VERSION}")
    endif ()
else ()
    if (GIT_LOG)
        set(CPP_PTHREAD_VERSION "${PROJECT_VERSION}-${GIT_LOG}-SNAPSHOT")
    else ()
        set(CPP_PTHREAD_VERSION "${PROJECT_VERSION}-SNAPSHOT")
    endif ()
endif ()

option(BUILD_TESTS "enable/disable tests (default is enabled)" ON)
option(GCOV "Activate GCOV options")
option(DEBUG "Set macro DEBUG")

if (DEBUG)
    message(STATUS "Setting DEBUG macro...")
    add_definitions(-DDEBUG=1)
endif ()

if (GCOV)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        message(STATUS "Setting GCOV compiler options")
        add_compile_options(--coverage)
        if (CMAKE_VERSION VERSION_GREATER "3.13.5")
            message(STATUS "Setting GCOV linker options")
            add_link_options(--coverage)
        else ()
            message(STATUS "Setting GCOV linker options (prior to 3.13.5)")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
        endif ()
    else ()
        message(SEND_ERROR "The GCOV option is only supported when using GNU...")
    endif ()
endif ()

find_package(GTestExt PATHS cmake)

# main targets --------------------------------------------------
#

add_definitions(-DCPP_PTHREAD_VERSION="${CPP_PTHREAD_VERSION}")

# This part MUST be executed before the loading of the CMake package. As we are using the CPP-PTHREAD_VERSION variable in the sonar properties file, 
# we need to reun this after the setting of the version variable.
set(SONAR_PROPERTIES_FILE ${CMAKE_CURRENT_BINARY_DIR}/sonar-project.properties)
message(STATUS "Generating SONAR properties file ${SONAR_PROPERTIES_FILE}")
configure_file(${CMAKE_CURRENT_LIST_DIR}/sonar-project.properties.in ${SONAR_PROPERTIES_FILE})
find_package(SonarCloud PATHS cmake)

configure_file(src/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h)

# project's public headers
include_directories(include src)

set(PTHREAD_SOURCE_CODE
        src/config.h
        src/condition_variable.cpp
        src/exceptions.cpp
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
endif ()

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

endif ()

# install -------------------------------------------------------
#
install(TARGETS cpp-pthread-static cpp-pthread-shared DESTINATION lib)
install(DIRECTORY include DESTINATION include COMPONENT Devel)
install(DIRECTORY ${PROJECT_BINARY_DIR}/html/ DESTINATION doc/cpp-pthread COMPONENT Documentation)

# CPACK ---------------------------------------------------------
#

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "Herbert Koelman")
set(CPACK_PACKAGE_VENDOR "Urbix Software")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

include(CPack)
