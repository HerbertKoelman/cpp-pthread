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

#include "pthread/mutex.hpp"

namespace pthread {
  
  using namespace std ;
  
  /**
   This class was designed to encapsulate a mutex and automatically control the lock attribute.
   The ScopedMutex lock the associated mutex once we instanciate the class and the lock is automatically unlocked
   once the object is destroyed. This allow us to correlate the lock with the scope of the object.
   */
  class lock_guard {
    
  public:
    /*
     constructors and destructors. The constructor is forced to only accept a mutex object or any object of a subclass.
     */
    explicit lock_guard(mutex &m) : _mutex(&m) {
      _mutex->lock();
    }
    
    ~lock_guard() {
      _mutex->unlock();
    }
    
    /*
     Desabling the = operator.
     */
    void operator=(lock_guard &);
    
    mutex *mutex() const { return _mutex ;};
    
  private:
    pthread::mutex *_mutex;
  };
  
} // namespace pthread

#endif /* pthread_lock_guard_H */
