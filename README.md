# what it does

IBM's compiler is not implement all the features of C++11 standard, especially it's laking the concurrency features that the standard brings.

This wrapper intends to bring these feature by wrapping the pthread library.  Of course, as it is a replacement of C++11 features, it is best to use the standard implementation if your compiler support it.

To use this library:
```
configure
make
make install
```

Hello world
# how it's used

## content

To use the library include `#include "pthread/pthread.hpp" to use the actual set of wrappers:
- `pthread::mutex`: handles a pthread mutex structure
- `pthread::lock_guard`: this class is in charge of locking and unlocking a given mutex.
- `pthread::condition_variable`: handles a pthread condition variable.
- `pthread::thread`: wrap's into a class a pthread thread.
- `pthread::pthread_exception`: base class off pthread exceptions

## using pthread::mutex

This code scrap illustrates how to use `mutex`objects:
```
pthread::mutex mtx;

// beginning of a critical section, get mutex
{
  pthread::lock_guard lck(mtx); // lock mutex

  // do something critical
  ...

} // lock_guard is freed and the mutex is unlock

```

## using pthread::condition_variable

To wait for special conditions to be met you use a condition variable. A condition variable blocks the current thread and waits to be signaled by another thread when the wanted condition was met. Condition variables are usefull to implement producer/consumer pettern. 

This sample synchronized queue uses mutexes and condition_variable objects. 

```
template<typename T> class sync_queue {
public:

  /** Put an item in the queue.
  *
  * If the queue max size is reached, then the operation waits until a size is smaller then max size again.
  */
  void put (const T& item);

  /** Put an item in the queue (wait if size >= max_size).
  *
  */
  void put (const T& item, int wait_time);

  /** Get an item from the queue.
  *
  * If the queue is empty, get waits for an item to be put.
  *
  * @param item item that will receive an item found onto the queue.
  */
  void get ( T& item);

  /** Get an item from the queue, if empty wait for one during duration milliseconds.
  *
  * @param item item that will receive an item found onto the queue.
  * @param duration duration we are willing to wait for a new item.
  *
  * @throw sync_queue_error when an error occurs.
  */
  void get ( T& item, int wait_time );

  size_t size() const noexcept;
  bool empty() const noexcept;

  int      max_size();
  void set_max_size( int ms );

  /** constructor
  *
  * ms: max queue size (default is 10).
  */
  sync_queue( int ms = 10 );

  /** destructor */
  virtual ~sync_queue();

private:

  pmu::logger::logger     log ;

  pthread::mutex              mtx;
  pthread::condition_variable _not_full_cv;
  pthread::condition_variable _not_empty_cv;

  std::list<T>            items ;

  atomic<int>             _max_size ;

};

// template implementation ------------------------------------------------

template<typename T> void sync_queue<T>::get( T& item ){
pthread::lock_guard lck(mtx);

if (_not_empty_cv.wait(lck,[this]{ return !items.empty(); })){
item=items.front();
items.pop_front();

_not_full_cv.notify_one();
}
}

template<typename T> void sync_queue<T>::get( T& item, int wait_time ){
pthread::lock_guard lck(mtx);

if ( _not_empty_cv.wait_for(lck,wait_time, [this]{ return !items.empty(); })){
item=items.front();
items.pop_front();
_not_full_cv.notify_one(); // signal queue is not full
} else {
throw sync_queue_timeout("queue is empty, sync_queue::get timedout.");
}
}

template<typename T> void sync_queue<T>::put( const T& item ) {
pthread::lock_guard lck(mtx);

if (_not_full_cv.wait(lck,[this]{ return !items.empty(); })){
items.push_back(item);
_not_empty_cv.notify_one(); // signal queue is not empty
}
}

template<typename T> void sync_queue<T>::put( const T& item, int wait_time ){
pthread::lock_guard lck(mtx);

if (_not_full_cv.wait_for(lck, wait_time, [this]{ return items.size() < _max_size; })){
items.push_back(item);
_not_empty_cv.notify_one();
} else {
_not_full_cv.notify_all();
throw sync_queue_full{"queue is full, sync_queue::put() timeout."};
}
}

template<typename T> int sync_queue<T>::max_size(){
return _max_size ;
}

template<typename T> void sync_queue<T>::set_max_size( int ms ){

log->trace("set_max_size({})", ms);

if (ms > 0){
_max_size = ms;
log->debug("max_size is set to {}", _max_size);
}else{
throw sync_queue_error("sync_queue's max size must be greater then 0, max_size " + std::to_string(ms) + " is not.");
}
}

template<typename T> bool sync_queue<T>::empty() const noexcept{
return items.empty();
}

template<typename T> size_t sync_queue<T>::size() const noexcept {
return items.size();
}

template<typename T> sync_queue<T>::sync_queue( int ms ): log{"sync_queue"}  {
this->set_max_size( ms );
}

template<typename T> sync_queue<T>::~sync_queue(){
}
};
```

## using pthread::thread

to use pthread threads it is need to extend the class `pthread::thread` like in this sample code:
```
class a_thread: public pthread::thread {
public:
  void run(){
    try {
   
    } catch ... ){
      // handle error
    }
  }
};

void main(){

  a_thread t1;
  a_thread t2;
  
  t1.start();
  t2.start();
  
  t1.join();
  t2.join();
}
```
