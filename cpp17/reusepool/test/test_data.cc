#include "datapool.h"
#include <memory>
#include <iostream>
int main() {
  cpp17::data_pool<int> a;
  a.enqueue(std::make_unique<int>(1));
  for(int i = 0; i < 4 ;i++) {
    auto ret = a.get();
    std::cout << ret.get() << std::endl;
  }
}