/**
 * Copyright 2024 @rubikchen
 *
 */

#ifndef CPP17_BACKTRACE_INCLUDE_BACKTRACE_H_
#define CPP17_BACKTRACE_INCLUDE_BACKTRACE_H_
#include <vector>

class BackTrace {
 public:
  static std::vector<void*> GetBackTrace();
};
#endif  // CPP17_BACKTRACE_INCLUDE_BACKTRACE_H_
