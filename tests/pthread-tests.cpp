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
#include <thread>
#include "pthread/pthread.hpp"

pthread::condition_variable condition;
pthread::mutex mtx;
pthread::mutex cout_mtx;
int counter = 0;

void message ( const std::string m){
  pthread::lock_guard<pthread::mutex> lck{cout_mtx};
  std::cout << m << std::endl;
}

class worker: public pthread::runnable {
public:
  
  ~worker(){
    message("deallocating worker");
  };
  
  void run() noexcept override {
    message("waiting 2s");
    pthread::this_thread::sleep(2000);
    message("thread wokeup");

    pthread::lock_guard<pthread::mutex> lck(mtx);
    
    message("running");
    if ( condition.wait_for(lck, 20*1000, [this]{ return counter >= 10000;})){
      message("counter >= 10000");
    } else {
      message("counter < 10000");
    }
    
    message("worker is ending");
  };
  
};

void worker_function(std::string msg, int y, worker &w) {
  //  std::string msg = "fall back";
  message("waiting 2s: " + msg);
  pthread::this_thread::sleep(2000);
  message(msg + " thread wokeup");
  
  pthread::lock_guard<pthread::mutex> lck(mtx);
  
  for ( auto x = y; x > 0 ; x--){
    counter++ ;
  }
  
  std::string dummy;
  message(msg + " thread stopped, type enter to continue and exit thread");
  std::getline(std::cin, dummy);
  message(msg + " worker is ending");
};

void start_thread(std::string m){
  message(m);
}

int main(int argc, const char * argv[]) {
  
  pthread::string dummy;
  worker w;
  pthread::thread t0;
  pthread::thread t2{worker_function, "herbert", 10000, &w};
  
  t0 = std::move(t2) ;
  
  pthread::thread t4{std::move(t0)};
  pthread::thread t1{worker_function, "laura", 20000, &w};
  
//  t0.join();
  t1.join();
  t4.join();
//  t2.join();

  message( "end reached");
  
}
