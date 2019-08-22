
#if (GCOV AND ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
#  message(STATUS "Setting GCOV library (-lgcov)")
#  set(GCOV_LIB gcov)
#endif()

add_executable(exception_tests exceptions_tests.cpp )
target_link_libraries(exception_tests GTest::GTest GTest::gtest_main cpp-pthread-static ${GCOV_LIB})
add_test(NAME exception_tests COMMAND exception_tests)

add_executable(concurrency_tests concurrency_tests.cpp )
target_link_libraries(concurrency_tests GTest::GTest GTest::gtest_main cpp-pthread-static ${GCOV_LIB})
add_test(NAME concurrency_tests COMMAND concurrency_tests)

add_executable(thread_tests thread_tests.cpp)
target_link_libraries(thread_tests GTest::GTest GTest::gtest_main cpp-pthread-static ${GCOV_LIB})
add_test(NAME thread_tests COMMAND thread_tests)

add_executable(abstract_thread_tests abstract_thread_tests.cpp)
target_link_libraries(abstract_thread_tests GTest::GTest GTest::gtest_main cpp-pthread-static ${GCOV_LIB})
add_test(NAME abstract_thread_tests COMMAND abstract_thread_tests)

add_executable(synchronized_queue_tests synchronized_queue_tests.cpp)
target_link_libraries(synchronized_queue_tests GTest::GTest GTest::gtest_main cpp-pthread-static ${GCOV_LIB})
add_test(NAME synchronized_queue_tests COMMAND synchronized_queue_tests)

