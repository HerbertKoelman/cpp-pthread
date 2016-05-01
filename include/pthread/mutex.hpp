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

  
  class condition_variable;
  
  /** The mutex class is a synchronization primitive that can be used to protect shared data from being simultaneously accessed by multiple threads.
   *
   * @author herbert koelman
   * @date 18/3/2016
   */
  class mutex {
    
    friend class condition_variable;
  
  public:
    /**
     The mutex object is locked (by calling pthread_mutex_lock). If the mutex is already locked, the calling thread blocks until the mutex becomes
     available. This operation returns with the mutex object referenced by mutex in the locked state with the calling thread as its owner.
     @throw mutex_exception if error conditions preventing this method to succeed.
     */
    void lock ();
    
    /**
     The function pthread_mutex_trylock is identical to pthread_mutex_lock except that
     if the mutex object referenced by mutex is currently locked (by any thread,
     including the current thread), the call returns immediately.

     @throw mutex_exception if error conditions preventing this method to succeed.
     @see lock
     */
    void try_lock ();
    
    /**
     The pthread_mutex_unlock function releases the mutex object referenced by mutex.  The manner in which a mutex is released is dependent upon the mutex's type attribute. If there are threads blocked on the mutex object referenced by mutex when unlock is called, resulting in the mutex becoming available, the scheduling policy is used to determine which thread shall acquire the mutex.
     @throw mutex_exception if error conditions preventing this method to succeed.
     */
    void unlock ();
    
    /*
     Constructor/Desctructor
     @throw mutex_exception if error conditions preventing this method to succeed.
     */
    mutex ();
    virtual ~mutex ();
    
  protected:
    /** pthread mutex structure */
    pthread_mutex_t _mutex; 
  };

  /** throw to indicate that something went wrong with a mutex.
   */
  class mutex_exception: public pthread_exception {
  public:

    /** thrown when mutex actions fail
     *
     * @param message short description
     * @param pthread_errno error returned by the pthread function
     */
    mutex_exception( const std::string message, const int pthread_errno = 0) ;
    
  };
  
} // namespace pthread

#endif /* mutex_hpp */
