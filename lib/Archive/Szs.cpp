#include "Archive/Szs.h"

#include <cstring>
#include <fstream>
#include <filesystem>
#include <Compression/Yaz0.h>

#include "Sarc.h"

namespace Archive {

void Szs::Extract(const char* output) {
    Compression::Yaz0* yaz0 = new Compression::Yaz0(mPath);

    const auto sarc_output = new char;

    strcat(sarc_output, ".sarc");

    yaz0->Decompress(sarc_output);

    const auto sarc = new Sarc(sarc_output);

    sarc->Extract(output);

    std::filesystem::remove(sarc_output);

}


} // namespace Archive

