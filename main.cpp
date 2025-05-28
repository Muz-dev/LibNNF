#include <stdio.h>
#include <Archive/Sarc.h>
#include <Compression/Yaz0.h>


int main() {

    const auto sarc = new Archive::Sarc("../Output");

    sarc->Extract("../output");

}
