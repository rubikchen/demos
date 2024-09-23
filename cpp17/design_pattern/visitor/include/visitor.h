#pragma once
class Obj1;
class Obj2;
class Visitor {
public:
    virtual void visit(Obj1* obj) = 0;
    virtual void visit(Obj2* obj) = 0;
};