//
//  main.cpp
//  pthread_tests
//
//  Created by herbert koelman on 18/03/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include <iostream>
#include "pthread/pthread.hpp"

//#include "pthread/mutex.hpp"
//#include "pthread/lock_guard.hpp"
//#include "pthread/condition_variable.hpp"

pthread::condition_variable condition;
pthread::mutex mtx;

int counter = 0;

class thread: public pthread::thread {
public:
  
  void run() noexcept override {
    std::cout << "start wait for counter" << std::endl;
    
    if ( condition.wait(mtx, [](){ return counter > 10000 ;} ) ){
      std::cout << "condition was true" << std::endl;
    } else {
      std::cout << "condition was false" << std::endl;
    }
      
  }
};

int main(int argc, const char * argv[]) {
  
  thread t ;
  t.start();
  
  for ( auto x = 100000; x > 0 ; x--){
    {
      pthread::lock_guard lck{mtx};
      counter++ ;
    }
  }
  
  condition.notify_all();
  
  t.join();
  std::cout << "end reached" << std::endl;
  
}
