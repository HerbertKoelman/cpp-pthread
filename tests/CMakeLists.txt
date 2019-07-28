
if (GCOV AND ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
  message(STATUS "Setting GCOV library (-lgcov)")
  set(GCOV_LIB gcov)
endif()

add_executable(cpp_thread_tests concurrency_tests.cpp exceptions_tests.cpp thread_tests.cpp)
target_link_libraries(cpp_thread_tests GTest::GTest GTest::gtest_main cpp-pthread-static ${GCOV_LIB})
add_test(NAME cpp_thread_tests COMMAND cpp_thread_tests)

add_executable(synchronized_queue_tests synchronized_queue_tests.cpp)
target_link_libraries(synchronized_queue_tests GTest::GTest GTest::gtest_main cpp-pthread-static ${GCOV_LIB})
add_test(NAME synchronized_queue_tests COMMAND synchronized_queue_tests)

