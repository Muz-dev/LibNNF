#pragma once
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

using u8 = uint8_t;
using u32 = uint32_t;

inline u32 swap_u32(const u32 val) {
    return (val & 0xFF000000) >> 24 |
    (val & 0x00FF0000) >> 8  |
    (val & 0x0000FF00) << 8  |
    (val & 0x000000FF) << 24;
}

inline u32 charArrToInt(const char* arr) {
    u32 result = 0;

    memcpy(&result, arr, sizeof(u32));

    return result;
}

inline u32 genHash(const char* str, const int length, const u32 key) {
    u32 result = 0;
    for (int i = 0; i < length; i++) {
        result = str[i] + result * key;
    }
    return result;
}

inline const char* matchHashWithName(const u32 hash, const std::vector<std::string>& names, const u32 key) {
    for (int i = 0; i < names.size(); i++) {
        if (hash == genHash(names[i].c_str(), strlen(names[i].c_str()), key)) {
            return names[i].c_str();
        }
    }
    return nullptr;
}

inline const char* appendWithChar(const char* str1, const char* str2, const char str3) {
    const size_t len1 = strlen(str1);
    const size_t len2 = strlen(str2);
    const size_t total_Len = len1 + 1 + len2 + 1;

    char* result = new char[total_Len];

    strcpy(result, str1);

    result[len1] = str3;

    strcpy(result + len1 + 1, str2);

    return result;
}
