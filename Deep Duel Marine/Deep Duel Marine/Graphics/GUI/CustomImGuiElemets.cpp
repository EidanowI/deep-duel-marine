#include "CustomImGuiElemets.h"



ImVec2  operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
ImVec2  operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }


bool MainMenuButton(const char* label, ImGuiButtonFlags flags)
{
	ImVec2 size_arg = ImVec2(MainWindow::GetWindowWidth() / 4.2, MainWindow::GetWindowHeight() / 20);

	using namespace ImGui;
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	// Render
	ImColor buton_active_color = ImColor(100, 100, 100, 255 - 100);
	ImColor buton_hovered_color = ImColor(115, 115, 115, 255 - 115);
	ImColor buton_color = ImColor(0, 0, 0, 0);

	const ImU32 col = ((held && hovered) ? buton_active_color : hovered ? buton_hovered_color : buton_color);
	RenderNavHighlight(bb, id);
	RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

	if (g.LogEnabled)
		LogSetNextTextDecoration("[", "]");
	RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

	IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);

	return pressed;
}
bool MainMenuGitHubButton(ImGuiButtonFlags flags)
{
	static Texture github_logo_tex = Texture("GitHubLogo.png");
	const char* label = "GitHubButton";
	ImVec2 size_arg = ImVec2(MainWindow::GetWindowHeight() / 16.62f * 2.7f, MainWindow::GetWindowHeight() / 16.62f);


	using namespace ImGui;
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);

	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	// Render
	ImColor buton_active_color = ImColor(150, 150, 150, 255);
	ImColor buton_hovered_color = ImColor(255, 255, 255, 150);
	ImColor buton_color = ImColor(255, 255, 255, 255);

	const ImVec4 col = ((held && hovered) ? buton_active_color : hovered ? buton_hovered_color : buton_color);

	ImGui::SetCursorPos(ImVec2(7, MainWindow::GetWindowHeight() - MainWindow::GetWindowHeight() / 16.62f - 7));
	ImGui::Image(github_logo_tex.GetShaderResView(), size_arg, ImVec2(0, 0), ImVec2(1, 1), col);

	return pressed;
}
bool MainMenuTelegramButton(ImGuiButtonFlags flags)
{
	static Texture telegram_logo_tex = Texture("TelegramLogo.png");
	const char* label = "TelegramButton";
	ImVec2 size_arg = ImVec2(MainWindow::GetWindowHeight() / 16.62f, MainWindow::GetWindowHeight() / 16.62f);

	using namespace ImGui;
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	// Render
	ImColor buton_active_color = ImColor(150, 150, 150, 255);
	ImColor buton_hovered_color = ImColor(255, 255, 255, 150);
	ImColor buton_color = ImColor(255, 255, 255, 255);

	const ImVec4 col = ((held && hovered) ? buton_active_color : hovered ? buton_hovered_color : buton_color);

	ImGui::SetCursorPos(ImVec2(16 + MainWindow::GetWindowHeight() / 16.62f * 2.7f, MainWindow::GetWindowHeight() - MainWindow::GetWindowHeight() / 16.62f - 7));
	ImGui::Image(telegram_logo_tex.GetShaderResView(), size_arg, ImVec2(0, 0), ImVec2(1, 1), col);

	return pressed;
}