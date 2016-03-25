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
    message("waiting 200ms");
    pthread::this_thread::sleep(20000);
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

void start_thread(std::string m){
  message(m);
}

int main(int argc, const char * argv[]) {
  std::string dummy;
  
//  pthread::thread{start_thread, "hello"};
  
  std::list<pthread::thread> threads;
  for ( auto x = 10; x > 0 ; x--){
    worker w;
    threads.push_back(pthread::thread{w}) ;
  }

  for ( auto x = 1000000; x > 0 ; x--){
    pthread::lock_guard<pthread::mutex> lck{mtx};
    counter++ ;
    condition.notify_one();
  }
  
//  std::cout << "hit enter to continue" << std::endl ;
//  std::getline(std::cin, dummy);
  
  message("sleeping for 5 seconds...");
  pthread::this_thread::sleep(5*1000);
  
  message("woke up from sleep, main thread counted 100000, notifying all condition_variables");
  condition.notify_all();
  
  message("main waiting for threads to end");
  
  std::for_each( threads.begin(), threads.end(), [](pthread::thread &t){
    message("main joining thread");
    t.join();
  });
  
  message( "end reached");
  
}
