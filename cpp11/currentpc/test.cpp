void get_current_info() {
  void* pc;
  asm volatile("lea (%%rip), %0" : "=r"(pc));
}