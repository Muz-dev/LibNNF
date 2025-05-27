#pragma once
#include <Tools/Tools.h>

namespace Compression {

class Yaz0 {
public:
    Yaz0(const char* path);
    void Decompress(const char* output);
    void Compress(const char* input);
private:
    const char* mPath;
    void Decode(const u8* src, u8* dst, const int uncompressedSize);
};

} // namespace Compression