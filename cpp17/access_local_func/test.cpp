#include <dlfcn.h>
#include <elf.h>
#include <fcntl.h>
#include <link.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

void invoke_show() {
  auto hd = dlopen("./xx.so", RTLD_NOW);
  //  get base addr
  struct link_map* lmap{};
  dlinfo(hd, RTLD_DL_LINKMAP, &lmap);
  auto lib_base_addr = reinterpret_cast<char*>(lmap->l_addr);
  // get elf header

  auto fd = open("./xx.so", O_RDONLY);
  struct stat file_stat {};
  if (fstat(fd, &file_stat) == -1) {
    perror("Error getting file size");
    close(fd);
  }

  auto s_addr = mmap(nullptr, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  auto base_addr = reinterpret_cast<char*>(s_addr);
  auto elf_header = reinterpret_cast<Elf64_Ehdr*>(base_addr);

  // find symbol table
  auto base_shdr_addr =
      reinterpret_cast<Elf64_Shdr*>(base_addr + elf_header->e_shoff);
  Elf64_Shdr symtab_hdr{};
  Elf64_Shdr strsymtab_hdr{};
  for (int i = 0; i < elf_header->e_shnum; ++i) {
    if (base_shdr_addr[i].sh_type == SHT_SYMTAB) {
      symtab_hdr = base_shdr_addr[i];
      strsymtab_hdr = base_shdr_addr[symtab_hdr.sh_link];
      break;
    }
  }

  std::string symbol_name("show");
  auto sym_table_array =
      reinterpret_cast<Elf64_Sym*>(base_addr + symtab_hdr.sh_offset);
  auto sym_table_array_len = symtab_hdr.sh_size / symtab_hdr.sh_entsize;
  auto sym_str_table_addr = base_addr + strsymtab_hdr.sh_offset;
  void* sym_func_addr = nullptr;
  for (int i = 0; i < sym_table_array_len; i++) {
    if (sym_table_array[i].st_name != 0 && sym_table_array[i].st_info != 0 &&
        ELF64_ST_BIND(sym_table_array[i].st_info) == STB_LOCAL &&
        ELF64_ST_TYPE(sym_table_array[i].st_info) == STT_FUNC &&
        strcmp(sym_str_table_addr + sym_table_array[i].st_name,
               symbol_name.c_str()) == 0) {
      sym_func_addr = lib_base_addr + sym_table_array[i].st_value;
      break;
    }
  }

  // invoke
  auto show2 = reinterpret_cast<void (*)()>(sym_func_addr);
  show2();
  dlclose(hd);
}

int main() {
  invoke_show();

  return 0;
}
