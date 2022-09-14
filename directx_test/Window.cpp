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
	: width(newWidth), height(newHeight), x(0.f), y(0.f)
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

	DrawDiamond();
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

Graphics& Window::Gfx()
{
	return *pGfx;
}

LRESULT CALLBACK Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	static WindowsMessageMap mm;
	OutputDebugString(mm.Parse(msg, lParam, wParam).c_str());

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		break;
	case WM_KEYDOWN:
		
		switch (wParam)
		{
		case 'a': case 'A':
			x -= 0.01f;
			break;
		case 'd': case 'D':
			x += 0.01f;
			break;
		case 's': case 'S':
			y -= 0.01f;
			break;
		case 'w': case 'W':
			y += 0.01f;
			break;
		default:
			break;
		}

		DrawDiamond();

		break;
	case WM_CHAR:
	{
		static std::wstring title;

		title.push_back(wParam);
		SetWindowText(hwnd, title.c_str());

		break;
	}
	case WM_LBUTTONDOWN:
	{
		POINTS pt = MAKEPOINTS(lParam);
		std::wostringstream oss;

		oss << "(" << pt.x << ", " << pt.y << ")";
		SetWindowText(hwnd, oss.str().c_str());

		break;
	}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Window::DrawDiamond()
{
	pGfx->CreateVertexBuffer({
			{x + 0.f,  y + 0.f, 0.5f},
			{x + 0.05f, y + 0.1f, 0.5f},
			{x + 0.1f,  y + 0.f, 0.5f},
			{x + 0.05f, y - 0.1f, 0.5f},
			{x + 0.f,  y + 0.f, 0.5f}
		});
}
