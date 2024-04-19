/**
 * Copyright 2024 @rubikchen
 *
 */

#include "./backtrace.h"

#include <dlfcn.h>
#include <unwind.h>

std::vector<void*> BackTrace::GetBackTrace() {
  std::vector<void*> trace;
  _Unwind_Backtrace(
      [](struct _Unwind_Context* context, void* trace_ptr) {
        auto& trace = *reinterpret_cast<std::vector<void*>*>(trace_ptr);
        auto addr = reinterpret_cast<void*>(_Unwind_GetIP(context));
        Dl_info info;
        dladdr(addr, &info);
        auto curr_ptr = reinterpret_cast<char*>(addr);
        auto base_ptr = reinterpret_cast<char*>(info.dli_fbase);
        trace.emplace_back(reinterpret_cast<void*>(curr_ptr - base_ptr - 1));
        return _URC_NO_REASON;
      },
      &trace);
  return trace;
}
