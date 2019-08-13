//! \file
//  sync_queue.hpp
//  substation
//
//  Created by herbert koelman on 25/02/2016.
//  Copyright © 2016 urbix-software. All rights reserved.
//

#ifndef pthread_synchronized_queue_hpp
#define pthread_synchronized_queue_hpp

#include <list>           // std::list

#include "pthread/pthread.hpp"

#if __cplusplus < 201103L
#else

#  include <atomic>

#endif

#include <string>


namespace pthread {

    /** \namespace pthread::util utility classes
     *
     * Set of utility classes.
     */
    namespace util {

        /** \addtogroup util Utility classes
         *
         * @{
         */

        /** synchronized fixed sized queue.
         *
         * This container can be used to pass items between threads in synchronized way. When the maximun capacity of the sync_queue is
         * reached the put operations are blocked until some space is available. The put methods try to pop a message off of the sync_queue.
         *
         * @author herbert koelman (herbert.koelman@me.com)
         * @tparam T type of items that the queue can handle.
         * @since 1.5
         */
        template<typename T> class sync_queue {
        public:

            /** Put an item in the queue.
             *
             * If the queue max size is reached, then the operation waits until a size is smaller then max size again.
             *
             * @param item item to store in the queue
             */
            void put(const T &item);

            /** Put an item in the queue.
             *
             * If the queue size is greater or equal to max_size, then the method blocks a couple of milli seconds in order
             * to let the queue empty a bit.
             *
             * @param item item to store in the queue
             * @param wait_time millis to wait for the queue to make some space for the new item.
             * @throw queue_full an exception is thrown when the waiting time has expired and no item was found.
             */
            void put(const T &item, int wait_time);

            /** Get an item from the queue.
             *
             * If the queue is empty,  the method blocks until an item put in the queue.
             *
             * @param item item that will receive an item found onto the queue.
             * @see put
             */
            void get(T &item);

            /** Get an item from the queue, if the queue is empty, then wait for an element wait_time milliseconds.
             *
             * @param item item that will receive an item found onto the queue.
             * @param wait_time duration we are willing to wait for a new item.
             *
             * @throw queue_timeout
             */
            void get(T &item, int wait_time);

            /** @return true if queue is empty */
            bool empty() const {
                return _items.empty();
            }

            /** @return current number of elements in the queue */
#if __cplusplus < 201103L
            size_t size() {
              // we use read/write locks when std::atomic is not available
              pthread::lock_guard<pthread::read_lock> lck(_rwlock);
#else

            size_t size() const {
#endif
                return _items.size();
            }

            /** @return maximun number of items that can be put in the queue */
#if __cplusplus < 201103L
            size_t max_size() {
              // we use read/write locks when std::atomic is not available
              pthread::lock_guard<pthread::read_lock> lck(_rwlock);
#else

            size_t max_size() const {
#endif
                return _max_size;
            }

            /** change the maximum number of elements that can be put in the queue.
             *
             * > *WARN* the queue size must greater then zero (0).
             *
             * @param max_size maximum size (greater then zero)
             */
            void set_max_size(size_t max_size) {
                if (max_size >= 0) {
#if __cplusplus < 201103L
                    // we use read/write locks when std::atomic is not available
                    pthread::lock_guard<pthread::write_lock> lck(_rwlock);
#endif
                    _max_size = max_size;
                } else {
#if __cplusplus < 201103L
                    throw queue_exception("synchronized_queue's max size must be greater then 0.");
#else
                    throw queue_exception("synchronized_queue's max size must be greater then 0, max_size " + std::to_string(max_size) +
                                          " is not.");
#endif
                }
            }

            /** setup a sync_queue instance.
             *
             * > *WARN* max size must be greater then 0.
             *
             * @param max_size max queue size (default is 10).
             * @see std::list
             */
            explicit sync_queue(int max_size = 10);

            /** destructor */
            virtual ~sync_queue();

        private:

            pthread::mutex _mutex;
            pthread::condition_variable _not_empty_cv;
            pthread::condition_variable _not_full_cv;
            std::list<T> _items;
#if __cplusplus < 201103L
            pthread::read_write_lock    _rwlock;
            int                         _max_size ;
#else
            std::atomic<int> _max_size;
#endif

        };


        /** @} */

        // template implementation ------------------------------------------------

        template<typename T>
        void sync_queue<T>::get(T &item) {
            pthread::lock_guard<pthread::mutex> lck(_mutex);

#if __cplusplus < 201103L
            while ( (! (not_empty = !_items.empty())) && not_empty_cv.wait(_mutex) ){
            }
#else
            _not_empty_cv.wait(lck, [this] { return !_items.empty(); });
#endif

            item = _items.front();
            _items.pop_front();
            _not_full_cv.notify_one();
        }

        template<typename T>
        void sync_queue<T>::get(T &item, int wait_time) {

            pthread::lock_guard<pthread::mutex> lck(_mutex);

#if __cplusplus < 201103L
            bool not_empty = true;
            auto delay = wait_time;
            while ( ! (not_empty = !_items.empty()) && (_not_empty_cv.wait_for(_mutex, delay) == pthread::cv_status::no_timeout)){
              delay = -1 ;
            }
#else
            bool not_empty = _not_empty_cv.wait_for(lck, wait_time,
                                                    [this] { return !_items.empty(); }); // keep waiting if item list is full
#endif

            if (not_empty) {
                item = _items.front();
                _items.pop_front();
                _not_full_cv.notify_one();
            } else {
                _not_full_cv.notify_all();
                throw queue_timeout("synchronized_queue::get() timed out.");
            }
        }

        template<typename T>
        void sync_queue<T>::put(const T &item) {
            pthread::lock_guard<pthread::mutex> lck(_mutex);

#if __cplusplus < 201103L
            bool not_full = true;
            while ( ! (not_full = (_items.size() < _max_size))){
              _not_full_cv.wait(_mutex);
            }
#else
            _not_full_cv.wait(_mutex, [this] { return _items.size() < _max_size; });
#endif

            _items.push_back(item);
            _not_empty_cv.notify_one(); // signal that there is at least a new message
        }

        template<typename T>
        void sync_queue<T>::put(const T &item, int wait_time) {

            pthread::lock_guard<pthread::mutex> lck(_mutex);

#if __cplusplus < 201103L
            bool not_full = true;
            auto delay = wait_time;
            while ( ! (not_full = (_items.size() < _max_size)) && (_not_full_cv.wait_for(_mutex, delay) == pthread::cv_status::no_timeout)){
              delay = -1 ;
            }
#else
            // The following method signature uses lambda which is not supported by AIX XL C/C++ 13.1.2
            bool not_full = _not_full_cv.wait_for(lck, wait_time, [this] { return _items.size() < _max_size; });
#endif

            if (not_full) {
                _items.push_back(item);
                _not_empty_cv.notify_one();
            } else {
                _not_empty_cv.notify_all();
                throw queue_full("synchronized_queue::put() timeout, queue is full.");
            }
        }


        template<typename T>
        sync_queue<T>::sync_queue(int max_size): _max_size(max_size) {
        }

        template<typename T>
        sync_queue<T>::~sync_queue() {
            // Intentionally unimplemented...
        }

    }; // namespace util
};   // namespace pthread


#endif /* sync_queue_hpp */
