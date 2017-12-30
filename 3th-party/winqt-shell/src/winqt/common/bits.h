//
// deskhare - cross-platform quick launcher
// Copyright (C) 2017 Richard Liebscher
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#pragma once

#include <cstddef>

namespace WinQt {

// word

constexpr uint16_t low_word(uint32_t x) {
  return x;
}

constexpr uint32_t low_word(uint32_t x, uint16_t value) {
  return (x & 0xFFFF0000) | value;
}

constexpr uint16_t high_word(uint32_t x) {
  return x >> 16;
}

constexpr uint32_t high_word(uint32_t x, uint16_t value) {
  return (x & 0x0000FFFF) | (value << 16);
}

constexpr uint32_t dword(uint16_t high, uint16_t low) {
  return (static_cast<uint32_t>(high) << 16) | (low);
}

// byte

constexpr uint8_t low_byte(uint16_t x) {
  return x;
}

constexpr uint16_t low_byte(uint16_t x, uint8_t value) {
  return (x & 0xFF00) | value;
}

constexpr uint8_t high_byte(uint16_t x) {
  return x >> 8;
}

constexpr uint16_t high_byte(uint16_t x, uint8_t value) {
  return (x & 0x00FF) | (value << 8);
}

constexpr uint16_t word(uint8_t high, uint8_t low) {
  return (static_cast<uint16_t>(high) << 8) | (low);
}

} // namespace WinQt
