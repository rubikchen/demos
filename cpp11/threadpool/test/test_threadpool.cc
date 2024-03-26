#include <iostream>

#include "threadpool.h"

void good(int a) {
  std::cout << __LINE__ << " " << a << std::endl;
}

class Test{
public:
  void show() {
    std::cout << __func__ << std::endl;
  }
};

int main() {
  cpp11::threadpool ts(2);
	ts.enqueue(good, 2);
  Test t;
  ts.enqueue(&Test::show, &t);

  std::cout << ts.enqueue([](int answer) { return answer; }, 42).get() << std::endl;
}