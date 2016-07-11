//
//  exceptions.hpp
//  cpp-pthread
//
//  Created by herbert koelman on 18/03/2016.
//
//

#ifndef PTHREAD_EXCEPTIONS_HPP
#define PTHREAD_EXCEPTIONS_HPP

#include <pthread.h>

#include <cerrno>
#include <string>    // std::string
#include <cstring>
#include <exception> // std::exception


namespace pthread {
  
//  using namespace std ;

  /** \addtogroup exception Errors and exceptions
   *
   * Threading related errors and exceptions
   * @author herbert koelman (herbert.koelman@me.com)
   *
   * @{
   */
  
  /** general purpose pthread exception.
   * @example tests/exceptions-tests.cpp
   */
  class pthread_exception: public std::exception {
    
  public:
    
    /**
     * @param message error message
     * @param pthread_errno a pthread function return code.
     */
    pthread_exception( const std::string &message, const int pthread_errno = -1 );

    virtual ~pthread_exception();
    
    /** @return exception's error message. */
#if __cplusplus < 201103L
    virtual const char *what() const throw() ;
#else
    virtual const char *what() const noexcept override ;
#endif    

    /** @return pthread error code that was at the orgin of the error */
    virtual int pthread_errno() ;
    
    /** @return related pthread_errno error message using strerror */
    virtual const char *pthread_errmsg() ;
    
  private:
    std::string  _message;
    //const char  *_message;
    int          _pthread_errno;
    
  };
  
  /** pthread operation timed out.
   */
  class timeout_exception: public pthread_exception{
  public:
    /** thrown when a time out occurs.
     *
     * @param message timeout condition
     */
    explicit timeout_exception(const std::string &message);

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
    explicit mutex_exception( const std::string &message, const int pthread_errno = -1) ;
    
  };

  /** throw to indicate that something went wrong with a read/write lock
   */
  class read_write_lock_exception: public pthread_exception {
  public:

    /** thrown when read_write_lock actions fail
     *
     * @param message short description
     * @param pthread_errno error returned by the pthread function
     */
    explicit read_write_lock_exception( const std::string &message, const int pthread_errno = -1) ;
    
  };

  /** Condition variable exception
   */
  class condition_variable_exception: public pthread_exception {
  public:

    /** thrown when mutex actions fail
     *
     * @param message short description
     * @param pthread_errno error returned by the pthread function
     */
    condition_variable_exception( const std::string &message, const int pthread_errno = -1);

    virtual ~condition_variable_exception(){
      // Intentionally unimplemented...
    };
  };
  
  /** thrown to indicate that something went wrong with a thread */
  class thread_exception: public pthread_exception {
  public:
    /**
     * @param message short error description.
     * @param pthread_error value return by a function in the pthread library.
     */
    thread_exception(const std::string &message, const int pthread_error = -1);
  };

  namespace util {

    /** \addtogroup exception
     *
     * @{
     */
    
    /** thrown when something goes wrong in a synchonized queue.
     */
    class queue_exception : public std::exception {
    public:
      /**
       * new instance.
       *
       * @param msg explanatory message.
       */
      explicit queue_exception(const std::string &msg = "queue_error occured.");

      virtual ~queue_exception(){
        // intintional...
      };

      /** @return an explanatory message
       */
#if __cplusplus < 201103L
      virtual const char *what() const throw();
#else
      virtual const char *what() const noexcept override;
#endif
      
    protected:
      std::string _message; //!< message buffer
    };
    
    /** thrown when the queue's max_size is reached
     */
    class queue_full: public queue_exception{
    public:
      /**
       * new instance.
       *
       * @param msg explanatory message.
       */
      explicit queue_full(const std::string &msg = "synchronized_queue full.");
      
    };
    
    /** thrown when a get operation times out
     */
    class queue_timeout: public queue_exception{
    public:
      /**
       * new instance.
       *
       * @param msg explanatory message.
       */
      explicit queue_timeout(const std::string &msg = "synchronized_queue get/put timed out.");
      
    };
    /** @} */
  }; // namespace util
  
  /** @} */
  
} // namespace pthread

#endif
