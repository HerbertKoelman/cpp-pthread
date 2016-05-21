//
//  main.cpp
//  pthread_tests
//
//  Thread stack size can be ajusted using this environment variable: 
//  - AIXTHREAD_STK=<n bytes>
//
//  Check ulimit to make sure you're allowed to allocate the required memory (ulimit -a).
//
//  More here https://www.ibm.com/support/knowledgecenter/ssw_aix_72/com.ibm.aix.performance/thread_supp_tun_params.htm
//
//  Created by herbert koelman on 18/03/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include <cstdio>
#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <memory>
#include "pthread/pthread.hpp"

pthread::condition_variable condition;
pthread::mutex mtx;
pthread::mutex cout_mtx;
int counter = 0;

void message ( const std::string m){
  pthread::lock_guard<pthread::mutex> lck(cout_mtx);
  std::cout << m << std::endl;
}

class worker: public pthread::abstract_thread {
public:
  
  worker(const std::string m = "anonymous worker", int sleep = 2*1000):abstract_thread(8000000),  msg(m), _sleep(sleep){
//    msg.resize(msg.size()+1);    
  };
  
  ~worker(){
    message("deallocating worker");
  };
  
#if __cplusplus < 201103L
  void run() throw()  {
#else
  void run() noexcept override {
#endif
    try {
      message("worker: " + msg);
      {
        pthread::lock_guard<pthread::mutex> lck(mtx);
        
        message("running worker class. wait max 2s for condition to be signaled");
        bool stop_waiting = true;
        auto delay = _sleep;
        while ( ! (stop_waiting = (counter >= 10000)) && (condition.wait_for(mtx, delay) == pthread::cv_status::no_timeout)){
          delay = -1 ;
        }
        if ( counter >= 10000 ) {
          message("worker class, counter >= 10000");
        } else {
          message("worker class, counter < 10000");
        }
      }
      
      message("worker sleeping");
      pthread::this_thread::sleep(200);
      message("worker class is ending");

    } catch ( pthread::pthread_exception &err ){
      message(err.what());
    } catch ( std::exception &err ) {
      message(err.what());
    } catch ( ... ) {
      message("Unexpected exception was thrown.");
    }
  };
  
private:
  std::string    msg ;
  int            _sleep;
};

int main(int argc, const char * argv[]) {
  
  try {
    std::cout << "lib version: " << pthread::cpp_pthread_version() << std::endl;

    int number_of_threads = 2;
    if ( argc > 0 ){
       number_of_threads = atoi(argv[1]);
    }

    pthread::thread_group threads(true);
    for (auto x = number_of_threads ; x > 0 ; x--){
      message("add new work to thread group");
      threads.add( new worker("herbert"));
    }
    
    threads.start();
    
    message("main increment counter");
    for ( auto x = 20000 ; x > 0 ; x--){
      pthread::lock_guard<pthread::mutex> lck(mtx);
      counter++ ;
      condition.notify_one();
    }
    condition.notify_all();
    
    message("main is waiting for threads to finish");
    threads.join();
  
  } catch ( pthread::pthread_exception &ex ){
    message(ex.what());
    message(ex.pthread_errmsg());
  } catch ( std::exception &ex ){
    message(ex.what());
  } catch ( ... ){
    message("Unhandled exception was thrown in main");
  }

  pthread::this_thread::sleep(5*1000); // sleep 10 seconds
  message( "end reached");
}
