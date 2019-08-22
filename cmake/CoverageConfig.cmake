message(STATUS "Detecting if lcov was installed")
find_program(LCOV lcov)
if ( LCOV )

  message(STATUS "Found lcov [${LCOV}]...")
  add_custom_target( coverage
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/LCOV
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Coverage report (${CMAKE_CURRENT_LIST_DIR}/LCOV)"
    VERBATIM
    )
  message(STATUS "Added custom target [coverage]...")

endif()
