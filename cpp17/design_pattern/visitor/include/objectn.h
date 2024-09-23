#pragma once
#include <iostream>
#include "object.h"
#include "visitor.h"

class Obj1 : public Object {
public:
    void accept(Visitor* vist) override {
        vist->visit(this);
    }
    void print() override {
        std::cout << __FILE__ << " "  << __LINE__ << std::endl;
    }
};

class Obj2 : public Object {
public:
    void accept(Visitor* vist) override {
        vist->visit(this);
    }
    void print() override {
        std::cout << __FILE__ << " "  << __LINE__ << std::endl;
    }
};