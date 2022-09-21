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
			pGfx->GetCamera().Translate(-step, 0, 0);
			break;
		case 'd': case 'D':
			pGfx->GetCamera().Translate(step, 0, 0);
			break;
		case 's': case 'S':
			pGfx->GetCamera().Translate(0, 0, -step);
			break;
		case 'w': case 'W':
			pGfx->GetCamera().Translate(0, 0, step);
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

		ax = std::min(std::max(-XM_PIDIV2, ax), XM_PIDIV2);
		az = std::min(std::max(-XM_PIDIV2, az), XM_PIDIV2);

		pGfx->GetCamera().SetRotation(ax, ay, az);

		break;
	}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Window::DrawDiamond()
{
	//pGfx->CreateVertexBuffer({
	//	{ {-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },
	//	{ {1.0f, -1.0f, -1.0f},  {1.0f, 1.0f, 0.0f, 1.0f} },
	//	{ {1.0f, -1.0f, 1.0f},   {1.0f, 1.0f, 1.0f, 1.0f} },
	//	{ {-1.0f, -1.0f, 1.0f},  {0.0f, 0.0f, 0.0f, 1.0f} },
	//	{ {0.0f, 1.0f, 0.0f},  {1.0f, 0.0f, 0.5f, 1.0f} },
	//	});
	//
	//pGfx->CreateIndexBuffer({
	//	0, 1, 2,
	//	2, 3, 0,
	//
	//	0, 4, 1,
	//
	//	3, 4, 0,
	//
	//	1, 4, 2,
	//
	//	2, 4, 3
	//
	//	});

	pGfx->CreateVertexBuffer({
		{{-0.5f, -1.f, 0.f}, {.5f, 0.f, .5f, 1.f}},
		{{-std::sin(XM_PIDIV4) / 2.f, -1.f, std::sin(XM_PIDIV4) / 2.f}, {.5f, 0.f, 0.f, 1.f}},
		{{0.f, -1.f, 0.5f}, {0.f, .5f, .5f, 1.f}},
		{{std::sin(XM_PIDIV4) / 2.f, -1.f, std::sin(XM_PIDIV4) / 2.f}, {0.f, 0.f, .5f, 1.f}},
		{{0.5f, -1.f, 0.f}, {.5f, .5f, 0.f, 1.f}},
		{{std::sin(XM_PIDIV4) / 2.f, -1.f, -std::sin(XM_PIDIV4) / 2.f}, {0.f, .5f, 0.f, 1.f}},
		{{0.f, -1.f, -0.5f}, {0.f, 0.f, .5f, 1.f}},
		{{-std::sin(XM_PIDIV4) / 2.f, -1.f, -std::sin(XM_PIDIV4) / 2.f}, {.5f, 0.f, 0.f, 1.f}},

		{{-0.5f, 1.f, 0.f}, {.5f, 0.f, .5f, 1.f}},
		{{-std::sin(XM_PIDIV4) / 2.f, 1.f, std::sin(XM_PIDIV4) / 2.f}, {.5f, 0.f, 0.f, 1.f}},
		{{0.f, 1.f, 0.5f}, {0.f, .5f, .5f, 1.f}},
		{{std::sin(XM_PIDIV4) / 2.f, 1.f, std::sin(XM_PIDIV4) / 2.f}, {0.f, 0.f, .5f, 1.f}},
		{{0.5f, 1.f, 0.f}, {.5f, .5f, 0.f, 1.f}},
		{{std::sin(XM_PIDIV4) / 2.f, 1.f, -std::sin(XM_PIDIV4) / 2.f}, {0.f, .5f, 0.f, 1.f}},
		{{0.f, 1.f, -0.5f}, {0.f, 0.f, .5f, 1.f}},
		{{-std::sin(XM_PIDIV4) / 2.f, 1.f, -std::sin(XM_PIDIV4) / 2.f}, {.5f, 0.f, 0.f, 1.f}},

		{{-1.f, -1.f, 0.f}, {1.f, 0.f, 1.f, 1.f}},
		{{-std::sin(XM_PIDIV4), -1.f, std::sin(XM_PIDIV4)}, {1.f, 0.f, 0.f, 1.f}},
		{{0.f, -1.f, 1.f}, {0.f, 1.f, 1.f, 1.f}},
		{{std::sin(XM_PIDIV4), -1.f, std::sin(XM_PIDIV4)}, {0.f, 0.f, 1.f, 1.f}},
		{{1.f, -1.f, 0.f}, {1.f, 1.f, 0.f, 1.f}},
		{{std::sin(XM_PIDIV4), -1.f, -std::sin(XM_PIDIV4)}, {0.f, 1.f, 0.f, 1.f}},
		{{0.f, -1.f, -1.f}, {0.f, 0.f, 1.f, 1.f}},
		{{-std::sin(XM_PIDIV4), -1.f, -std::sin(XM_PIDIV4)}, {1.f, 0.f, 0.f, 1.f}},

		{{-1.f, 1.f, 0.f}, {1.f, 0.f, 1.f, 1.f}},
		{{-std::sin(XM_PIDIV4), 1.f, std::sin(XM_PIDIV4)}, {1.f, 0.f, 0.f, 1.f}},
		{{0.f, 1.f, 1.f}, {0.f, 1.f, 1.f, 1.f}},
		{{std::sin(XM_PIDIV4), 1.f, std::sin(XM_PIDIV4)}, {0.f, 0.f, 1.f, 1.f}},
		{{1.f, 1.f, 0.f}, {1.f, 1.f, 0.f, 1.f}},
		{{std::sin(XM_PIDIV4), 1.f, -std::sin(XM_PIDIV4)}, {0.f, 1.f, 0.f, 1.f}},
		{{0.f, 1.f, -1.f}, {0.f, 0.f, 1.f, 1.f}},
		{{-std::sin(XM_PIDIV4), 1.f, -std::sin(XM_PIDIV4)}, {1.f, 0.f, 0.f, 1.f}},
		});

	pGfx->CreateIndexBuffer({

		// inner

		0, 8, 9,
		9, 1, 0,

		1, 9, 10,
		10, 2, 1,

		2, 10, 11,
		11, 3, 2,

		3, 11, 12,
		12, 4, 3,

		4, 12, 13,
		13, 5, 4,

		5, 13, 14,
		14, 6, 5,

		6, 14, 15,
		15, 7, 6,

		7, 15, 8,
		8, 0, 7,

		//outer

		16, 25, 24,
		16, 17, 25,

		17, 26, 25,
		17, 18, 26,

		18, 27, 26,
		18, 19, 27,

		19, 28, 27,
		19, 20, 28,

		20, 29, 28,
		20, 21, 29,

		21, 30, 29,
		21, 22, 30,

		22, 31, 30,
		22, 23, 31,

		23, 24, 31,
		23, 16, 24,

		//top

		8, 24, 25,
		25, 9, 8,

		9, 25, 26,
		26, 10, 9,

		10, 26, 27,
		27,	11, 10,

		11, 27, 28,
		28, 12, 11,

		12, 28, 29,
		29, 13, 12,

		13, 29, 30,
		30, 14, 13,

		14, 30, 31,
		31, 15, 14,

		15, 31, 24,
		24, 8, 15,

		//bottom

		0, 17, 16,
		1, 17, 0,

		1, 18, 17,
		2, 18, 1,

		2, 19, 18,
		3, 19, 2,

		3, 20, 19,
		4, 20, 3,

		4, 21, 20,
		5, 21, 4,

		5, 22, 21,
		6, 22, 5,

		6, 23, 22,
		7, 23, 6,

		7, 16, 23,
		0, 16, 7
		});
}
