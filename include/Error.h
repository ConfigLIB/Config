#pragma once

#include <string>

namespace Config {

inline void Error(const std::wstring& msg) {
    throw msg;
}

}
