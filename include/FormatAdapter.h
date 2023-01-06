/**
 * @file FormatAdapter.h
 * @author 0x574859 (Hex574859@outlook.com)
 * @brief `<format>` adapter.
 * @version 0.1
 * @date 2023-01-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <version>
#include <string>


// <format> support tester
#ifdef __cpp_lib_format

#include <format>

namespace Config {

template<typename... T>
auto Format(const std::wstring_view fmt,const T&... t) {
   return std::vformat(fmt,std::make_wformat_args(t...));
}

} // Config

#else

#include <fmt/xchar.h>

namespace Config {

template<typename... T>
auto Format(const fmt::wstring_view fmt, const T&... t) {
   return fmt::vformat(fmt, fmt::make_wformat_args(t...));
}

} // Config

#endif

