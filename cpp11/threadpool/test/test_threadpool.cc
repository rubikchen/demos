#include <iostream>

#include "threadpool.h"
#include "threadpool_stack.h"

void good(int a) { std::cout << __LINE__ << " " << a << std::endl; }

class Test {
 public:
  void show() { std::cout << __func__ << std::endl; }
};

template <typename T>
void test(T&& ts) {
  ts.enqueue(good, 2);
  Test t;
  ts.enqueue(&Test::show, &t);

  std::cout << ts.enqueue([](int answer) { return answer; }, 42).get()
            << std::endl;
}

int main() { 
  test(cpp11::threadpool(2)); 
  test(cpp11::threadpool_stack(2)); 
}