// SPDX-License-Identifier: GPL-2.0-or-later

// LibISDB uses std::basic_string<uint8_t>.  libc++ (LLVM 19 or later) no longer provides the
// generic std::char_traits template for types other than character types.  This header provides
// std::char_traits<unsigned char> only when libc++ is used.  libstdc++ still provides it.

#pragma once

#include <cstddef>
#include <cstring>
#include <cwchar>
#include <ios>

#if defined(_LIBCPP_VERSION)

namespace std {

template <>
struct char_traits<unsigned char> {
  using char_type = unsigned char;
  using int_type = unsigned int;
  using off_type = streamoff;
  using pos_type = streampos;
  using state_type = mbstate_t;

  static constexpr void assign(char_type& c1, const char_type& c2) noexcept { c1 = c2; }
  static constexpr bool eq(char_type c1, char_type c2) noexcept { return c1 == c2; }
  static constexpr bool lt(char_type c1, char_type c2) noexcept { return c1 < c2; }

  static constexpr int compare(const char_type* s1, const char_type* s2, size_t n) {
    for (; n; --n, ++s1, ++s2) {
      if (lt(*s1, *s2)) return -1;
      if (lt(*s2, *s1)) return 1;
    }
    return 0;
  }
  static constexpr size_t length(const char_type* s) {
    size_t len = 0;
    for (; !eq(*s, char_type(0)); ++s) ++len;
    return len;
  }
  static constexpr const char_type* find(const char_type* s, size_t n, const char_type& a) {
    for (; n; --n, ++s) {
      if (eq(*s, a)) return s;
    }
    return nullptr;
  }
  static char_type* move(char_type* s1, const char_type* s2, size_t n) {
    if (n) std::memmove(s1, s2, n);
    return s1;
  }
  static char_type* copy(char_type* s1, const char_type* s2, size_t n) {
    if (n) std::memcpy(s1, s2, n);
    return s1;
  }
  static char_type* assign(char_type* s, size_t n, char_type a) {
    if (n) std::memset(s, a, n);
    return s;
  }

  static constexpr int_type not_eof(int_type c) noexcept { return eq_int_type(c, eof()) ? ~eof() : c; }
  static constexpr char_type to_char_type(int_type c) noexcept { return char_type(c); }
  static constexpr int_type to_int_type(char_type c) noexcept { return int_type(c); }
  static constexpr bool eq_int_type(int_type c1, int_type c2) noexcept { return c1 == c2; }
  static constexpr int_type eof() noexcept { return int_type(EOF); }
};

}  // namespace std

#endif  // defined(_LIBCPP_VERSION)
