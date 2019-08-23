option(COVERAGE "Activate coverage")

if (COVERAGE AND ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
  message(STATUS "Detecting if have LCOV")
  find_program(LCOV lcov)
  if ( LCOV )

    message(STATUS "Found LCOV [${LCOV}]...")
    add_custom_target( coverage
      COMMAND ${CMAKE_CURRENT_LIST_DIR}/LCOV
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Coverage report (${CMAKE_CURRENT_LIST_DIR}/LCOV)"
      VERBATIM
      )
    message(STATUS "Added custom target [coverage]...")

  endif()

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
  message(SEND_ERROR "The COVERAGE option only works with GCOV...")
endif ()
