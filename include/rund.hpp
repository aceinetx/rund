#pragma once
#define RUND_STRING_SIZE 32767

#include <string>

namespace rund {
    extern const unsigned int windowHeight;
    extern const unsigned int windowWidth;
    extern std::string assetsDir;
    extern char *input;
    extern bool done;
    extern bool fileBrowser;
    extern char* fileBrowserPath;

    void Main();
    void Atexit();
    void RenderMain();
    void RenderFileBrowser();
    void RenderFrame();
    std::string FindAssetsDir();
}