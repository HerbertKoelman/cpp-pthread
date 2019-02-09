#include <pthread.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <memory>
#include "pthread/pthread.hpp"
#include "gtest/gtest.h"

TEST(exceptions, pthread_exception) {
  try {
    throw pthread::pthread_exception("POSIX Thread exception test.", -1);
  } catch (pthread::pthread_exception &ex) {
//    std::cerr << ex.what() << std::endl;
//    std::cerr << ex.pthread_errmsg() << std::endl;

    EXPECT_STREQ("POSIX Thread exception test.", ex.what());
    EXPECT_EQ(-1, ex.pthread_errno());
  }
}

TEST(exceptions, thread_exception) {
  try {
    throw pthread::thread_exception("Thread exception test.", 1);
  } catch (pthread::pthread_exception &ex) {
//    std::cerr << ex.what() << std::endl;
//    std::cerr << ex.pthread_errmsg() << std::endl;

    EXPECT_STREQ("Thread exception test.", ex.what());
    EXPECT_EQ(1, ex.pthread_errno());
  }
}

TEST(exceptions, timeout_exception) {
  try {
    throw pthread::timeout_exception("Timeout exception test.");
  } catch (pthread::pthread_exception &ex) {
//    std::cerr << ex.what() << std::endl;
//    std::cerr << ex.pthread_errmsg() << std::endl;

    EXPECT_STREQ("Timeout exception test.", ex.what());
    EXPECT_EQ(110, ex.pthread_errno());
  }
}

TEST(exceptions, queue_exception) {
  try {
    throw pthread::util::queue_exception();
  }catch (std::exception &ex ){
    EXPECT_STREQ("queue_error occured.", ex.what());
  }
}