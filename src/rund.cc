#include <rund.hpp>
#include <imgui.h>
#include <filesystem>
#include <vector>
#include <style.hpp>
#include <codecvt>
#include <locale>
#include <fmt/format.h>
#include <windows.h>

std::string rund::assetsDir = "";
char* rund::input = nullptr;
char* rund::fileBrowserPath = nullptr;
bool rund::fileBrowser = false;

std::string rund::FindAssetsDir(){
    std::vector<std::string> paths = {"../assets/", "./assets/", "../../assets/"};
    for(std::string path : paths){
        if(std::filesystem::exists(path)){
            return path;
        }
    }
    return "";
}

void rund::Main(){
    rund::assetsDir = rund::FindAssetsDir();
    rund::input = new char[RUND_STRING_SIZE];
    rund::fileBrowserPath = new char[RUND_STRING_SIZE];

    memset(rund::input, 0, RUND_STRING_SIZE);
    memset(rund::fileBrowserPath, 0, RUND_STRING_SIZE);

    strncpy(rund::fileBrowserPath, "C:\\", RUND_STRING_SIZE);

    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr;
    if(!rund::assetsDir.empty()){
        ImVector<ImWchar> ranges;
        ImFontGlyphRangesBuilder builder;
        builder.AddText("");
        builder.AddText("1234567890~`QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm ,.!@#$%^&*():\"'[]{}\\/");
        builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
        builder.BuildRanges(&ranges);

        io.Fonts->AddFontFromFileTTF((rund::assetsDir+"IosevkaNerdFontMono-Regular.ttf").c_str(), 21.5f, nullptr, ranges.Data);
        io.Fonts->Build();

        SetupImGuiStyle();
    }
}

void rund::Atexit(){
    delete[] rund::input;
    delete[] rund::fileBrowserPath;
}

void rund::RenderMain(){
    ImGui::Text("Type a name of a program, folder, document, or");
    ImGui::Text("Internet resource, and rund will open it for you");

    ImGui::InputText("Open", rund::input, RUND_STRING_SIZE);

    ImGui::Separator();

    if(ImGui::Button("OK")){
        if((rund::input[0] >= 'A' && rund::input[0] <= 'Z') && rund::input[1] == ':'){ // check if it's a disk

        }
        ShellExecuteA(NULL, "open", rund::input, NULL, NULL, SW_SHOWDEFAULT);
    }
    ImGui::SameLine();
    if(ImGui::Button("Cancel")) rund::done = true;
    ImGui::SameLine();
    if(ImGui::Button("Browse")) rund::fileBrowser = !rund::fileBrowser;
}

void rund::RenderFileBrowser(){
    if(ImGui::Button("X")) rund::fileBrowser = !rund::fileBrowser;
    ImGui::SameLine();
    ImGui::InputText("##path", rund::fileBrowserPath, RUND_STRING_SIZE);

    if(!std::filesystem::exists(rund::fileBrowserPath)){
        ImGui::Text("Invalid path");
        return;
    }

    if(ImGui::Button("..")){
        strncpy(rund::fileBrowserPath, std::filesystem::path(rund::fileBrowserPath).parent_path().string().c_str(), RUND_STRING_SIZE);
    }

    try {
        for (const auto & entry : std::filesystem::directory_iterator(rund::fileBrowserPath)){
            if(!entry.is_directory()) continue;
            if(ImGui::Button(fmt::format(" {}", entry.path().filename().string()).c_str())){
                strncpy(rund::fileBrowserPath, entry.path().string().c_str(), RUND_STRING_SIZE);
            }
        }

        for (const auto & entry : std::filesystem::directory_iterator(rund::fileBrowserPath)){
            if(!entry.is_regular_file()) continue;
            if(ImGui::Button(fmt::format(" {}", entry.path().filename().string()).c_str())){
                rund::fileBrowser = !rund::fileBrowser;
                strncpy(rund::input, entry.path().string().c_str(), RUND_STRING_SIZE);
            }
        }
    } catch (std::filesystem::filesystem_error &e){
        std::string what = "";
        bool afterColumn = false;
        for(char c : std::string(e.what())){
            if(afterColumn) what.push_back(c);
            if(c == ':') afterColumn = true;
        }

        ImGui::Text("%s", what.c_str());
    }
}

void rund::RenderFrame(){
    ImGui::SetNextWindowPos({-1.0f, -1.0f});
    ImGui::SetNextWindowSize({(float)(rund::windowWidth-14), (float)(rund::windowHeight-35)});

    ImGui::Begin("rund", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    if(rund::assetsDir.empty()){
        ImGui::Text("Check failed: rund::assetsDir.empty() == true");
        ImGui::Text("(Are the assets installed?)");
        ImGui::End();
        return;
    }

    if(!rund::fileBrowser){
        rund::RenderMain();
    } else {
        rund::RenderFileBrowser();
    }

    ImGui::End();
}