#include <signal.h>

#include<iostream>
#include<string>


#include "./backtrace.h"

void show(int) {
    auto trace = BackTrace::GetBackTrace();
    for (const auto& item: trace) {
        std::cout << item << std::endl;
    }
}

int main() {
    signal(SIGABRT, show);

    throw 1;
}
