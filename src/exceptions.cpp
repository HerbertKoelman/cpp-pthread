//
//  exceptions.hpp
//  cpp-pthread
//
//  Created by herbert koelman on 18/03/2016.
//
//

#include "pthread/exceptions.hpp"

namespace pthread {
  
  using namespace std ;
  
  pthread_exception::pthread_exception( const string &message, const int pthread_errno ): _message(message.c_str()), _pthread_errno(pthread_errno){
  };

  pthread_exception::pthread_exception( const char *message, const int pthread_errno ): _message(message), _pthread_errno(pthread_errno){
  };

  pthread_exception::~pthread_exception(){
  };
    
#if __cplusplus < 201103L
  const char *pthread_exception::what() const throw() {
    return _message;
  };
#else
  const char *pthread_exception::what() const noexcept {
    return _message;
  };
#endif    

  int pthread_exception::pthread_errno(){
    return _pthread_errno ;
  };
    
  const char *pthread_exception::pthread_errmsg(){
    return strerror(_pthread_errno );
  };
    
  // timeout_exception -----------------------------
  //
  timeout_exception::timeout_exception(const string &message): pthread_exception(message, ETIMEDOUT){
  };
    
  // mutex exception -----------------------------
  //
  mutex_exception::mutex_exception( const std::string &message, const int pthread_error): pthread_exception(message, pthread_error) {
  };
    
  // condition_variable_exception -----------------------------
  //
  condition_variable_exception::condition_variable_exception( const std::string &message, const int pthread_error): pthread_exception(message, pthread_error){
  };
  
  // thread exception -----------------------------
  //
  thread_exception::thread_exception(const string &message, const int pthread_error): pthread_exception(message, pthread_error){
  }
  
}
