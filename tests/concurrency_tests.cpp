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

TEST(concurrency, condition_variable_wait_for){
    pthread::condition_variable condition;
    pthread::mutex              mutex;

    EXPECT_EQ(pthread::cv_status::timedout, condition.wait_for(mutex, 1*1000));
    EXPECT_THROW(mutex.try_lock(), pthread::pthread_exception);
    mutex.unlock();

    {
        pthread::lock_guard<pthread::mutex> lock{mutex};
        EXPECT_EQ(pthread::cv_status::timedout, condition.wait_for(lock, 1 * 1000));
    }
    EXPECT_NO_THROW(mutex.try_lock());
}

/* NOSONAR for later use
   class test_thread: public pthread::abstract_thread{
    public:
        void run() noexcept override{
            pthread::this_thread::sleep_for(5*1000);
            std::cout << "thread is waiting for mutex" << std::endl;
            pthread::lock_guard<pthread::mutex> lock(*_mutex);
            std::cout << "thread got mutex" << std::endl;
        }

        test_thread( pthread::mutex *mutex): _mutex(mutex){

        }
    private:
        pthread::mutex *_mutex;
    };

    pthread::mutex mutex;

    test_thread thread{&mutex};
    thread.start();
    {
        pthread::lock_guard<pthread::mutex> lock(mutex);
        pthread::this_thread::sleep_for(10*1000);
    }

    thread.join();

 */