#pragma once
class Visitor;
class Object {
public:
    virtual void accept(Visitor* vist) = 0;
    virtual void print() = 0;
};