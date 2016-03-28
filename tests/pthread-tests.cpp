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


class worker: public pthread::runnable {
public:
  
  worker(const std::string m = "anonymous worker"): msg{m}{
    
  };
  
  ~worker(){
    message("deallocating worker");
  };
  
  void run() noexcept override {
    
    worker_function("worker_function: " + msg, 10000);
    
    {
      pthread::lock_guard<pthread::mutex> lck(mtx);
      
      message("running worker class. wait max 20s for condition to be signaled");
      if ( condition.wait_for(lck, 20*1000, [this]{ return counter >= 10000;})){
        message("worker class, counter >= 10000");
      } else {
        message("worker class, counter < 10000");
      }
    }
    
    pthread::this_thread::sleep(20*1000);
    message("worker class is ending, counter actually was: "+ std::to_string(counter));
  };
  
private:
  std::string    msg ;
};

void worker_function(std::string msg, int y) {
  message("waiting 2s: " + msg);
  pthread::this_thread::sleep(2000);
  message(msg + " thread wokeup");
  
  pthread::lock_guard<pthread::mutex> lck(mtx);
  
  message("looping: " + std::to_string(y));
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
  
  message("main is waiting for threads to finish");
  
  for_each(threads.begin(), threads.end(), [&](pthread::thread &thrd){
    thrd.join();
  });

  message( "end reached");
  
}
