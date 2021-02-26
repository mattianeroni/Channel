// *******************************************************************
// ----------------------------- TEST --------------------------------
// *******************************************************************


#include <iostream>
#include <thread>
#include "channel.hh"

using namespace std;



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
