#include "MainWindow.h"

#include "../Dependencies/imGUI/imgui.h"
#include "../Dependencies/imgui/imgui_impl_win32.h"
#include "../Dependencies/imgui/imgui_impl_dx11.h"



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



MainWindow::WindowClass MainWindow::WindowClass::s_wndClass = MainWindow::WindowClass();
const wchar_t* MainWindow::WindowClass::s_wndClassName = L"ddm_wind_class";


unsigned short MainWindow::s_windowWidth;
unsigned short MainWindow::s_windowHeight;
HWND MainWindow::s_hWnd = nullptr;



MainWindow::WindowClass::WindowClass() noexcept : m_hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = WindowProcess;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	//wc.hIcon = (HICON)LoadImage(m_hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 64, 64, 0);
	wc.hIcon = (HICON)0;
	wc.hCursor = LoadCursorW(0, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	//wc.hIconSm = (HICON)LoadImage(m_hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0);
	wc.hIconSm = (HICON)0;
	RegisterClassEx(&wc);
}
MainWindow::WindowClass::~WindowClass() noexcept
{
	UnregisterClass(s_wndClassName, GetInstance());
}



void MainWindow::Initialize() noexcept {
	s_windowWidth = GetSystemMetrics(SM_CXSCREEN);
	s_windowHeight = GetSystemMetrics(SM_CYSCREEN);

	s_hWnd = CreateWindowW(WindowClass::GetName(),
		MAIN_WINDOW_NAME,
		WS_POPUP,
		(GetSystemMetrics(SM_CXSCREEN) - s_windowWidth) / 2,///takes centre of a screen
		(GetSystemMetrics(SM_CYSCREEN) - s_windowHeight) / 2,///takes centre of a screen
		s_windowWidth,
		s_windowHeight,
		nullptr,
		nullptr,
		WindowClass::GetInstance(),
		nullptr
	);

	ShowWindow(s_hWnd, SW_HIDE);
	ImGui_ImplWin32_Init(s_hWnd);
}
void MainWindow::Terminate() noexcept
{
	ImGui_ImplWin32_Shutdown();

	if (s_hWnd != nullptr) {
		DestroyWindow(s_hWnd);
		s_hWnd = nullptr;
	}
}

std::optional<int> MainWindow::ProcessMsg() noexcept {
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return std::optional<int>(msg.wParam);
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return std::optional<int>{};
}

HWND MainWindow::GetHinstnce() noexcept {
	return s_hWnd;
}

unsigned short MainWindow::GetWindowWidth() noexcept {
	return s_windowWidth;
}
unsigned short MainWindow::GetWindowHeight() noexcept {
	return s_windowHeight;
}

void MainWindow::SetCursorVisibility(bool isVisible) noexcept {
	if (isVisible) {
		while (ShowCursor(TRUE) < 0);
	}
	else {
		while (ShowCursor(FALSE) >= 0);
	}
}
void MainWindow::FixateCursor(bool isFixed) noexcept {
	if (isFixed) {
		POINT cursorPos;
		GetCursorPos(&cursorPos);

		RECT rect;
		rect.right = cursorPos.x + 0;
		rect.left = cursorPos.x - 0;
		rect.top = cursorPos.y - 0;
		rect.bottom = cursorPos.y + 0;

		ClipCursor(&rect);
	}
	else {
		ClipCursor(nullptr);
	}
}
void MainWindow::SetWindowVisibility(bool isVisible) noexcept {
	if (isVisible) {
		ShowWindow(s_hWnd, SW_SHOW);
	}
	else {
		ShowWindow(s_hWnd, SW_HIDE);
	}
}

LRESULT WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_PAINT: {
		break;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(MainWindow::s_hWnd, &ps);
		HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
		HBRUSH brush_red = CreateSolidBrush(RGB(255, 0, 0));
		RECT a;
		a.left = 30;
		a.bottom = 30;
		a.right = rand()%400;
		a.top = 60;
		FillRect(hdc, &ps.rcPaint, brush);
		FillRect(hdc, &a, brush_red);
		DeleteObject(brush);
		DeleteObject(brush_red);
		EndPaint(MainWindow::s_hWnd, &ps);
	}//FixateCursor
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		if ((unsigned char)wParam == 'W') {
			MainWindow::FixateCursor(true);
		}
		else if ((unsigned char)wParam == 'Q') {
			MainWindow::FixateCursor(false);
		}
		else if ((unsigned char)wParam == 'E') {
			MainWindow::SetCursorVisibility(false);
		}
		else if ((unsigned char)wParam == 'R') {
			MainWindow::SetCursorVisibility(true);
		}
		break;
	}
	/*case WM_KILLFOCUS:
	{
		if (Engine::Manager::ConfigManager::GetGraphicsConfig()->window_mode == Engine::Manager::ConfigManager::GraphicsConfig::WINDOW_MODE_FULLSCREEN) {
			ShowWindow(Engine::MainWindow::GetHwnd(), SW_MINIMIZE);
		}
		break;
	}*/
	}

	/*
	* case WM_SIZE:
			//MoveWindow(MainWindow::s_hWnd, 0, 0, MainWindow::s_window_width, MainWindow::s_window_height, TRUE);
			//InvalidateRect(MainWindow::s_hWnd, nullptr, true);
			//UpdateWindow(MainWindow::s_hWnd);
			//SetWindowPos(MainWindow::s_hWnd, nullptr, 0, 0, LOWORD(lParam), HIWORD(lParam), SWP_FRAMECHANGED | SWP_NOMOVE);
			//UpdateWindow(MainWindow::s_hWnd);
			break;
		}
	*
	case WM_INPUT:
	{
		if (InputSystem::S_isCursorEnable) break;

		UINT size;

		if (GetRawInputData(
			(HRAWINPUT)lParam,
			RID_INPUT,
			nullptr,
			&size,
			sizeof(RAWINPUTHEADER)) == -1)
		{
			break;
		}
		InputSystem::S_rawBuffer.resize(size);

		if (GetRawInputData(
			(HRAWINPUT)lParam,
			RID_INPUT,
			InputSystem::S_rawBuffer.data(),
			&size,
			sizeof(RAWINPUTHEADER)) != size)
		{
			break;
		}

		RAWINPUT ri = *(RAWINPUT*)InputSystem::S_rawBuffer.data();
		if (ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		{
			InputSystem::S_cursorDeltaX = ri.data.mouse.lLastX;
			InputSystem::S_cursorDeltaY = ri.data.mouse.lLastY;
		}
		break;
	}

	case WM_ACTIVATE:
	{
		break;
	}

	case WM_KILLFOCUS:
	{
		//callse when the window lost the focus
		InputSystem::KillFocus();
		break;
	}

	case WM_SETFOCUS:
	{
		//called when the window is back in focus
		break;
	}

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		if (!(lParam & 0b01000000000000000000000000000000)) {//30 bit is 1 if key was pressed before this message
			InputSystem::S_ppFunction_ONCE_KEY[(unsigned char)wParam](lParam);
			InputSystem::AddPressedKey((unsigned char)wParam, lParam);
		}
		break;
	}

	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		InputSystem::S_ppFunction_KEY_RELEASE[(unsigned char)wParam](lParam);
		InputSystem::RemovePressedKey((unsigned char)wParam);
		break;
	}

	case WM_CHAR:
	{
		InputSystem::S_pFunction_ONCHAR(wParam, lParam);
		break;
	}

	case WM_MOUSEMOVE:
	{
		short posX = *((short*)(char*)&lParam);
		short posY = *((short*)((char*)&lParam + 2));

		if (posX >= 0 && posX < Window::S_WindowWidth && posY >= 0 && posY < Window::S_WindowHeight)
		{

			//mouse.OnMouseMove(posX, posY);
			InputSystem::S_pFunction_MOVE_MOUSE(posX, posY);


			if (!InputSystem::S_isCursorInClientArea)
			{
				SetCapture(hWnd);
				InputSystem::S_isCursorInClientArea = true;
				InputSystem::S_pFunction_OnCursorEnterCA();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
			{
				InputSystem::S_pFunction_MOVE_MOUSE(posX, posY);
			}
			else
			{
				ReleaseCapture();
				InputSystem::KillFocus();
				InputSystem::S_isCursorInClientArea = false;
				InputSystem::S_pFunction_OnCursorLeaveCA();
			}
		}

		break;
	}
	case WM_LBUTTONDOWN:
	{
		//if (wParam == (long long)0b00000101) MessageBox(nullptr, L"LINE: ", L"afaf", MB_OK | MB_ICONEXCLAMATION);
		InputSystem::S_ppFunction_ONCE_MBUTTON[InputSystem::LEFT_MOUSE_BUTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::AddPressedMButton(InputSystem::LEFT_MOUSE_BUTON, wParam, lParam);
		break;
	}

	case WM_LBUTTONUP:
	{
		InputSystem::S_ppFunction_MOUSE_RELEASE[InputSystem::LEFT_MOUSE_BUTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::RemovePressedMButton(InputSystem::LEFT_MOUSE_BUTON);
		break;
	}

	case WM_RBUTTONDOWN:
	{
		InputSystem::S_ppFunction_ONCE_MBUTTON[InputSystem::RIGHT_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::AddPressedMButton(InputSystem::RIGHT_MOUSE_BUTTON, wParam, lParam);
		break;
	}

	case WM_RBUTTONUP:
	{
		InputSystem::S_ppFunction_MOUSE_RELEASE[InputSystem::RIGHT_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::RemovePressedMButton(InputSystem::RIGHT_MOUSE_BUTTON);
		break;
	}

	case WM_MBUTTONDOWN:
		InputSystem::S_ppFunction_ONCE_MBUTTON[InputSystem::MIDDLE_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::AddPressedMButton(InputSystem::MIDDLE_MOUSE_BUTTON, wParam, lParam);
		break;
	case WM_MBUTTONUP:
	{
		InputSystem::S_ppFunction_MOUSE_RELEASE[InputSystem::MIDDLE_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::RemovePressedMButton(InputSystem::MIDDLE_MOUSE_BUTTON);
		break;
	}

	case WM_XBUTTONDOWN:
	{
		if (*(unsigned short*)(((char*)&wParam) + 2) & XBUTTON1) {
			InputSystem::S_ppFunction_ONCE_MBUTTON[InputSystem::XBUTTON1_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
			InputSystem::AddPressedMButton(InputSystem::XBUTTON1_MOUSE_BUTTON, wParam, lParam);
		}
		else if (*(unsigned short*)(((char*)&wParam) + 2) & XBUTTON2) {
			InputSystem::S_ppFunction_ONCE_MBUTTON[InputSystem::XBUTTON2_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
			InputSystem::AddPressedMButton(InputSystem::XBUTTON2_MOUSE_BUTTON, wParam, lParam);
		}
		break;
	}

	case WM_XBUTTONUP:
	{
		if (*(unsigned short*)(((char*)&wParam) + 2) & XBUTTON1) {
			InputSystem::S_ppFunction_MOUSE_RELEASE[InputSystem::XBUTTON1_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
			InputSystem::RemovePressedMButton(InputSystem::XBUTTON1_MOUSE_BUTTON);
		}
		else if (*(unsigned short*)(((char*)&wParam) + 2) & XBUTTON2) {
			InputSystem::S_ppFunction_MOUSE_RELEASE[InputSystem::XBUTTON2_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
			InputSystem::RemovePressedMButton(InputSystem::XBUTTON2_MOUSE_BUTTON);
		}
		break;
	}

	case WM_MOUSEWHEEL:
	{
		//if (*(short*)&wParam != (short)0b00000100) break; -- BREAK IF YOU DOESNT HOLD SHIFT WHILE SPIN THE WHEEL
		//InputSystem::func_WHEELMOVE(wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::S_wheelDelta += (short)*((char*)&wParam + 2);
		while (InputSystem::S_wheelDelta >= WHEEL_DELTA) {
			InputSystem::S_wheelDelta -= WHEEL_DELTA;
			InputSystem::S_pFunction_WHEELUP_STEP(*((short*)(void*)&lParam), *((short*)((char*)&lParam + 2)));
		}
		while (InputSystem::S_wheelDelta <= -WHEEL_DELTA) {
			InputSystem::S_wheelDelta += WHEEL_DELTA;
			InputSystem::S_pFunction_WHEELDOWN_STEP(*((short*)(void*)&lParam), *((short*)((char*)&lParam + 2)));
		}
		break;
	}

	}*/

	return DefWindowProc(hWnd, msg, wParam, lParam);
}