#ifndef PRINTER_H_
#define PRINTER_H_


#include <iostream>
#include <string_view>


namespace cryptopals {

  inline std::ostream& print_bold(std::ostream& os, std::string_view str) {
    return os << "\033[1m" << str << "\033[0m";
  }

  inline std::ostream& print_failure(std::ostream& os, std::string_view str) {
    return os << "\033[1;31m" << str << "\033[0m";
  }

  inline std::ostream& print_success(std::ostream& os) {
    return os << "\033[1;32mSUCCESS\033[0m";
  }

} // namespace cryptopals


#endif // PRINTER_H_
