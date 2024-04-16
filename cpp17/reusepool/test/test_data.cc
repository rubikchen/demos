#include "datapool.h"
#include <memory>
#include <iostream>
class T{
public:
  T(int i):i_(i){}
  ~T(){
    std::cout << "release " << *this << " " << this << std::endl;
  }
  friend std::ostream& operator << (std::ostream& os, const T& t) {
    os << t.i_;
    return os;
  }
private:
  int i_;
};
int main() {
  cpp17::data_pool<T> a;
  a.make_item(1);
  a.make_item(2);
  a.make_item(3);
  for(int i = 0; i < 20 ;i++) {
    auto ret = a.get();
    std::cout << *ret.get() << " " << ret.get() << std::endl;
  }
  {
    auto temp = a.get();
    a.clear();
    a.make_item(4);
    a.make_item(5);
    for(int i = 0; i < 20 ;i++) {
      auto ret = a.get();
      std::cout << *ret.get() << " " << ret.get() << std::endl;
    }
  }
}