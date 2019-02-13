# Testing -------------------------------------------------------
#

# Name: Test-Herbert
# Creation date: mercredi 7 novembre 2018, 10:13:42 (UTC+0100)
# Description: C++ pthread library test
#

# Locate GTest
find_package(GTest)
if (GTest_FOUND)

  enable_testing()
  include_directories(${GTEST_INCLUDE_DIRS})

  add_executable(cpp_thread_tests concurrency_tests.cpp exceptions_tests.cpp synchronized_queue_tests.cpp)
  target_link_libraries(cpp_thread_tests ${GTEST_LIBRARIES} cpp-pthread-shared gtest_main)

  set_property(TARGET cpp_thread_tests APPEND_STRING PROPERTY LINK_FLAGS "--coverage -fprofile-arcs -ftest-coverage")

  add_test(NAME cpp_thread_tests COMMAND cpp_thread_tests)

endif()
