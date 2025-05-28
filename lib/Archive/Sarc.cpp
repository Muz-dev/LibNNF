#include "Archive/Sarc.h"

#include <cstring>
#include <fstream>
#include <iosfwd>
#include <stdio.h>
#include <filesystem>
#include <vector>
#include <Tools/Tools.h>

namespace Archive {

Sarc::Sarc(const char *path) {
    mPath = path;
    mIsBig = false;
}

void Sarc::Extract(const char* output_dir) {
    if (!std::filesystem::exists(mPath)) {
        fprintf(stderr, "SARC: File does not exist: %s\n", mPath);
        return;
    }
    std::filesystem::create_directory(output_dir);
    std::filesystem::path path(output_dir);
    std::ifstream input(mPath, std::ios::binary | std::ios::ate);


    std::streamoff size = input.tellg();
    input.seekg(0, std::ios::beg);

    char magic[5] = {};

    input.read(magic, 4);


    if (strcmp(magic, "SARC")) {
        fprintf(stderr, "Invalid Magic, Expected SARC, got %s", magic);
        return;
    }
    input.seekg(4, std::ios::beg);
    char buffer[2];
    input.read((char*)&buffer, 2);

    const uint16_t header_len = buffer[0] | (buffer[1] << 8);

    if (header_len != 0x14) {
        fprintf(stderr, "Invalid header length, expected 0x14, got %hu", header_len);
        return;
    }

    input.seekg(6, std::ios::beg);
    char BOM[2];
    input.read((char*)&BOM, 2);

    if ((uint8_t)BOM[0] == 0xFE && (uint8_t)BOM[1] == 0xFF) {
        mIsBig = true;
    }
    else if ((uint8_t)BOM[0] == 0xFF && (uint8_t)BOM[1] == 0xFE) {
        mIsBig = false;
    }
    else {
        fprintf(stderr, "Invalid BOM, expected FEFF or FFFE, got %02X%02X\n",
                (uint8_t)BOM[0], (uint8_t)BOM[1]);
        return;
    }

    input.seekg(12, std::ios::beg);

    char Beginning_data[4];

    input.read((char*)&Beginning_data, 4);


    char Ver_num[2];
    input.read(Ver_num, 2);

    const uint16_t version = (uint8_t)Ver_num[0] | ((uint8_t)Ver_num[1] << 8);

    if (version != 0x0100) {
        fprintf(stderr, "Invalid version, expected 0x0100, got 0x%02X%02X\n",
                (uint8_t)Ver_num[1], (uint8_t)Ver_num[0]);
        return;
    }


    input.seekg(2, std::ios::cur);

    char SFAT_magic[5];

    input.read((char*)&SFAT_magic, 4);
    SFAT_magic[4] = '\0';

    if (strcmp(SFAT_magic, "SFAT")) {
        fprintf(stderr, "Invalid Magic, expected SFAT, got %s", SFAT_magic);
        return;
    }


    short SFAT_Len;
    input.read((char*)&SFAT_Len, 2);

    if (SFAT_Len != 0xC) {
        fprintf(stderr, "Invalid Length, expect C, got %hd", SFAT_Len);
        return;
    }

    //input.seekg(2, std::ios::cur);
    short NodeCount;

    input.read((char*)&NodeCount, 2);
    Node** nodes = new Node*[NodeCount];
    //input.seekg(2, std::ios::cur);

    int Hash_Key;
    input.read((char*)&Hash_Key, 4);

    if (Hash_Key != 0x00000065) {
        fprintf(stderr, "Invalid Hash Key, expect 0x65, got 0x%08x", Hash_Key);
        return;
    }

    for (int i = 0; i < NodeCount; i++) {
        nodes[i] = new Node;

        input.read((char*)&nodes[i]->mFilename_hash, 4);
        input.read((char*)&nodes[i]->mFile_attributes, 4);
        input.read((char*)&nodes[i]->mBeginning, 4);
        input.read((char*)&nodes[i]->mEnd, 4);
    }


    //input.seekg(4, std::ios::cur);

    int OffsetToString = (16 * NodeCount) + 8;

    //input.seekg(OffsetToString, std::ios::cur);
    input.seekg(8, std::ios::cur);



    //printf("Data: %s", data);

    std::vector<std::string> fileNames;

    while ((int)fileNames.size() < NodeCount) {
        std::string name;
        char ch;
        while (input.get(ch) && ch != '\0') {
            name += ch;
        }
        if (!name.empty()) {
            fileNames.push_back(name);
        }
    }



    for (int i = 0; i < NodeCount; i++) {
        printf("%s\n", fileNames[i].c_str());
    }

    input.seekg(charArrToInt(Beginning_data), std::ios::beg);

    for (int i = 0; i < NodeCount; i++) {
        std::ofstream output;
        const char* filename = matchHashWithName(nodes[i]->mFilename_hash, fileNames, Hash_Key);
        const char* dir = appendWithChar(output_dir, filename, '/');
        output.open(dir, std::ios::binary);

        u32 beginning = nodes[i]->mBeginning;
        u32 end = nodes[i]->mEnd;
        u32 size_of_file = end - beginning;

        int beginning_with_header = beginning + charArrToInt(Beginning_data);

        input.seekg(beginning_with_header, std::ios::beg);

        char buffer_size[size_of_file + 1];

        input.read(buffer_size, size_of_file);
        output.write(buffer_size, size_of_file);
    }


}



}  // namespace Archive
