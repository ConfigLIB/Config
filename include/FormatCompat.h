#pragma once

#include <version>
#include <string>



#ifdef __cpp_lib_format

#include <format>

namespace Config {

template<typename... T>
auto Format(const std::wstring_view fmt,const T&... t) {
   return std::vformat(fmt,std::make_wformat_args(t...));
}

}

#else

#include <fmt/xchar.h>

namespace Config {

template<typename... T>
auto Format(const fmt::wstring_view fmt, const T&... t) {
   return fmt::vformat(fmt, fmt::make_wformat_args(t...));
}

}

#endif

