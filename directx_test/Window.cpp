#include "Window.h"
#include <sstream>
#include "WindowsMessageMap.h"

Window::WindowClass Window::WindowClass::wndClassInstance {};

const wchar_t* Window::WindowClass::Name() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::HInstance() noexcept
{
	return wndClassInstance.hInstance;
}

Window::WindowClass::WindowClass() noexcept
	: hInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc{ 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = HInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = Name();
	wc.lpszMenuName = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, HInstance());
}

Window::Window(int newWidth, int newHeight, const wchar_t* name) noexcept
	: width(newWidth), height(newHeight)
{
	RECT wr{};
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	const auto dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	AdjustWindowRect(&wr, dwStyle, FALSE);

	currHwnd = CreateWindow(WindowClass::Name(), name, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowClass::HInstance(), this);

	ShowWindow(currHwnd, SW_SHOWDEFAULT);

	pGfx = std::make_unique<Graphics>(currHwnd, width, height);
}

Window::~Window()
{
	DestroyWindow(currHwnd);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pwnd = static_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pwnd));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
	
		return pwnd->HandleMsg(hwnd, msg, wParam, lParam);
	}
	else
		return DefWindowProc(hwnd, msg, wParam, lParam); 
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Window* const pwnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	return pwnd->HandleMsg(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	static WindowsMessageMap mm;
	OutputDebugString(mm.Parse(msg, lParam, wParam).c_str());

	switch (msg)
	{
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	case WM_CLOSE:
		PostQuitMessage(69);
		break;
	case WM_KEYDOWN: case WM_SYSKEYDOWN:
	{
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled())
			kbd.OnKeyPressed(static_cast<UCHAR>(wParam));

		break;
	}
	case WM_KEYUP: case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<UCHAR>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<UCHAR>(wParam));
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
