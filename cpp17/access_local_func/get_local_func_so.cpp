#include <dlfcn.h>
#include <elf.h>
#include <link.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

Elf64_Addr find_local_symbol_address(const std::string& filename,
                                     const std::string& symbol_name) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return 0;
  }

  // Read ELF header
  Elf64_Ehdr header;
  file.read(reinterpret_cast<char*>(&header), sizeof(header));
  if (memcmp(header.e_ident, ELFMAG, SELFMAG) != 0) {
    std::cerr << "Not an ELF file." << std::endl;
    file.close();
    return 0;
  }

  if (header.e_ident[EI_CLASS] != ELFCLASS64) {
    std::cerr << "Not a 64-bit ELF file." << std::endl;
    file.close();
    return 0;
  }

  // Find symbol table section header
  Elf64_Shdr symtab_hdr;
  bool found_symtab = false;
  for (int i = 0; i < header.e_shnum; ++i) {
    file.seekg(header.e_shoff + i * header.e_shentsize);
    Elf64_Shdr section;
    file.read(reinterpret_cast<char*>(&section), sizeof(section));
    if (section.sh_type == SHT_SYMTAB) {
      symtab_hdr = section;
      found_symtab = true;
    //   break;
    }
  }

  if (!found_symtab) {
    std::cerr << "Symbol table (SHT_SYMTAB) not found." << std::endl;
    file.close();
    return 0;
  }

  // Read string table section header
  Elf64_Shdr strtab_hdr;
  file.seekg(header.e_shoff + symtab_hdr.sh_link * header.e_shentsize);
  file.read(reinterpret_cast<char*>(&strtab_hdr), sizeof(strtab_hdr));

  std::vector<char> strtab;
  strtab.resize(strtab_hdr.sh_size);
  file.seekg(strtab_hdr.sh_offset);
  file.read(strtab.data(), strtab.size());

  // Read symbol table
  file.seekg(symtab_hdr.sh_offset);
  std::vector<Elf64_Sym> symbols(symtab_hdr.sh_size / symtab_hdr.sh_entsize);
  file.read(reinterpret_cast<char*>(symbols.data()),
            symbols.size() * sizeof(Elf64_Sym));

  // Find symbol with local binding and matching name
  Elf64_Addr symbol_address = 0;
  for (const auto& sym : symbols) {
    if (sym.st_name != 0 && sym.st_info != 0 &&
        ELF64_ST_BIND(sym.st_info) == STB_LOCAL &&
        ELF64_ST_TYPE(sym.st_info) == STT_FUNC &&
        strcmp(strtab.data() + sym.st_name, symbol_name.c_str()) == 0) {
      symbol_address = sym.st_value;
      break;
    }
  }

  file.close();
  return symbol_address;
}
