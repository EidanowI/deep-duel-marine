#pragma once
#include <Windows.h>
#include <optional>

#include "../DefineConfig.h"



LRESULT WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class MainWindow {
private:
	friend LRESULT WindowProcess(HWND, UINT, WPARAM, LPARAM);

	class WindowClass // singleton
	{
	public:
		static const wchar_t* GetName() noexcept {
			return s_wndClassName;
		}
		static HINSTANCE GetInstance() noexcept {
			return s_wndClass.m_hInst;
		}

	private:
		WindowClass() noexcept;
		~WindowClass() noexcept;
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static WindowClass s_wndClass;
		static const wchar_t* s_wndClassName;
		HINSTANCE m_hInst;
	};

public:
	static void Initialize() noexcept;
	static void Terminate() noexcept;

	static std::optional<int> ProcessMsg() noexcept;

	static HWND GetHinstnce() noexcept;

	static unsigned short GetWindowWidth() noexcept;
	static unsigned short GetWindowHeight() noexcept;

	static void SetCursorVisibility(bool isVisible) noexcept;
	static void FixateCursor(bool isFixed) noexcept;
	static void SetWindowVisibility(bool isVisible) noexcept;

private:
	static unsigned short s_windowWidth;
	static unsigned short s_windowHeight;
	static HWND s_hWnd;
};