/**
 * @file FormatAdapter.h
 * @author 0x574859 (Hex574859@outlook.com)
 * @brief `<format>` adapter
 * @version 0.1
 * @date 2023-01-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <version>               // `__cpp_lib_format`

/**
 * @brief `<format>` support tester
 *
 */
#ifdef __cpp_lib_format

#include <string>                // `std::wstring_view`
#include <format>                // `std::vformat`, `std::make_wformat_args`

/**
 * @brief Partial definition of `Config`
 *
 */
namespace Config {

/**
 * @brief Wrapper for `vformat`
 *
 * @tparam T: arguments pack
 * @param fmt: view of format string
 * @param t: arguments
 * @return auto: `std::string`
 */
template<typename... T>
auto Format(const std::wstring_view fmt,const T&... t) {
   return std::vformat(fmt,std::make_wformat_args(t...));
}

} // Config

#else

#include <fmt/xchar.h>     // `fmt::vformat, make_wformat_args, wstring_view`

/**
 * @brief Partial definition of `Config`
 *
 */
namespace Config {

/**
 * @brief Wrapper for `vformat`
 *
 * @tparam T: arguments pack
 * @param fmt: view of format string
 * @param t: arguments
 * @return auto: `std::string`
 */
template<typename... T>
auto Format(const fmt::wstring_view fmt, const T&... t) {
   return fmt::vformat(fmt, fmt::make_wformat_args(t...));
}

} // Config

#endif

