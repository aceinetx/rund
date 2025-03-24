#include <rund.hpp>
#include <imgui.h>

void rund::RenderFrame(){
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({(float)rund::windowWidth, (float)rund::windowHeight});

    ImGui::Begin("rund", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::End();
}