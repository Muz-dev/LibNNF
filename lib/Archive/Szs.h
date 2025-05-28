#pragma once

namespace Archive {

class Szs {
public:
    Szs(const char* path) : mPath(path) {}
    void Extract(const char* output);
    void Compress(const char* input);
private:
    const char* mPath;
};

} // namespace Archive