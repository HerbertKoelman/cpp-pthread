//
//  thread.hpp
//  cpp-pthread
//
//  Created by herbert koelman on 18/03/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#ifndef PTHREAD_THREAD_HPP
#define PTHREAD_THREAD_HPP

// must be include as first hearder file of each source code file (see IBM's
// recommandation for more info p.285 �8.3.1).
#include <pthread.h>

#include <iostream>
#include <string>
#include <functional>
#include <memory> // std::auto_ptr, std::unique_ptr
#include <list>
#include <cstddef>

#include "pthread/exceptions.hpp"
#include "pthread/mutex.hpp"
#include "pthread/lock_guard.hpp"


namespace pthread {

    /** \addtogroup threads Threads
     *
     * Set of classes that handle threads
     *
     * @author herbert koelman (herbert.koelman@me.com)
     * @{
     */

    /** Function used to startup a thread.
     *
     * it expects a reference to a runnable instance
     */
    void *thread_startup_runnable(void *);

    /** current status of a thread instance
    */
    enum class thread_status {
        not_a_thread, /*!< this not a thread (i.e. after a move operation) */
        a_thread      /*!< a valid thread */
    };

    /**
     * Interface of a runnable class.
     *
     * You can write code to be run through a Thread by implementing this interface.
     * @author herbert koelman (herbert.koelman@me.com)
     */
    class runnable {
    public:
        /**
         * This method must be overritten
         */
#if __cplusplus < 201103L
        virtual void run () throw() = 0 ;
#else

        virtual void run() noexcept = 0;

#endif

        virtual ~runnable() {
            // Intentionally unimplemented...
        };
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
     * @author herbert koelman (herbert.koelman@me.com)
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
         * @param stack_size thread stack size in bytes (default is 0 and means use default stack size)
         */
        thread(const runnable &runner, const std::size_t stack_size = 0);

        /** Move constructor.
         *
         * once moved the given thread is not a thread anymore (status is thread_status::not_a_thread)
         *
         * @param other thread that will be moved, on successfull completion, the passed argument is no longer a thread.
         */
        thread(thread &&other); // NOSONAR this is std interface and cannot be changed

        /** Copy constructor is flagged deleted because it makes no sense to copy a thread.
         */
        thread(const thread &) = delete;

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
        void join();

        /** A thread is considered joinable, if it has been allocated (`pthread_create`)
         *
         * @return true if this thread can be joined.
         */
        bool joinable() const { return _thread != 0; };

        /** @return the status of the thread (thread::status).
         */
        inline thread_status status() const {
            return _status;
        };

        /** thread's current stack size (pthread_attr_getstacksize).
         *
         * @return the stack size in bytes.
         */
         size_t stact_size();

        /** copy operator is flagged deleted,  copying doesn't make sense
         */
        thread &operator=(const thread &) = delete;

        /** move a thread to another thread.
         *
         * @param other thread to move, on completion it is not a thread anymore (thread_status::not_a_thread).
         * @return a thread
         */
        thread &operator=(thread &&other); // NOSONAR this is a std method signature

    private:

        /**
         * The cancel method requests the cancellation of the thread. The action depends on the
         * cancelability of the target thread:
         *
         * - If its cancelability is disabled, the cancellation request is set pending.
         * - If its cancelability is deferred, the cancellation request is set pending till the thread reaches a cancellation point.
         * - If its cancelability is asynchronous, the cancellation request is acted upon immediately; in some cases, it may result in unexpected behaviour.
         *
         * The cancellation of a thread terminates it safely, using the same termination
         * procedure as the pthread_exit subroutine.
         */
        int cancel();

        /** Exchanges the underlying handles of two thread objects.
         *
         * @param other the thread to swap with, on completion other is not a thread.
         */
        void swap(thread &other);

        pthread_t          _thread; //!< thread identifier
        pthread_attr_t     _attr;   //!< thread attributes (stack size, ...)
        pthread_attr_t    *_attr_ptr; //!< pthread attribute pointer (null, if pthread_attr_t was not initialized) (NOSONAR)
        thread_status      _status; //!< thread status (@see thread_status)
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
     *       auto delay = _sleep; // use sleep seconds to calculate point in time timeou
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
     *     pthread::this_thread::sleep_for(200);
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
     * @author herbert koelman (herbert.koelman@me.com)
     */
    class abstract_thread : public runnable {
    public:
        /**
         * setup thread base.
         *
         * @param stack_size thread's stack size (default 0 which means use PTHREAD_STACK_MIN)
         */
        explicit abstract_thread(const std::size_t stack_size = 0);

        /** not copy-assignable */
        abstract_thread(const abstract_thread &) = delete;

        /** deallocate thread.
         */
        virtual ~abstract_thread();

        /** start running the `run()` method in a new thread.
         */
        void start();

        /** joins this thread.
         *
         * @throw pthread_exception if deadlock conditions are detected.
         */
        void join();

        /** @return true if this thread can be joined.
         */
        bool joinable() const;


        /** not copy-assignable */
        void operator=(const abstract_thread &) = delete;

    private:
        pthread::thread *_thread;
        std::size_t _stack_size;
    };

    /** Group of abstract_threads pointers.
     *
     * This helper class is in charge of handling group of threads as a whole. Method in this class apply to all threads in the group.
     *
     * **When destroyed, a thread_group deletes the thread that were registered/added to it.**
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
     * @author herbert koelman (herbert.koelman@me.com)
     * @since 1.3
     */
    class thread_group {
    public:
        /** Setup a thread container/list.
         *
         * @param destructor_joins_first if true then destructor tries to wait for all registered threads to join the calling one before deleting thread instances.
         */
#if __cplusplus < 201103L
        explicit thread_group( bool destructor_joins_first = false ) throw();
#else
        explicit thread_group(bool destructor_joins_first = false) noexcept;
#endif

        /** not copy-assignable */
        thread_group(const thread_group &) = delete;

        /** delete all abstract_thread referenced by the thread_group.
         *
         * If destructor_joins_first is true then the method abstract_thread::join() is called before deleting the referenced abstract_thread.
         */
        virtual ~thread_group();

        /** @param thread add/register a thread.
         */
        void add(abstract_thread *thread);

        /** Start running all registered threads.
         *
         * @see add(abstract_thread *thread)
         */
        void start();

        /** Wait for all registered threads to join the caller thread.
         *
         * The method iterates the list of abstract_thread and calls the join method of each entry found.
         *
         * @see abstract_thread::join()
         */
        void join();

        /**
         * @return the number of threads in the thread_group
         */
        unsigned long size();

        /** @return current value of destructor_joins_first property. If true the destructor shall try to join registered threads before destroying them.
         */
        const bool destructor_joins_first() { return _destructor_joins_first; };

        /** not copy-assignable */
        void operator=(const thread_group &) = delete;

    private:
        std::list<pthread::abstract_thread *> _threads;
        bool _destructor_joins_first;
    };

    /** \namespace pthread::this_thread
     *
     * helper functions
     * @author herbert koelman (herbert.koelman@me.com)
     */
    namespace this_thread {
        /** \addtogroup threads Threads
         * @{
         */
        /** let the current thread sleep for the given milliseconds.
         *
         * @param millis time to wait.
         */
        void sleep_for(const int millis);

        /** @return current thread id/reference */
        pthread_t get_id();
        /** @} */
    }

    /** @} */

} // namespace pthread

#endif /* thread_hpp */
