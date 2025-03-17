#include "ImGuiRender.h"
#include "../MainWindow/MainWindow.h"





void ImGuiRenderer::Initialize() noexcept {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
}
void ImGuiRenderer::Terminate() noexcept {
    ImGui::DestroyContext();
}

void ImGuiRenderer::NewFrame() noexcept {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::SetNextWindowPos(ImVec2(MainWindow::GetWindowWidth() * 0.06, MainWindow::GetWindowWidth() * 0.16));
    ImGui::Begin("Test window", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;

    if (ImGui::Button("Play", ImVec2(MainWindow::GetWindowWidth() * 0.2, MainWindow::GetWindowWidth() * 0.02))) {
        MessageBeep(0xFFFFFFFF);
    }
    ImGui::End();
   
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}