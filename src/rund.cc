#include <rund.hpp>
#include <imgui.h>
#include <filesystem>
#include <vector>
#include <style.hpp>
#include <codecvt>
#include <locale>
#include <fmt/format.h>

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

    if(!rund::assetsDir.empty()){
        ImGuiIO &io = ImGui::GetIO();
        io.IniFilename = nullptr;

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

    ImGui::Button("OK");
    ImGui::SameLine();
    if(ImGui::Button("Cancel")) rund::done = true;
    ImGui::SameLine();
    if(ImGui::Button("Browse")) rund::fileBrowser = !rund::fileBrowser;
}

void rund::RenderFileBrowser(){
    if(ImGui::Button("X")) rund::fileBrowser = !rund::fileBrowser;
    ImGui::SameLine();
    ImGui::InputText("##path", rund::fileBrowserPath, RUND_STRING_SIZE);

    for (const auto & entry : std::filesystem::directory_iterator(rund::fileBrowserPath)){
        if(!entry.is_directory()) continue;
        //ImGui::Button(std::format(" {}", entry.path().filename()).c_str());
        ImGui::Button(fmt::format(" {}", entry.path().filename().string()).c_str());
    }

    for (const auto & entry : std::filesystem::directory_iterator(rund::fileBrowserPath)){
        if(!entry.is_regular_file()) continue;
        // 
        ImGui::Button(fmt::format(" {}", entry.path().filename().string()).c_str());
    }
}

void rund::RenderFrame(){
    ImGui::SetNextWindowPos({-1.0f, -1.0f});
    ImGui::SetNextWindowSize({(float)(rund::windowWidth-14), (float)(rund::windowHeight-35)});

    ImGui::Begin("rund", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    if(rund::assetsDir.empty()){
        ImGui::Text("Check failed: rund::assetsDir.empty() == true");
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