#include <pthread.h>
#include <pthread/pthread.hpp>

#include <cstdio>
#include <iostream>
#include <string>
#include <atomic>
#include <memory>


class message {
  public:
    message (const std::string &buffer):_message(buffer){
      printf ("message allocated: %s\n", _message.c_str());
    };

    virtual ~message(){
      printf ("message de-allocated\n");
    };

    const std::string content() const {
      return _message.c_str();
    };

    void set_content(const std::string &message){
      _message = message ;
    };

  private:
    std::string _message;
};

typedef std::shared_ptr<message> message_ptr;
typedef pthread::util::synchronized_queue<message_ptr> synchronized_message_queue;


class status {
  public:

    status(synchronized_message_queue &queue): _queue(queue){ // : _running(true){
      printf ("status allocated\n");
    };

    virtual ~status(){
      printf ("status de-allocated\n");
    };

    bool running() const { return _running; };
    void stop() {
      pthread::lock_guard<pthread::mutex> lock(_mutex);
      _running = false ;
      printf("class status changed running to %d (%s, %d)\n", _running, __FILE__, __LINE__);
    };

  protected:
    synchronized_message_queue &_queue;

  private:
    static bool           _running;
    static pthread::mutex _mutex;
};

class producer : public status, public pthread::abstract_thread {
  public:

    producer(synchronized_message_queue &queue): status(queue){
    };

    void run() noexcept {
      printf ("starting producer\n");
      while( running() ){
        message_ptr pmessage(new message("producer creation..."));
      }
      printf("stopping producer\n");
    };
};

class consumer : public status, public pthread::abstract_thread {
  public:

    consumer(synchronized_message_queue &queue): status(queue){
    };

    void run() noexcept {
      printf ("starting consumer\n");
      message_ptr pmessage ; // (new message("hello"));
      while( running() ){

        _queue.get(pmessage);
        printf("consumer received message: %s\n", pmessage->content().c_str()); 

        pmessage->set_content("consumer changed message content");
        printf("consumer modification: %s\n", pmessage->content().c_str()); 
      }
      printf("stopping consumer\n");
    };
};

bool status::_running = true;
pthread::mutex status::_mutex ;

int main(int argc, const char * argv[]) {
  
  std::cout << "version: " << pthread::cpp_pthread_version() << std::endl;

  try {

    synchronized_message_queue queue;
    status status(queue);

    pthread::thread_group group;

    group.add(new consumer(queue));
    // group.add(new producer(queue));
    group.start();

    // std::string buffer;
    // printf("type <enter> to end program:");
    // std::getline (std::cin,buffer);

    // it's going too fast, so sleep a bit
    pthread::this_thread::sleep_for(1 * 1000);

    message_ptr main_message(new message("Salut les amis du threading..."));
    queue.put(main_message);

    status.stop();

    group.join();
    printf("threads joined main programm (%s, %d)\n", __FILE__, __LINE__);

  }catch (std::exception &err ){
    std::cerr << __FILE__ << "(at:" << __LINE__ << ")" << err.what() << std::endl;
  }
}
