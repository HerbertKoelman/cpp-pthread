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

  pthread_exception::pthread_exception( const string &message, const int pthread_errno ): _message(message), _pthread_errno(pthread_errno){
      if ( _pthread_errno != 0 ){
          _message = _message + " " + pthread_errmsg() ;
      }
  };

  pthread_exception::~pthread_exception(){
    // Intentionally unimplemented...
  };

#if __cplusplus < 201103L
  const char *pthread_exception::what() const throw() {
#else
  const char *pthread_exception::what() const noexcept {
#endif

    return _message.c_str();
  };

  int pthread_exception::pthread_errno(){
    return _pthread_errno ;
  };

  const char *pthread_exception::pthread_errmsg() const {
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

  // read_write_lock exception -----------------------------
  //
  read_write_lock_exception::read_write_lock_exception( const std::string &message, const int pthread_error): pthread_exception(message, pthread_error) {
  };

  // condition_variable_exception -----------------------------
  //
  condition_variable_exception::condition_variable_exception( const std::string &message, const int pthread_error): pthread_exception(message, pthread_error){
  };

  // thread exception -----------------------------
  //
  thread_exception::thread_exception(const string &message, const int pthread_error): pthread_exception(message, pthread_error){
  }

  namespace util {

    // synchonized queue
    //
    queue_exception::queue_exception(const std::string &msg): _message(msg){
    };

#if __cplusplus < 201103L
    const char *queue_exception::what() const throw(){
#else
    const char *queue_exception::what() const noexcept {
#endif
        return _message.c_str();
      };

    queue_full::queue_full(const std::string &msg): queue_exception(msg){
    };

    queue_timeout::queue_timeout(const std::string &msg): queue_exception(msg){
    };
  }; //namespace util
} // namespace pthread
