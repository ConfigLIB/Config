/**
 * @file Error.h
 * @author 0x574859 (Hex574859@outlook.com)
 * @brief Error processor.
 * @version 0.1
 * @date 2023-01-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <string>

namespace Config {

inline void Error(const std::wstring& msg) {
    throw msg;
}

} // Config
