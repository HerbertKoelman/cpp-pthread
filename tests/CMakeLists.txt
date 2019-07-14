
if (GCOV AND ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
    set(GCOV_LIB gcov)
endif()

message(STATUS "Add GoogleTest cpp_thread_tests")
add_executable(cpp_thread_tests concurrency_tests.cpp exceptions_tests.cpp synchronized_queue_tests.cpp)
target_link_libraries(cpp_thread_tests GTest::GTest GTest::gtest_main cpp-pthread-static ${GCOV_LIB})

add_test(NAME cpp_thread_tests COMMAND cpp_thread_tests)
