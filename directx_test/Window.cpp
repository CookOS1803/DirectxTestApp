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
	: width(newWidth), height(newHeight), x(0.f), y(0.f), z(-5.f), ax(0.f), ay(0.f), az(0.f)
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
	{
		const float step = 0.1f;
		const float astep = 0.1f;

		switch (wParam)
		{
		case 'a': case 'A':
			x -= step;
			break;
		case 'd': case 'D':
			x += step;
			break;
		case 's': case 'S':
			z -= step;
			break;
		case 'w': case 'W':
			z += step;
			break;
		case VK_DOWN:
			ax += astep;
			break;
		case VK_UP:
			ax -= astep;
			break;
		case VK_LEFT:
			ay -= astep;
			break;
		case VK_RIGHT:
			ay += astep;
			break;
		default:
			break;
		}

		ax = std::min(std::max(-XM_PIDIV4, ax), XM_PIDIV4);
		az = std::min(std::max(-XM_PIDIV4, az), XM_PIDIV4);

		pGfx->SetCameraPosition(x, y, z);
		pGfx->SetCameraRotation(ax, ay, az);

		break;
	}
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
		{ {-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
		{ {1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
		{ {1.0f, 1.0f, 1.0f},  {0.0f, 1.0f, 1.0f, 1.0f} },
		{ {-1.0f, 1.0f, 1.0f},  {1.0f, 0.0f, 0.0f, 1.0f} },
		{ {-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },
		{ {1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 0.0f, 1.0f} },
		{ {1.0f, -1.0f, 1.0f},  {1.0f, 1.0f, 1.0f, 1.0f} },
		{ {-1.0f, -1.0f, 1.0f},  {0.0f, 0.0f, 0.0f, 1.0f} },
	});
}
