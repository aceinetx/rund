#include <rund.hpp>
#include <imgui.h>
#include <filesystem>
#include <vector>
#include <style.hpp>

std::string rund::assetsDir = "";
char* rund::input = nullptr;

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
    rund::input = new char[RUND_INPUT_SIZE];

    memset(rund::input, 0, RUND_INPUT_SIZE);

    if(!rund::assetsDir.empty()){
        ImGuiIO &io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF((rund::assetsDir+"Nunito-Regular.ttf").c_str(), 21.5f);

        SetupImGuiStyle();
    }
}

void rund::Atexit(){
    delete[] rund::input;
}

void rund::RenderFrame(){
    ImGui::SetNextWindowPos({-1.0f, -1.0f});
    ImGui::SetNextWindowSize({(float)rund::windowWidth, (float)rund::windowHeight});

    ImGui::Begin("rund", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    if(rund::assetsDir.empty()){
        ImGui::Text("Check failed: rund::assetsDir.empty() == true");
        ImGui::End();
        return;
    }

    ImGui::Text("Type a name of a program, folder, document, or Internet");
    ImGui::Text("resource, and rund will open it for you");

    ImGui::InputText("Open", rund::input, RUND_INPUT_SIZE);

    ImGui::Separator();

    ImGui::Button("OK");
    ImGui::SameLine();
    if(ImGui::Button("Cancel")) rund::done = true;
    ImGui::SameLine();
    ImGui::Button("Browse");

    ImGui::End();
}