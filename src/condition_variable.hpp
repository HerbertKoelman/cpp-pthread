//
//  csfi_consumer.hpp
//  substation
//
//  Created by herbert koelman on 13/03/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#ifndef pthread_condition_variable_hpp
#define pthread_condition_variable_hpp

#include <exception>
#include "mutex.hpp"

namespace pthread {
  
  /** pthread condition variable
   */
  class condition_variable : public mutex {
  public:
    
    int wait();
    
    int wait_for ( int millis = 0);
    
    int waitAndLock ( int millis  = 0 );
    
    /*
     The pthread_cond_signal() call unblocks at least one of the threads that are blocked
     on the specified condition variable cond (if any threads are blocked on cond).
     */
    int signal ();
    
    /*
     The pthread_cond_broadcast() call unblocks all threads currently blocked on the specified condition variable cond.
     */
    int broadcast ();
    
    // properties ------------------------------------------------------------
    
    // constructor/destructor ------------------------------------------------
    
    condition_variable ();
    virtual ~condition_variable();
    
  private:
    pthread_cond_t _condition;
  };
  
  class pthread_condition_exception: public exception {
    
  };
} // namespace pthread
#endif