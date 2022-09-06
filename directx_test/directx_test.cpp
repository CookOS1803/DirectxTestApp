#include "NormWin.h"
#include <sstream>
#include "WindowsMessageMap.h"
#include "Window.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsMessageMap mm;
	OutputDebugString(mm(msg, lParam, wParam).c_str());

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		break;
	case WM_KEYDOWN:
		if (wParam == 'F')
		{
			SetWindowText(hwnd, L"ASdas");
		}

		break;
	case WM_KEYUP:
		if (wParam == 'F')
		{
			SetWindowText(hwnd, L"nu window");
		}

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

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window wnd(1600, 900, L"nu window");

	MSG msg;
	BOOL gResult;
	float sine = 0.f;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		// for WM_CHAR messages
		TranslateMessage(&msg);

		DispatchMessage(&msg);

		sine += 0.1f;
		const float c = sin(sine) / 2.f + 0.5f;

		wnd.Gfx().ClearBuffer(c, c, 1.f);
		wnd.Gfx().EndFrame();

	}

	if (gResult == -1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}