//
//  mutex.hpp
//  cpp-pthread
//
//  Created by herbert koelman on 18/03/2016.
//
//

#ifndef pthread_mutex_hpp
#define pthread_mutex_hpp

// WARN pthread.h must be include as first header file of each source code file (see IBM's
// recommendation for more info p.285 chapter 8.3.1).
#include <pthread.h>

#include <exception>
#include <string>

#include "pthread/exceptions.hpp"


namespace pthread {

    /** \addtogroup concurrency
     *
     * @{
     */

    class condition_variable;

    /** The mutex class is a synchronization primitive that can be used to protect shared data from being simultaneously accessed by multiple threads.
     *
     * @author herbert koelman
     * @date 18/3/2016
     */
    class mutex {

        friend class condition_variable;

    public:
        /** Lock the mutex.
         *
         * The mutex object is locked (by calling pthread_mutex_lock). If the mutex is already locked, the calling thread blocks until the mutex becomes
         * available. This operation returns with the mutex object referenced by mutex in the locked state with the calling thread as its owner.
         *
         * @throw mutex_exception if error conditions preventing this method to succeed.
         * @see unlock
         * @see pthread_mutex_lock
         */
        void lock();

        /** Try to lock the mutex.
         *
         * Identical to lock method except that if the mutex object is currently locked (by any thread, including the
         * current thread), the call returns immediately.
         *
         * @return true if the mutex is locked, false is returned if the lock is held by some other thread.
         * @throw mutex_exception if error conditions preventing this method to succeed.
         * @see lock
         * @see unlock
         * @see pthread_mutex_trylock
         */
        bool try_lock();

        /** Release the mutex.
         *
         * Releases the mutex object referenced by mutex.  The manner in which a mutex is released is dependent upon the mutex's
         * type attribute. If there are threads blocked on the mutex object referenced by mutex when unlock is called,
         * resulting in the mutex becoming available, the scheduling policy is used to determine which thread shall acquire the mutex.
         *
         * @throw mutex_exception if error conditions preventing this method to succeed.
         * @see pthread_mutex_unlock
         */
        void unlock();

        /** create and initialize a mutex.
         *
         * > *WARN* the default mutex attributes are used.
         * 
         * @throw mutex_exception if error conditions preventing this method to succeed.
         * @see pthread_mutex_init
         */
        mutex();

        /** destroys the mutex.
         *
         * @see pthread_mutex_destroy
         */
        virtual ~mutex();

        /** not copy-assignable, so no copy constructor is needed.
         *
         */
        mutex(const mutex &) = delete;

        /** not copy-assignable
         *
         */
        void operator=(const mutex &) = delete;

    protected:
        /** pthread mutex structure */
        pthread_mutex_t _mutex;
    };

    /** @} */

} // namespace pthread


#endif /* mutex_hpp */
