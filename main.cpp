#include <Archive/Sarc.h>

#include "lib/Compression/Yaz0.h"

int main() {
    Compression::Yaz0* myYaz = new Compression::Yaz0("../Yoshi.szs");
    Archive::Sarc* mySarc = new Archive::Sarc("../output.sarc");

    mySarc->Extract("../output");

}