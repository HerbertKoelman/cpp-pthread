//
//  main.cpp
//  pthread_tests
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
#include "ibm.hpp"

pthread::condition_variable condition;
pthread::mutex mtx;
pthread::mutex cout_mtx;
int counter = 0;

void message ( const std::string m){
  pthread::lock_guard<pthread::mutex> lck(cout_mtx);
  std::cout << m << std::endl;
}

class worker: public pthread::runnable {
public:
  
  worker(const std::string m = "anonymous worker", int sleep = 2*1000): msg(m), _sleep(sleep){
    
  };
  
  ~worker(){
    message("deallocating worker");
  };
  
  void run() __NOEXCEPT__ __OVERRIDE__ {
    
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
  };
  
private:
  std::string    msg ;
  int            _sleep;
};

int main(int argc, const char * argv[]) {
  
  pthread::string dummy;
  worker w("laura");
  pthread::thread t0;
  pthread::thread t2(w);
  t0 = ibm::move(t2) ;
  t0.join();
  
  std::auto_ptr<pthread::thread> pt0(new pthread::thread(w));
  pt0->join();

//  std::list<std::auto_ptr<pthread::thread>> threads;
  std::list<pthread::thread> threads;
  for (auto x = 20 ; x > 0 ; x--){
//    threads.push_back(std::auto_ptr<pthread::thread>(new pthread::thread(w)));
    threads.push_back(ibm::move(pthread::thread(w)));
  }
//  threads.push_back(pthread::thread(w));
//  threads.push_back(pthread::thread(w));
//  threads.push_back(pthread::thread(w));
//  threads.push_back(pthread::thread(worker("herbert's worker")));
//  
//  message("increment counter");
//  for ( auto x = 20000 ; x > 0 ; x--){
//    pthread::lock_guard<pthread::mutex> lck(mtx);
//    counter++ ;
//    condition.notify_one();
//  }
//  message("main is waiting for threads to finish");
//  
//  std::list<std::auto_ptr<pthread::thread>>::iterator iterator;
//  for(iterator = threads.begin(); iterator != threads.end(); iterator++){
//    message("join a thread (loop)");
//    (*iterator)->join();
//  }

  message( "end reached");
  
}
