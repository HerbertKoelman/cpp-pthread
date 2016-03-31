//
//  main.cpp
//  pthread_tests
//
//  Created by herbert koelman on 18/03/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#include <iostream>
#include <string>
#include <list>
//#include <thread>
#include "pthread/pthread.hpp"

pthread::condition_variable condition;
pthread::mutex mtx;
pthread::mutex cout_mtx;
int counter = 0;

void message ( const std::string m){
  pthread::lock_guard<pthread::mutex> lck{cout_mtx};
  std::cout << m << std::endl;
}

void worker_function(std::string msg, int y) ;

class no_lambda_worker: public pthread::runnable {
public:

  no_lambda_worker(): _msg{"no_lambda_worker"}{
    
  }
  
  void run() noexcept override {
    pthread::lock_guard<pthread::mutex> lck{mtx};
    message("no_lambda_worker::run has started to execute: " + _msg);
    bool stop_waiting = false ;
    
    while ( !(stop_waiting = (counter >= 10000)) && condition.wait_for(lck, 20*1000) == pthread::cv_status::no_timeout);
    
    if ( counter >= 10000 ){
      message("counter was greater then 10000");
    } else {
      message("counter was not greater then 10000");
    }
  }
private:
  std::string _msg;
};

/** using lamdba
 */
class worker: public pthread::runnable {
public:
  
  worker(const std::string m = "anonymous worker", int sleep = 2000): msg{m}, _sleep{sleep}{
    
  };
  
  ~worker(){
    message("deallocating worker");
  };
  
  void run() noexcept override {
    
    { // cirtical section
      pthread::lock_guard<pthread::mutex> lck(mtx);
      
      message("running worker class. wait max 20s for condition to be signaled");
      if ( condition.wait_for(lck, 20*1000, [this]{ return counter >= 10000;})){
        message("worker class, counter >= 10000");
      } else {
        message("worker class, counter < 10000");
      }
    } // mutex is released
    
    pthread::this_thread::sleep(_sleep);
    message("worker class is ending, counter actually was: "+ std::to_string(counter));
  };
  
private:
  std::string    msg ;
  int            _sleep;
};


int main(int argc, const char * argv[]) {
  
  pthread::string dummy;
  pthread::thread t10{no_lambda_worker{}};
  t10.join();
  
  worker w{"laura"};
  pthread::thread t0;
  pthread::thread t2{w};
  
  t0 = std::move(t2) ;
  
  pthread::thread t4{std::move(t0)};
  pthread::thread t1{w};
  
  std::list<pthread::thread> threads;
  threads.push_back(std::move(t1));
  threads.push_back(std::move(t4));
  threads.push_back(pthread::thread{w});
  threads.push_back(pthread::thread{worker{"herbert's worker"}});
  threads.push_back(pthread::thread {no_lambda_worker{}});
  
  for ( auto x = 100000; x > 0 ; x--){
    pthread::lock_guard<pthread::mutex> lck{mtx};
    counter++;
  }
  
  condition.notify_all();
  
  message("main is waiting for threads to finish");
  
  for_each(threads.begin(), threads.end(), [&](pthread::thread &thrd){
    thrd.join();
  });

  message( "end reached");
  
}
