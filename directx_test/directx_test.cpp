#include "NormWin.h"
#include <sstream>
#include <cmath>
#include "WindowsMessageMap.h"
#include "Window.h"
#include "SceneObject.h"
#include "Scene.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window wnd(1600, 900, L"nu window");
	//wnd.Gfx().SetFullscreenState(true);

	Scene scene(wnd.Gfx());

	Mesh mesh;
	mesh.SetVertices({
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
	mesh.SetIndices({

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

	Mesh mesh2;
	mesh2.SetVertices({
		{ {-1.0f, 1.0f, -1.0f},  {0.0f, 0.0f, 1.0f, 1.0f} },
		{ {1.0f, 1.0f, -1.0f},   {0.0f, 1.0f, 0.0f, 1.0f} },
		{ {1.0f, 1.0f, 1.0f},    {0.0f, 1.0f, 1.0f, 1.0f} },
		{ {-1.0f, 1.0f, 1.0f},   {1.0f, 0.0f, 0.0f, 1.0f} },
		{ {-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },
		{ {1.0f, -1.0f, -1.0f},  {1.0f, 1.0f, 0.0f, 1.0f} },
		{ {1.0f, -1.0f, 1.0f},   {1.0f, 1.0f, 1.0f, 1.0f} },
		{ {-1.0f, -1.0f, 1.0f},  {0.0f, 0.0f, 0.0f, 1.0f} }
		});
	mesh2.SetIndices({
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,

		});
	
	SceneObject o2;
	o2.SetMesh(mesh2);
	scene.AddObject(o2);

	for (size_t i = 0; i < 2; i++)
	{
		SceneObject o;
		o.SetMesh(mesh);

		scene.AddObject(o);
	}

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
			wnd.Gfx()->Render();

			wnd.Gfx()->EndFrame();
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