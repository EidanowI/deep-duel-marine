#include "ImGuiRender.h"
#include "../MainWindow/MainWindow.h"





void ImGuiRenderer::Initialize() noexcept {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.Fonts->AddFontFromFileTTF("C:/Users/Fiodar/Desktop/GAVS.otf", 32.0f);
    ImGui::StyleColorsDark();
}
void ImGuiRenderer::Terminate() noexcept {
    ImGui::DestroyContext();
}

void ImGuiRenderer::StartFrame() noexcept {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}
void ImGuiRenderer::EndFrame() noexcept {
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}