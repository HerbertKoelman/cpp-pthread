//! \file

#include <pthread.h>
#include "pthread/pthread.hpp"

#include <cstdio>
#include <iostream>
#include <string>
#include <memory>
#include <ctime>

int main(int argc, const char * argv[]) {
  
  std::cout << "version: " << pthread::cpp_pthread_version() << std::endl;

  try {

    pthread::read_write_lock _rwlock;

    {
      pthread::lock_guard<pthread::read_lock> lock(_rwlock);

      std::cout << "read locked" << std::endl;
    }

    {
      pthread::lock_guard<pthread::write_lock> lock(_rwlock);
      std::cout << "write locked" << std::endl;
    }

  }catch (std::exception &err ){
    std::cerr << __FILE__ << "(at:" << __LINE__ << ")" << err.what() << std::endl;
  }
}
