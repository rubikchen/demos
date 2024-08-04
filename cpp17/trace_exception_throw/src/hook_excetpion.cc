/**
 * Copyright 2024 @rubikchen
 *
 */
/**
 * override abi func, load symbol from global or next
 */
#include <typeinfo>

#include <dlfcn.h>
#include <unwind.h>

#include <vector>
#include <iostream>

static std::vector<void*> GetBackTrace() {
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

void print_trace() {
    for (auto i : GetBackTrace()) {
        std::cout << std::hex << i << std::endl;
    }
}

#define symbol_next(sym) \
 *reinterpret_cast<decltype(sym)*>(dlsym(RTLD_NEXT, #sym))
extern "C" {
void __cxa_throw(void* a, std::type_info* b, void (*c)(void*)) {
    static auto ret = symbol_next(__cxa_throw);
    print_trace();
    ret(a, b, c);
}

void __cxa_rethrow() {
    static auto ret = symbol_next(__cxa_rethrow);
    print_trace();
    ret();
}
}