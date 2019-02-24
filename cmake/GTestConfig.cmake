set(__GTEST_DOWNLOAD ${CMAKE_BINARY_DIR}/googletest-download)
file(MAKE_DIRECTORY ${__GTEST_DOWNLOAD})
if (EXISTS ${__GTEST_DOWNLOAD})

    option(INSTALL_GMOCK "Install Googletest's GMock?" OFF)
    option(INSTALL_GTEST "Install Googletest's GTest?" OFF)

    message(STATUS "Generating GoogleTest CMakeLists.txt")
    configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt.in ${__GTEST_DOWNLOAD}/CMakeLists.txt)

    message(STATUS "Setup build for GoogleTest")
    execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
            WORKING_DIRECTORY "${__GTEST_DOWNLOAD}" )

    message(STATUS "Building GoogleTest")
    execute_process(COMMAND "${CMAKE_COMMAND}" --build .
            WORKING_DIRECTORY "${__GTEST_DOWNLOAD}" )

    # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This adds
    # the following targets: gtest, gtest_main, gmock
    # and gmock_main
    add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
            "${CMAKE_BINARY_DIR}/googletest-build")

    # The gtest/gmock targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if(CMAKE_VERSION VERSION_LESS 2.8.11)
    message(STATUS "GoogleTest include directory ${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include)")
        include_directories("${gtest_SOURCE_DIR}/include"
                "${gmock_SOURCE_DIR}/include")
    endif()

    add_library(GTest::GTest ALIAS gtest)
    add_library(GTest::gtest_main ALIAS gtest_main)

    add_library(GMock::GMock ALIAS gmock)
    add_library(GMock::gmock_main ALIAS gmock_main)

else()
    message(STATUS "Failed to create GoogleTest download directory")
endif()