option(COVERAGE "Activate coverage")

if (COVERAGE)

  # Handle coverage with GNU compilers
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(STATUS "Detecting LCOV")
    find_program(LCOV lcov)
    if ( LCOV )

      message(STATUS "Detecting LCOV [${LCOV}] - done")
      add_custom_target( coverage
        COMMAND ${CMAKE_CURRENT_LIST_DIR}/LCOV
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Coverage report (${CMAKE_CURRENT_LIST_DIR}/LCOV)"
        VERBATIM
        )
      message(STATUS "Added custom build target [coverage]...")

    endif()

    message(STATUS "Setting GCOV --coverage compiler option")
    add_compile_options(--coverage)
    if (CMAKE_VERSION VERSION_GREATER "3.13.5")
        message(STATUS "Setting GCOV --coverage linker option")
        add_link_options(--coverage)
    else ()
        message(STATUS "Setting GCOV --coverage option in linker related variables CMAKE_EXE_LINKER_FLAGS and CMAKE_SHARED_LINKER_FLAGS")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
    endif ()
  endif ()
endif ()
