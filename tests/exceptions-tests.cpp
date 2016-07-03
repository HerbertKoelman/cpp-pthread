#include <pthread.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <memory>
#include "pthread/pthread.hpp"

int main(int argc, const char * argv[]) {
  
  std::cout << "tesing exceptions in lib version: " << pthread::cpp_pthread_version() << std::endl;

  try {
    throw pthread::pthread_exception("POSIX Thread exception test.", -1);
  } catch ( pthread::pthread_exception &ex ){
    std::cerr << ex.what() << std::endl;
    std::cerr << ex.pthread_errmsg() << std::endl;
  }

  try {
    throw pthread::thread_exception("Thread exception test.", 1);
  } catch ( pthread::pthread_exception &ex ){
    std::cerr << ex.what() << std::endl;
    std::cerr << ex.pthread_errmsg() << std::endl;
  }

  try {
    throw pthread::timeout_exception("Timeout exception test.");
  } catch ( pthread::pthread_exception &ex ){
    std::cerr << ex.what() << std::endl;
    std::cerr << ex.pthread_errmsg() << std::endl;
  }

  try {
    throw pthread::util::queue_exception();
  }catch (std::exception &err ){
    std::cerr << err.what() << std::endl;
  }
}
