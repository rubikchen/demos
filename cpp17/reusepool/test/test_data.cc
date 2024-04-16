#include "datapool.h"
#include <memory>
#include <iostream>
int main() {
  cpp17::data_pool<int> a;
  a.enqueue(std::make_unique<int>(1));
  a.make_item(2);
  a.make_item(3);
  for(int i = 0; i < 20 ;i++) {
    auto ret = a.get();
    std::cout << *ret.get() << " " << ret.get() << std::endl;
  }
}