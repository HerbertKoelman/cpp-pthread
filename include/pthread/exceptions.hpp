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
   * @example exceptions_tests.cpp
   */
  class pthread_exception: public std::exception {
  public:

    /** Base class of exception thrown by this module.
     *
     * If an error_number is passed, then the corresponding system error message is catenated to the message string. If the message
     * string ends witht a period, then a space (`" "`) is inserted between the message and the system error message. Else
     * a period + space is inserted (`". "`).
     *
     * @param message error message
     * @param error_number associated error number (default is 0).
     */
    explicit pthread_exception( const std::string &message, const int error_number = 0 );

    virtual ~pthread_exception();

    /** @return exception's error message. */
#if __cplusplus < 201103L
    virtual const char *what() const throw() ;
#else
    virtual const char *what() const noexcept override ;
#endif

    /** @return system error number that was at the origin of the error */
    int error_number() const;

    /** @return corresponding system error message (`strerror`) */
    const char *error_message() const;

  private:
    std::string  _message;
    int          _error_number;

  };

  /** pthread operation timed out.
   *
   * @example exceptions_tests.cpp
   */
  class timeout_exception: public pthread_exception{
  public:
    /**
     * associated error number (default is 0).
     * @param message what was timed out.
     */
    explicit timeout_exception(const std::string &message);

  };

  /** Something went wrong with a mutex.
   *
   * @example exceptions_tests.cpp
   */
  class mutex_exception: public pthread_exception {
  public:

    /** Mutex actions fail
     *
     * mutex related may return:
     * - EBUSY The implementation has detected an attempt to destroy the object referenced by mutex while it is locked
     *         or referenced (for example, while being used in a pthread_cond_timedwait() or pthread_cond_wait()) by another thread.
     * - EINVAL The value specified by mutex is invalid.
     * - EAGAIN The system lacked the necessary resources (other than memory) to initialize another mutex.
     * - ENOMEM Insufficient memory exists to initialize the mutex.
     * - EPERM The caller does not have the privilege to perform the operation.
     * - EBUSY The implementation has detected an attempt to reinitialize the object referenced by mutex, a previously
     *         initialized, but not yet destroyed, mutex.
     * - EINVAL The value specified by attr is invalid.
     *
     * @param message short description
     * @param error_number an error code returned by a call to a pthread function.
     */
    explicit mutex_exception( const std::string &message, const int error_number = 0) ;

  };

  /** throw to indicate that something went wrong with a read/write lock
   * @example exceptions_tests.cpp
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
   * @example exceptions_tests.cpp
   */
  class condition_variable_exception: public pthread_exception {
  public:

    /** thrown when condition variable actions fail
     *
     * @param message short description
     * @param pthread_errno error returned by the pthread function
     */
    explicit condition_variable_exception( const std::string &message = "conditional_variable_exception", const int pthread_errno = -1);
  };

  /** thrown to indicate that something went wrong with a thread */
  class thread_exception: public pthread_exception {
  public:
    /**
     * @param message short error description.
     * @param pthread_error value return by a function in the pthread library.
     */
    explicit thread_exception(const std::string &message, const int pthread_error = -1);
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

    /** thrown when a get operation times ou
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
