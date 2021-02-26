#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;

template <class T>
struct chan {

  vector<T> buffer;
  int size = 0;

  mutex mut;
  condition_variable cv;
  bool ready = true;


  T get () {
    unique_lock<mutex> lock(mut);

    cv.wait(lock, [this](){return ready;});

    ready = false;
    T i = buffer[0];
    buffer.erase(buffer.begin());

    lock.unlock();
    cv.notify_one();
    ready = true;
    return i;
  }

  void put (T element ){
    unique_lock<mutex> lock(mut);
    cv.wait(lock, [this](){return ready;});
    ready = false;
    buffer.push_back(element);
    lock.unlock();
    cv.notify_one();
    ready = true;
  }

};



void putter (chan<int>* c, int element){
  c->put(element);
}


void getter (chan<int>* c) {
  int a = c->get();
  cout << "Retrieved " << a << "\n";
}


int main () {

  chan<int> c;

  thread t1 (putter, &c, 10);
  thread t2 (putter, &c, 3);
  thread t3 (getter, &c);

  t1.join(); t2.join(); t3.join();

  cout << "Program correctly concluded.\n";
  return 0;
}
