/**
 * @file Error.h
 * @author 0x574859 (Hex574859@outlook.com)
 * @brief Error processor
 * @version 0.1
 * @date 2023-01-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <string>               // `std::wstring`

/**
 * @brief Partial definition of `Config`
 *
 */
namespace Config {

/**
 * @brief For reporting error using throw
 *
 * @param msg error message
 */
inline void Error(const std::wstring& msg) {
    throw msg;
}

} // Config
