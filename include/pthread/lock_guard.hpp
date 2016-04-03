//
//  lock_guard.hpp
//  lock_guard
//
//  Created by herbert koelman on 18/03/2016.
//
//

#ifndef pthread_lock_guard_H
#define pthread_lock_guard_H

#include <pthread.h>

#include "pthread/config.h"

#include "pthread/mutex.hpp"

namespace pthread {
  
  using namespace std ;
  
  /**
   This class was designed to encapsulate a mutex and automatically control the lock attribute.

   The lock_guard lock the associated mutex once we instanciate the class and the lock is automatically unlocked
   once the object is destroyed. This allow us to correlate the lock with the scope of the object.
   
   @author herbert koelman
   */
  template<class MutexType>
  class lock_guard {
    
  public:
    /**
     * The constructor is forced to only accept a mutex object or any object of a subclass.
     * 
     * The mutex is locked up completion.
     *
     * @param m reference to a valid pthread::mutex
     */
    explicit lock_guard(mutex &m) : _mutex(&m) {
      _mutex->lock();
    }
    
    /** release the guared mutex.
     */
    ~lock_guard() {
      _mutex->unlock();
    }
    
    /*
     Desabling the = operator.
     */
    void operator=(lock_guard &) = delete;
    
    /** @return a const reference to the guarded mutex */
    MutexType *mutex() const { return _mutex ;};
    
  private:
    MutexType *_mutex;
  };
  
} // namespace pthread

#endif /* pthread_lock_guard_H */
