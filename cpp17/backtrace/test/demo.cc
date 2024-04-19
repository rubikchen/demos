#include <signal.h>

#include <future>
#include <iostream>
#include <string>

#include "./backtrace.h"

void show(int) {
  auto trace = BackTrace::GetBackTrace();
  for (const auto& item : trace) {
    std::cout << item << std::endl;
  }
}

int main() {
  signal(SIGABRT, show);

  // if not set noexcept function attr, you will not get an exception at the
  // place it throw, rather than future get was invoked.
  std::async(std::launch::async, []() noexcept { std::abort(); }).get();
}
