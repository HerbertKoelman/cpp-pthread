//! \file

#include <pthread.h>
#include "pthread/pthread.hpp"
#include "gtest/gtest.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <memory>
#include <ctime>

TEST(concurrency, mutex) {

  bool success = false;

  pthread::mutex mutex;
  try {
    pthread::lock_guard<pthread::mutex> lock(mutex);
    success = true;
    // std::cout << "in critical section (mutex)" << std::endl;

  } catch ( std::exception &err) {
    std::cerr << "something went wrong: " << err.what() << std::endl;
  }catch ( ... ){
    std::cerr << "something went wrong, unexpected exception catched."<< std::endl;
  }

  EXPECT_TRUE(success);
}

TEST(concurrency, read_write_lock) {
  bool success = false ;

  try {

    pthread::read_write_lock _rwlock;

    {
      pthread::lock_guard<pthread::read_lock> lock(_rwlock);

      // std::cout << "read locked" << std::endl;
    }

    {
      pthread::lock_guard<pthread::write_lock> lock(_rwlock);
      // std::cout << "write locked" << std::endl;
    }

    success = true;

  }catch (std::exception &err ){
    std::cerr << __FILE__ << "(at:" << __LINE__ << ")" << err.what() << std::endl;
  }

  EXPECT_TRUE(success);
}