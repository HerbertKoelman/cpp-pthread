//
//  main.cpp
//  pthread_tests
//
//  Created by herbert koelman on 18/03/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include <iostream>
#include <string>
#include "pthread/pthread.hpp"

//#include "pthread/mutex.hpp"
//#include "pthread/lock_guard.hpp"
//#include "pthread/condition_variable.hpp"

pthread::condition_variable condition;
pthread::mutex mtx;

int counter = 0;


void message ( const char *m){
  pthread::lock_guard lck{mtx};
  std::cout << m << std::endl;
}

class thread: public pthread::thread {
public:
  
  void run() noexcept override {
    {
      pthread::lock_guard lck{mtx};
      try {
        std::cout << "got mutex and start waiting for counter" << std::endl;
        
        if ( condition.wait_for(lck, 20000, [](){ return counter >= 100000 ;} ) ){
//        if ( condition.wait(lck, [](){ return counter >= 100000 ;} ) ){
          std::cout << "condition was true" << std::endl;
        } else {
          std::cout << "condition was false" << std::endl;
        }
        
        std::cout << "end off wait" << std::endl ;
      } catch ( pthread::pthread_exception &err){
          std::cerr << "thread failed. " << err.what() << " " << err.pthread_errmsg() << std::endl;
      }
    }
    
    sleep(10*1000);
    message("thread woke up from a 10s nap");
  }
};
      
int main(int argc, const char * argv[]) {
  std::string dummy;
  thread t ;
  t.start();
  
  for ( auto x = 100000; x > 0 ; x--){
    pthread::lock_guard lck{mtx};
    counter++ ;
    //condition.notify_one();
  }
  
  std::cout << "hit enter to continue" << std::endl ;
  std::getline(std::cin, dummy);
  
  message("sleeping for 5 seconds...");
  pthread::thread::sleep(5*1000);
  
  message("woke up from sleep, main thread counted 100000, notifying all condition_variables");
  condition.notify_all();
  
  message("joining waiting trhreads");
  t.join();
  message( "end reached");
  
}
