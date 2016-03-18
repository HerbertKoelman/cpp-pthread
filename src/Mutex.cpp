//
//  mutex.cpp
//  cpp-pthread
//
//  Created by herbert koelman on 18/03/2016.
//
//

#include "mutex.hpp"

namespace pthread {
  
  mutex::mutex () {
    if ( pthread_mutex_init (&_mutex, NULL) != 0 ) {
      throw mutex_exception ( "In constructor of mutex pthread_mutex_init(&mutex, NULL) failed." );
    }
  }
  
  mutex::~mutex () {
    pthread_mutex_destroy (&_mutex);
  }
  
  int mutex::lock () {
    return pthread_mutex_lock ( &_mutex );
  }
  
  int mutex::try_lock () {
    
    return pthread_mutex_trylock ( &_mutex );
  }
  
  int mutex::unlock () {
    
    return pthread_mutex_unlock ( &_mutex );
  }
  
  // exceptions & errors ------------
  
  mutex_exception::mutex_exception(const string message): _message{message}{
    
  }
  
  mutex_exception::~mutex_exception(){
    
  }
  
  const char *mutex_exception::what() const noexcept {
    return _message.c_str();
  }
}
