#include "Compression/Yaz0.h"

#include <cstring>
#include <fstream>
#include <iosfwd>

#include "Tools/Tools.h"

namespace Compression {

Yaz0::Yaz0(const char *path) {
    mPath = path;
}

void Yaz0::Decompress(const char *output) {

    std::ifstream inFile(mPath, std::ios::binary);
    std::ofstream outFile(output, std::ios::out | std::ios::binary);

    if (!inFile.is_open()) {
        printf("Error opening input file\n");
        return;
    }
    inFile.seekg(0, std::ios::end);
    const std::streampos fileSize = inFile.tellg();
    const size_t compressedDataSize = (size_t)fileSize - 16;
    char Magic[4];
    inFile.seekg(0, std::ios::beg);
    inFile.read(Magic, 4);



    if (strcmp(Magic, "Yaz0")) {
        printf("Invalid magic number. Expected Yaz0, got %s", Magic);
        return;
    }

    u32 uncompressedSize = 0;
    inFile.seekg(4, std::ios::beg);
    inFile.read((char*)&uncompressedSize, 4);

    uncompressedSize = swap_u32(uncompressedSize);

    inFile.seekg(16, std::ios::beg);

    u8* dst = new u8[uncompressedSize];
    u8* src = new u8[compressedDataSize];
    inFile.read((char*)src, compressedDataSize);

    Decode(src, dst, uncompressedSize);

    outFile.write((char*)dst, uncompressedSize);

    delete[] dst;
    delete[] src;
}

void Yaz0::Decode(const u8* src, u8* dst, const int uncompressedSize) {
    int srcPlace = 0, dstPlace = 0; //current read/write positions

    u32 validBitCount = 0; //number of valid bits left in "code" byte
    u8 currCodeByte = 0;
    while(dstPlace < uncompressedSize)
    {
        //read new "code" byte if the current one is used up
        if(validBitCount == 0)
        {
            currCodeByte = src[srcPlace];
            ++srcPlace;
            validBitCount = 8;
        }

        if((currCodeByte & 0x80) != 0)
        {
            //straight copy
            dst[dstPlace] = src[srcPlace];
            dstPlace++;
            srcPlace++;
        }
        else
        {
            //RLE part
            u8 byte1 = src[srcPlace];
            u8 byte2 = src[srcPlace + 1];
            srcPlace += 2;

            u32 dist = ((byte1 & 0xF) << 8) | byte2;
            u32 copySource = dstPlace - (dist + 1);

            u32 numBytes = byte1 >> 4;
            if(numBytes == 0)
            {
                numBytes = src[srcPlace] + 0x12;
                srcPlace++;
            }
            else
                numBytes += 2;

            //copy run
            for(int i = 0; i < numBytes; ++i)
            {
                dst[dstPlace] = dst[copySource];
                copySource++;
                dstPlace++;
            }
        }

        // use the next bit from "code" byte
        currCodeByte <<= 1;
        validBitCount-=1;
    }
} // Thanks to http://www.amnoid.de/gc/yaz0.txt for this code

} // namespace Compression