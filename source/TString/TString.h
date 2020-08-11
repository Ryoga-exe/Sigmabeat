#pragma once
#include "DxLib.h"
#include <string>

namespace std {
    using tstring = std::basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> >;

    template <typename T>
    bool replaceStr(basic_string<T>& replacedStr, basic_string<T> from, basic_string<T> to) {
        const unsigned int pos = replacedStr.find(from);
        const int len = from.length();
        if (pos == std::string::npos || from.empty()) return false;
        replacedStr.replace(pos, len, to);
        return true;
    }
}