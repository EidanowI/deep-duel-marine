#pragma once

#include "../../Dependencies/imgui/imgui.h"
#include "../../Dependencies/imgui/imgui_impl_win32.h"
#include "../../Dependencies/imgui/imgui_impl_dx11.h"


class ImGuiRenderer {
public:
	static void Initialize() noexcept;
	static void Terminate() noexcept;

	static void StartFrame() noexcept;
	static void EndFrame() noexcept;
};