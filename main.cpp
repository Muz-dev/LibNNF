#include <stdio.h>
#include <Archive/Sarc.h>
#include <Archive/Szs.h>
#include <Compression/Yaz0.h>


int main() {
    Archive::Szs* output_szs = new Archive::Szs("../Yoshi.szs");

    output_szs->Extract("../Output_Yoshi");

}
