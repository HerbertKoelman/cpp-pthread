//
//  mutex.cpp
//  cpp-pthread
//
//  Created by herbert koelman on 18/03/2016.
//
//

#include "pthread/mutex.hpp"

namespace pthread {
  
  mutex::mutex () {
    auto rc = pthread_mutex_init (&_mutex, NULL);
    if ( rc != 0 ) {
      throw mutex_exception ( "In constructor of mutex pthread_mutex_init(&mutex, NULL) failed. ", rc );
    }
  }
  
  mutex::~mutex () {
    pthread_mutex_destroy (&_mutex);
  }
  
  void mutex::lock () {
    int rc = -1;
    if ((rc = pthread_mutex_lock ( &_mutex )) != 0){
      throw mutex_exception("pthread_mutex_lock failed.", rc);
    }
  }
  
  void mutex::try_lock () {
    
    auto rc = pthread_mutex_trylock ( &_mutex );
    if ( rc != 0 ){
      throw mutex_exception("pthread_mutex_trylock failed, already locked.", rc);
    }
  }
  
  void mutex::unlock () {
    auto rc = pthread_mutex_unlock ( &_mutex );
    if ( rc != 0 ){
      throw mutex_exception("pthread_mutex_unlock failed.", rc);
    }
  }

#ifdef __IBMCPP__
  mutex_exception::mutex_exception( const string message, const int pthread_error): pthread_exception(message, pthread_error) {
#else
  mutex_exception::mutex_exception( const string message, const int pthread_error): pthread_exception{message, pthread_error} {
#endif
  };
}
