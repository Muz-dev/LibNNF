#pragma once
#include <sys/types.h>
#include <Tools/Tools.h>

namespace Archive {



class Sarc {
public:
    Sarc(const char* path);
    void Extract(const char* output_dir);
    void Archive(const char* output);
private:
    const char* mPath;
    bool mIsBig;
    uint GetFilenameHash(const char* name, const int length) {
        uint result = 0;
        for (int i = 0; i < length; i++) {
            result = name[i] + result * 0x65;
        }
        return result;
    }
    struct Node {
        u32 mFilename_hash;
        int mFile_attributes;
        int mBeginning;
        int mEnd;
    };
};



} // namespace Archive