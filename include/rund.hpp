#pragma once
#define RUND_INPUT_SIZE 32767

#include <string>

namespace rund {
    extern const unsigned int windowHeight;
    extern const unsigned int windowWidth;
    extern std::string assetsDir;
    extern char *input;
    extern bool done;

    void Main();
    void Atexit();
    void RenderFrame();
    std::string FindAssetsDir();
}