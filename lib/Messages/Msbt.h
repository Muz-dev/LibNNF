#pragma once

namespace Messages {

class Msbt {
public:
    Msbt(const char* path) : mPath(path) {}
    void convertToTxt(const char* txtFile);
private:
    const char* mPath;
};

} // namespace Messages