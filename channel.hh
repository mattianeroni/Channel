#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>

using namespace std;



template <class T>
struct chan {
  
  vector<T>          buffer;          // The buffer where storing the elements in the channel
  int                size = 0;        // The current size of the cahnnel

  mutex              mut;             // The mutual exclusion of threads
  condition_variable cv;              // The condition variable
  bool               ready = true;    // The state of the channel (i.e., if TRUE it is accessible)

  
  
  T get () {
    // Instantiate the locker
    unique_lock<mutex> lock(mut);
    // Wait for the buffer to be free
    cv.wait(lock, [this](){return ready;});
    // Operate on the buffer
    ready = false;
    T i = buffer[0];
    buffer.erase(buffer.begin());
    // Release the locker
    lock.unlock();
    // Notify one of the threads waiting
    cv.notify_one();
    ready = true;
    // Return the element retrieved
    return i;
  }

  
  
  void put (T element ){
    // Instantiate the locker
    unique_lock<mutex> lock(mut);
    // Wait for the buffer to be free
    cv.wait(lock, [this](){return ready;});
    // Operate on the buffer
    ready = false;
    buffer.push_back(element);
    // release the locker
    lock.unlock();
    cv.notify_one();
    ready = true;
  }

};
