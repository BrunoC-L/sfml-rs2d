#pragma once
#include "crc_table.h"
#include <iostream>
#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>

// https://rosettacode.org/wiki/CRC-32#C.2B.2B

std::uint_fast32_t string_crc32(const std::string& s) {
    auto first = s.begin();
    auto last = s.end();
    return std::uint_fast32_t{ 0xFFFFFFFFuL } &
        ~std::accumulate(first, last,
            ~std::uint_fast32_t{ 0 } &std::uint_fast32_t{ 0xFFFFFFFFuL },
            [](std::uint_fast32_t checksum, std::uint_fast8_t value)
            { return crc_table[(checksum ^ value) & 0xFFu] ^ (checksum >> 8); });
}
