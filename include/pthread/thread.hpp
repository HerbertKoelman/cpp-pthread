//
//  thread.hpp
//  cpp-pthread
//
//  Created by herbert koelman on 18/03/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#ifndef pthread_thread_hpp
#define pthread_thread_hpp
#include <iostream>
#include <pthread.h>
#include <string>
#include <functional>
#include <memory> // std::auto_ptr, std::unique_ptr
#include <list>

#include "pthread/config.h"

#include "pthread/pthread_exception.hpp"
#include "pthread/mutex.hpp"
#include "pthread/lock_guard.hpp"

namespace pthread {
  
  /** function used to startup a thread.
   * 
   * it expects a reference to a runnable instance
   */
  void *thread_startup_runnable (void *);
  
  /** current status of a thread instance
  */
  enum class thread_status{
    not_a_thread, /*!< this not a thread (i.e. after a move operation) */
    a_thread      /*!< a valid thread */
  };
  
  /**
   * Interface of a runnable class.
   *
   * You can write code to be run through a Thread by implementing this interface.
   */
  class runnable {
  private:
    
  public:
    /**
     * This method must be overriden
     */
#if __cplusplus < 201103L
    virtual void run () throw() = 0 ;
#else
    virtual void run () noexcept = 0 ;
#endif
  };
  
  /**
   * Handles POSIX (Portable Operating System Interface) threads.
   *
   * <pre><code>
   *     class reader_thread: public runnable {
   *     public:
   *       void run() {...}
   *     };
   *
   *     reader_thread rt;
   *     thread t{rt};
   *     t.join();
   * </code></pre>
   */
  class thread {
  public:
    
    /**
     * create a new thread status is no_a_thread
     */
    thread();
    
    /** create a new thread
     *
     * The new thread is made runnable, and will start executing the run routine, with.
     *
     * @param runner a class that implements the runnable interface.
     */
    thread( const runnable &runner );
    
//    /** Starts running given function in a new trhread.
//     * 
//     * param f function to run in thread
//     * param args function paramleters/arguments.
//     */
//    template<class Function, class... Args> explicit thread(Function&& f, Args&&... args);
    
    /** move contructor
     *
     * once moved this not a thread anymore. Status is thread_status::not_a_thread
     *
     * @param other thread that will be moved, on completion other is no longer a thread.
     */
    thread( thread&& other);
    
    /** copy constructor is flagged deleted because it makes no sense to copy a thread.
     */
    thread(const thread &) = delete ;
    
    /**
     * cleanup thread ressources.
     */
    virtual ~thread();
    
    /**
     * The join method blocks the calling thread until the thread terminates. The target thread's
     * termination status is returned in the status parameter.
     *
     * If the target thread is already terminated, the method returns immediately.
     *
     * This method does not itself cause a thread to be terminated.
     *
     * @throws pthread_exception if this is not a thread or if thread_id == this_thread::get_id().
     */
    int join();
    
    /** @return true if this thread can be joined.
     */
    bool joinable() const { return _thread != 0 ;};
    
    /**
     * The cancel method requests the cancellation of the thread. The action depends on the
     * cancelability of the target thread:
     *
     * o If its cancelability is disabled, the cancellation request is set pending.
     * o If its cancelability is deferred, the cancellation request is set pending till the thread reaches a cancellation point.
     * o If its cancelability is asynchronous, the cancellation request is acted upon immediately; in some cases, it may result in unexpected behaviour.
     *
     * The cancellation of a thread terminates it safely, using the same termination
     * procedure as the pthread_exit subroutine.
     */
    int cancel();
    
    /** @return the status of the thread (thread::status).
     */
    inline thread_status status() { return _status ;};
    
    /** copy operator is flagged deleted,  copying doesn't make sense
     */
    thread& operator=(const thread&) = delete ;
    
    /** move a thread to another thread.
     *
     * @param other thread to move, on completion it is not a thread anymore (thread_status::not_a_thread).
     * @return thread 
     */
    thread& operator=(thread&& other);
    
  private:

    /** Exchanges the underlying handles of two thread objects.
     *
     * @param other the thread to swap with, on completion other is not a thread.
     */
    void swap ( thread& other );
    
    pthread_t      _thread;
    
    thread_status  _status;
  };
  
  /** base class of a thread.
   * 
   * utility class, that wraps a thread.
   * <pre><code>
   * class worker: public pthread::abstract_thread {
   * public:
   * 
   *   worker(const std::string m = "anonymous worker", int sleep = 2*1000): msg(m), _sleep(sleep){
   *   };
   *   
   *   ~worker(){
   *   };
   *   
   *   void run() noexcept override {
   *     { // critical section scope
   *       pthread::lock_guard<pthread::mutex> lck(mtx);
   * 
   *       bool stop_waiting = true; // if lambda syntax is not availbale then use this kind of implementation
   *       auto delay = _sleep; // use sleep seconds to calculate point in time timeout
   *       while ( ! (stop_waiting = (counter >= 10000)) && (condition.wait_for(mtx, delay) == pthread::cv_status::no_timeout)){
   *         delay = -1 ; // if timeout millis is negatif, then we keep last timeout calculation.
   *       }
   *   
   *       if ( counter >= 10000 ) {
   *         message("worker class, counter >= 10000");
   *       } else {
   *         message("worker class, counter < 10000");
   *       }
   *     } // end of critical section
   * 
   *     pthread::this_thread::sleep(200);
   *   };
   * 
   * private:
   *   std::string    msg ;
   *   int            _sleep;
   * };
   *
   * int main(int argc, const char * argv[]) {
   * 
   *   pthread::thread_group threads(true); // indicate that we want to join referenced threads when deallocating this instance.
   *   for (auto x = 10 ; x > 0 ; x--){
   *     threads.add( new worker("herbert"));
   *   }
   *   
   *   threads.start(); // start running all threads
   * 
   *   for ( auto x = 20000 ; x > 0 ; x--){
   *     pthread::lock_guard<pthread::mutex> lck(mtx);
   *     counter++ ;
   *   }
   * 
   *   condition.notify_all();
   * }
   *
   * </code></pre>
   */
  class abstract_thread: public runnable {
  public:
    virtual ~abstract_thread();
    
    /** start running the `run()` method in a new thread.
     */
    void start();
    
    /** joins this thread.
     *
     * an exception is thrown if deadlock condition are detected.
     */
    int join() { return _thread->join() ;};
    
  private:
    pthread::thread *_thread;
  };
  
  /** Group of abstract_threads pointers.
   *
   * This helper class is in charge of handling group of threads as a whole. Method in this class apply to all threads in the group.
   *
   * **A thread_group deletes the thread that were registered/added to it.**
   *
   * <pre><code>
   * int main(int argc, const char * argv[]) {
   * 
   *   pthread::thread_group threads; // this instance will free any registered thread when it will get out of scope
   * 
   *   for (auto x = 10 ; x > 0 ; x--){
   *     threads.add( new worker("herbert")); // register threads, they will run when start() is called
   *   }
   * 
   *   threads.start(); // start running all threads
   *   threads.join(); // wait for registered threads to join
   * } // scope end
   * 
   * </code></pre>
   */
  class thread_group{
  public:
    /** Setup a thread container/list.
     *
     * @param destructor_joins_first if true then destructor tries to wait for all registered threads to join the calling one before deleting thread instances.
     */
#if __cplusplus < 201103L
    thread_group( bool destructor_joins_first = false ) throw();
#else
    thread_group( bool destructor_joins_first = false ) noexcept;
#endif
    
    /** delete all abstract_thread referenced by the thread_group.
     *
     * If destructor_joins_first is true then the method abstract_thread::join() is called before deleting the referenced abstract_thread.
     */
    virtual ~thread_group();
    
    /** @param thread add/register a thread to the group.
     */
    void add(abstract_thread *thread);
    
    /** start run all registered threads.
     * @see add(abstract_thread *thread)
     */
    void start();
    
    /** what for all threads to join the caller of this method.
     */
    void join();
    
    unsigned long size();
    
    /** return if thread_group should wait for all referenced abstract_thread terminate
     */
    const bool destructor_joins_first(){ return _destructor_joins_first;};
    
  private:
    std::list<pthread::abstract_thread*> _threads;
    bool                                 _destructor_joins_first;
  };
  
  // exception & errors --------
  
  /** thrown to indicate that something went wrong with a thread */
  class thread_exception: public pthread_exception {
  public:
    /**
     * @param message short error description.
     * @param pthread_error value return by a function in the pthread library.
     */
    thread_exception(const string message, const int pthread_error = 0);
  };
  
  /** \namespace pthread::this_thread
   * 
   * helper functions
   */
  namespace this_thread{

    /** let the current thread sleep for the given milliseconds.
     *
     * @param millis time to wait.
     */
    void sleep(const int millis);
    
    /** @return current thread id/reference */
    pthread_t get_id() ;
  }

  // template implementations ------

//  // GCC magic
//  template <size_t...> struct __tuple_indices {};
//  
//  template <size_t _Sp, class _IntTuple, size_t _Ep>
//  struct __make_indices_imp;
//  
//  template <size_t _Sp, size_t ..._Indices, size_t _Ep>
//  struct __make_indices_imp<_Sp, __tuple_indices<_Indices...>, _Ep>
//  {
//    typedef typename __make_indices_imp<_Sp+1, __tuple_indices<_Indices..., _Sp>, _Ep>::type type;
//  };
//  
//  template <size_t _Ep, size_t ..._Indices>
//  struct __make_indices_imp<_Ep, __tuple_indices<_Indices...>, _Ep>
//  {
//    typedef __tuple_indices<_Indices...> type;
//  };
//  
//  template <size_t _Ep, size_t _Sp = 0>
//  struct __make_tuple_indices
//  {
//    static_assert(_Sp <= _Ep, "__make_tuple_indices input error");
//    typedef typename __make_indices_imp<_Sp, __tuple_indices<>, _Ep>::type type;
//  };
//
//  template <class _Fp, class ..._Args>
//  inline   auto __invoke(_Fp&& __f, _Args&& ...__args) -> decltype(std::forward<_Fp>(__f)(std::forward<_Args>(__args)...)){
//    return std::forward<_Fp>(__f)(std::forward<_Args>(__args)...);
//  }
//  
//  template <class _Tp, class ..._Args> struct __invoke_return {
//    typedef decltype(__invoke(std::declval<_Tp>(), std::declval<_Args>()...)) type;
//  };
//  
//  template <class _Tp> inline typename decay<_Tp>::type __decay_copy(_Tp&& __t) {
//    return std::forward<_Tp>(__t);
//  }
//  
//  template <class _Fp, class ..._Args, size_t ..._Indices> inline void __thread_execute(tuple<_Fp, _Args...>& __t, pthread::__tuple_indices<_Indices...>){
//    pthread::__invoke(std::move(std::get<0>(__t)), std::move(std::get<_Indices>(__t))...);
//  }
//  
//  template <class _Fp>
//  void* __thread_proxy(void* __vp) {
//    
////    didn't find out what this is for !!
////    __thread_local_data().reset(new __thread_struct);
//    
//    // get information from the given unique_ptr to my tuple
//    std::unique_ptr<_Fp> __p(static_cast<_Fp*>(__vp));
//    
//    // create a list indices of elements in the tuple starting at 1 (0 is the function pointer).
//    // The list of indices is create when object of this type is created
//    typedef typename pthread::__make_tuple_indices<tuple_size<_Fp>::value, 1>::type _Index;
//    
//    // Cut/pasted from GCC imlementation, quite mysterious but it seem to work !!!
//    pthread::__thread_execute(*__p, _Index{});
//    return nullptr;
//  }
//
//  template<class Function, class... Args>
//  thread::thread(Function&& func, Args&&... args){
//    int rc = 0 ;
//    pthread_attr_t attr{0};
//    
//    // global pointer to a tuple made of function and it's arguments
//    typedef std::tuple<typename std::decay<Function>::type, typename std::decay<Args>::type...> _Gp;
//    
////    This was the original GCC implementation, but I don't really understand this construction, so I simplfied it !!!!
//    std::unique_ptr<_Gp> __p(new _Gp(pthread::__decay_copy(std::forward<Function>(func)), pthread::__decay_copy(std::forward<Args>(args))...));
////    std::unique_ptr<_Gp> __p(new _Gp(std::forward<Function>(func), std::forward<Args>(args))...);
//    
//    //auto work = std::bind(func, std::forward<Args>(args)...); // moved in thread_proxy
//    
//    /* Initialize and set thread detached attribute */
//    if ( (rc = pthread_attr_init(&attr)) != 0){
//      throw thread_exception{"pthread_attr_init failed.", rc };
//    }
//    
//    if ( (rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE)) != 0 ){
//      throw thread_exception{"pthread_attr_setdetachstate failed.", rc };
//    }
//    
//    if ((rc = pthread_create(&_thread, &attr, &__thread_proxy<_Gp>, __p.get())) != 0) {
//      throw thread_exception{"pthread_create failed.", rc };
//    } else {
//      __p.release();
//      _status = thread_status::a_thread;
//      pthread_attr_destroy(&attr);
//    }
//    
//  }
  
  
} // namespace pthread
#endif /* thread_hpp */
