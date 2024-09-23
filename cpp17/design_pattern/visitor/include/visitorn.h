#pragma once
#include <iostream>
#include "visitor.h"
#include "objectn.h"

class Visitor1 : public Visitor {
 public:
  void visit(Obj1* obj) override {
    std::cout << __FILE__ << " v1o1 "  << __LINE__ << std::endl;
    obj->print();
  }
  void visit(Obj2* obj) override {
    std::cout << __FILE__ << " v1o2 "  << __LINE__ << std::endl;
    obj->print();
  }
};

class Visitor2 : public Visitor {
 public:
  void visit(Obj1* obj) override {
    std::cout << __FILE__ << " v2o1 "  << __LINE__ << std::endl;
    obj->print();
  }
  void visit(Obj2* obj) override {
    std::cout << __FILE__ << " v2o2 "  << __LINE__ << std::endl;
    obj->print();
  }
};