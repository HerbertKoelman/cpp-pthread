
#include "pthread/read_write_lock.hpp"


namespace pthread {

  void write_lock::lock (){
    int ret = pthread_rwlock_wrlock(&_rwlock);
    if ( ret != 0 ){
      throw read_write_lock_exception("Try get write lock failed.", ret);
    }
  }

  void write_lock::try_lock (){
    int ret = pthread_rwlock_trywrlock(&_rwlock);
    if ( ret != 0 ){
      throw read_write_lock_exception("Try get write lock failed.", ret);
    }
  }

  write_lock::write_lock (){//:read_lock(){
    // intentional
    // the read/write lock is created by the base class read_lock
  }

  write_lock::~write_lock(){
    // intentional... base class is in charge of freeing allocated resources
  }

  // read_lock -----------------------------
  //
  void read_lock::lock (){
    int ret = pthread_rwlock_rdlock(&_rwlock);
    if ( ret != 0 ){
      throw read_write_lock_exception("Try get read lock failed.", ret);
    }
  }

  void read_lock::try_lock (){
    int ret = pthread_rwlock_tryrdlock(&_rwlock);
    if ( ret != 0 ){
      throw read_write_lock_exception("Try get read lock failed.", ret);
    }
  }

  void read_lock::unlock (){
    int ret = pthread_rwlock_unlock(&_rwlock);
    if ( ret != 0 ){
      throw read_write_lock_exception("failed to unlock read/read lock.", ret);
    }
  }

  read_lock::read_lock (){
    int ret = pthread_rwlock_init(&_rwlock, NULL);
    if ( ret != 0 ){
      throw read_write_lock_exception("Failed to init read/write lock", ret);
    }
  }

  read_lock::~read_lock (){
    pthread_rwlock_destroy(&_rwlock);
  }


  /** @} */
} // namespace pthread
