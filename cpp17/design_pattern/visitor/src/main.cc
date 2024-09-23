#include "objectn.h"
#include "visitorn.h"
int main() {
    Visitor1 v1;
    Visitor2 v2;


    Obj1 o1;
    Obj2 o2;
    o1.accept(&v1);
    o1.accept(&v2);
    o2.accept(&v1);
    o2.accept(&v2);
}