//
//  mutex.hpp
//  cpp-pthread
//
//  Created by herbert koelman on 18/03/2016.
//
//

#ifndef pthread_mutex_hpp
#define pthread_mutex_hpp

#include <pthread.h>
#include <exception>
#include <string>

#include "pthread/config.h"

#include "pthread/pthread_exception.hpp"

namespace pthread {

  using namespace std ;
  
  class condition_variable;
  
  class mutex {
    
    friend class condition_variable;
  
  public:
    /**
     The mutex object is locked by calling pthread_mutex_lock. If
     the mutex is already locked, the calling thread blocks until the mutex becomes
     available. This operation returns with the mutex object referenced by mutex in
     the locked state with the calling thread as its owner.
     */
    void lock ();
    
    /**
     The function pthread_mutex_trylock is identical to pthread_mutex_lock except that
     if the mutex object referenced by mutex is currently locked (by any thread,
     including the current thread), the call returns immediately.
     */
    void try_lock ();
    
    /**
     The pthread_mutex_unlock function releases the mutex object referenced by mutex.
     The manner in which a mutex is released is dependent upon the mutex's type
     attribute. If there are threads blocked on the mutex object referenced by mutex
     when pthread_mutex_unlock is called, resulting in the mutex becoming available,
     the scheduling policy is used to determine which thread shall acquire the mutex.
     (In the case of PTHREAD_mutex_RECURSIVE mutexes, the mutex becomes available when
     the count reaches zero and the calling thread no longer has any locks on this
     mutex).
     */
    void unlock ();
    
    /*
     Constructor/Desctructor
     */
    mutex ();
    virtual ~mutex ();
    
  protected:
    pthread_mutex_t _mutex;
  };

  class mutex_exception: public pthread_exception {
  public:
    mutex_exception( const string message, const int pthread_error = 0) ;
    
  };
  
} // namespace pthread

#endif /* mutex_hpp */
