#include "NormWin.h"
#include <sstream>
#include <cmath>
#include "WindowsMessageMap.h"
#include "Window.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window wnd(1920, 1080, L"nu window");
	wnd.Gfx().SetFullscreenState(true);

	MSG msg{};
	BOOL gResult;
	float sine = 0.f;
	while (msg.message != WM_QUIT)
	{
		if (gResult = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// for WM_CHAR messages
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
		else
		{
			sine += 0.1f;
			const float r = sin(sine) / 2.f + 0.5f;
			const float g = sin(sqrt(sine)) / 2.f + 0.5f;
			const float b = sin(log2(sine)) / 2.f + 0.5f;

			wnd.Gfx().ClearBuffer(r, g, b);
			wnd.Gfx().EndFrame();
		}
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