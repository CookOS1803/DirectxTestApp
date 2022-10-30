#include "NormWin.h"
#include <sstream>
#include <cmath>
#include "mymath.h"
#include "WindowsMessageMap.h"
#include "Window.h"
#include "SceneObject.h"
#include "Scene.h"
#include "CylinderMovement.h"
#include "CubeMovementTop.h"
#include "CubeMovementBottom.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Timer timer;
	float t = 0.f;

	Window wnd(1600, 900, L"nu window");

	//wnd.mouse.EnableRaw();

	Scene scene(wnd.Gfx());

	std::unique_ptr<ID3D11PixelShader, DXDeleter<ID3D11PixelShader>> psLight;
	psLight.reset(wnd.Gfx()->CompileAndCreatePixelShader(L"Light.fx", "PS", "ps_5_0"));

	std::unique_ptr<ID3D11PixelShader, DXDeleter<ID3D11PixelShader>> psSolidColor;
	psSolidColor.reset(wnd.Gfx()->CompileAndCreatePixelShader(L"Light.fx", "PSSolid", "ps_5_0"));

	std::unique_ptr<ID3D11PixelShader, DXDeleter<ID3D11PixelShader>> psTexture;
	psTexture.reset(wnd.Gfx()->CompileAndCreatePixelShader(L"Light.fx", "PSTexture", "ps_5_0"));

	auto loadedMesh = std::make_unique<Mesh>(wnd.Gfx());
	loadedMesh->LoadFromFile(L"Padlock.obj");

	const auto sine = std::sin(DirectX::XM_PIDIV4);

	auto cylinderMesh = std::make_unique<Mesh>(wnd.Gfx());
	cylinderMesh->SetVertices({

		// bottom 0-15

		{{-0.5f,       -1.f, 0.f},         {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {0.f, 0.f}},
		{{-sine / 2.f, -1.f, sine / 2.f},  {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {1.f, 0.f}},
		{{0.f,         -1.f, 0.5f},        {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {0.f, 0.f}},
		{{sine / 2.f,  -1.f, sine / 2.f},  {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {1.f, 0.f}},
		{{0.5f,        -1.f, 0.f},         {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {0.f, 0.f}},
		{{sine / 2.f,  -1.f, -sine / 2.f}, {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {1.f, 0.f}},
		{{0.f,         -1.f, -0.5f},       {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {0.f, 0.f}},
		{{-sine / 2.f, -1.f, -sine / 2.f}, {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {1.f, 0.f}},
		{{-1.f,        -1.f, 0.f},         {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {0.f, 1.f}},
		{{-sine,       -1.f, sine},        {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {1.f, 1.f}},
		{{0.f,         -1.f, 1.f},         {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {0.f, 1.f}},
		{{sine,        -1.f, sine},        {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {1.f, 1.f}},
		{{1.f,         -1.f, 0.f},         {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {0.f, 1.f}},
		{{sine,        -1.f, -sine},       {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {1.f, 1.f}},
		{{0.f,         -1.f, -1.f},        {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {0.f, 1.f}},
		{{-sine,       -1.f, -sine},       {1.f, 1.f, 1.f}, {0.f, -1.f, 0.f}, {1.f, 1.f}},

		// top 16-31

		{{-0.5f,       1.f, 0.f},         {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {0.f, 0.f}},
		{{-sine / 2.f, 1.f, sine / 2.f},  {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f}},
		{{0.f,         1.f, 0.5f},        {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {0.f, 0.f}},
		{{sine / 2.f,  1.f, sine / 2.f},  {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f}},
		{{0.5f,        1.f, 0.f},         {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {0.f, 0.f}},
		{{sine / 2.f,  1.f, -sine / 2.f}, {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f}},
		{{0.f,         1.f, -0.5f},       {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {0.f, 0.f}},
		{{-sine / 2.f, 1.f, -sine / 2.f}, {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f}},
		{{-1.f,        1.f, 0.f},         {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {0.f, 1.f}},
		{{-sine,       1.f, sine},        {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 1.f}},
		{{0.f,         1.f, 1.f},         {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {0.f, 1.f}},
		{{sine,        1.f, sine},        {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 1.f}},
		{{1.f,         1.f, 0.f},         {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {0.f, 1.f}},
		{{sine,        1.f, -sine},       {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 1.f}},
		{{0.f,         1.f, -1.f},        {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {0.f, 1.f}},
		{{-sine,       1.f, -sine},       {1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 1.f}},

		// inner 32-63

		{{-0.5f,       -1.f, 0.f},         {1.f, 1.f, 1.f}, {cosd(-157.5f), 0.f, sind(-157.5f)}, {0.f, 0.f}   },
		{{-sine / 2.f, -1.f, sine / 2.f},  {1.f, 1.f, 1.f}, {cosd(-157.5f), 0.f, sind(-157.5f)}, {0.125f, 0.f}},
		{{-0.5f,       1.f, 0.f},          {1.f, 1.f, 1.f}, {cosd(-157.5f), 0.f, sind(-157.5f)}, {0.f, 1.f}   },
		{{-sine / 2.f, 1.f, sine / 2.f},   {1.f, 1.f, 1.f}, {cosd(-157.5f), 0.f, sind(-157.5f)}, {0.125f, 1.f}},
		{{-sine / 2.f, -1.f, sine / 2.f},  {1.f, 1.f, 1.f}, {cosd(-112.5f), 0.f, sind(-112.5f)}, {0.125f, 0.f}},
		{{0.f,         -1.f, 0.5f},        {1.f, 1.f, 1.f}, {cosd(-112.5f), 0.f, sind(-112.5f)}, {0.25f, 0.f} },
		{{-sine / 2.f, 1.f, sine / 2.f},   {1.f, 1.f, 1.f}, {cosd(-112.5f), 0.f, sind(-112.5f)}, {0.125f, 1.f}},
		{{0.f,         1.f, 0.5f},         {1.f, 1.f, 1.f}, {cosd(-112.5f), 0.f, sind(-112.5f)}, {0.25f, 1.f} },
		{{0.f,         -1.f, 0.5f},        {1.f, 1.f, 1.f}, {cosd(-67.5f), 0.f, sind(-67.5f)},   {0.25f, 0.f} },
		{{sine / 2.f,  -1.f, sine / 2.f},  {1.f, 1.f, 1.f}, {cosd(-67.5f), 0.f, sind(-67.5f)},   {0.375f, 0.f}},
		{{0.f,         1.f, 0.5f},         {1.f, 1.f, 1.f}, {cosd(-67.5f), 0.f, sind(-67.5f)},   {0.25f, 1.f} },
		{{sine / 2.f,  1.f, sine / 2.f},   {1.f, 1.f, 1.f}, {cosd(-67.5f), 0.f, sind(-67.5f)},   {0.375f, 1.f}},
		{{sine / 2.f,  -1.f, sine / 2.f},  {1.f, 1.f, 1.f}, {cosd(-22.5f), 0.f, sind(-22.5f)},   {0.375f, 0.f}},
		{{0.5f,        -1.f, 0.f},         {1.f, 1.f, 1.f}, {cosd(-22.5f), 0.f, sind(-22.5f)},   {0.5f, 0.f}  },
		{{sine / 2.f,  1.f, sine / 2.f},   {1.f, 1.f, 1.f}, {cosd(-22.5f), 0.f, sind(-22.5f)},   {0.375f, 1.f}},
		{{0.5f,        1.f, 0.f},          {1.f, 1.f, 1.f}, {cosd(-22.5f), 0.f, sind(-22.5f)},   {0.5f, 1.f}  },
		{{0.5f,        -1.f, 0.f},         {1.f, 1.f, 1.f}, {cosd(22.5f), 0.f, sind(22.5f)},     {0.5f, 0.f}  },
		{{sine / 2.f,  -1.f, -sine / 2.f}, {1.f, 1.f, 1.f}, {cosd(22.5f), 0.f, sind(22.5f)},     {0.625f, 0.f}},
		{{0.5f,        1.f, 0.f},          {1.f, 1.f, 1.f}, {cosd(22.5f), 0.f, sind(22.5f)},     {0.5f, 1.f}  },
		{{sine / 2.f,  1.f, -sine / 2.f},  {1.f, 1.f, 1.f}, {cosd(22.5f), 0.f, sind(22.5f)},     {0.625f, 1.f}},
		{{sine / 2.f,  -1.f, -sine / 2.f}, {1.f, 1.f, 1.f}, {cosd(67.5f), 0.f, sind(67.5f)},     {0.625f, 0.f}},
		{{0.f,         -1.f, -0.5f},       {1.f, 1.f, 1.f}, {cosd(67.5f), 0.f, sind(67.5f)},     {0.75f, 0.f} },
		{{sine / 2.f,  1.f, -sine / 2.f},  {1.f, 1.f, 1.f}, {cosd(67.5f), 0.f, sind(67.5f)},     {0.625f, 1.f}},
		{{0.f,         1.f, -0.5f},        {1.f, 1.f, 1.f}, {cosd(67.5f), 0.f, sind(67.5f)},     {0.75f, 1.f} },
		{{0.f,         -1.f, -0.5f},       {1.f, 1.f, 1.f}, {cosd(112.5f), 0.f, sind(112.5f)},   {0.75f, 0.f} },
		{{-sine / 2.f, -1.f, -sine / 2.f}, {1.f, 1.f, 1.f}, {cosd(112.5f), 0.f, sind(112.5f)},   {0.875f, 0.f}},
		{{0.f,         1.f, -0.5f},        {1.f, 1.f, 1.f}, {cosd(112.5f), 0.f, sind(112.5f)},   {0.75f, 1.f} },
		{{-sine / 2.f, 1.f, -sine / 2.f},  {1.f, 1.f, 1.f}, {cosd(112.5f), 0.f, sind(112.5f)},   {0.875f, 1.f}},
		{{-sine / 2.f, -1.f, -sine / 2.f}, {1.f, 1.f, 1.f}, {cosd(157.5f), 0.f, sind(157.5f)},   {0.875f, 0.f}},
		{{-0.5f,         -1.f, 0.f},       {1.f, 1.f, 1.f}, {cosd(157.5f), 0.f, sind(157.5f)},   {1.f, 0.f}   },
		{{-sine / 2.f, 1.f, -sine / 2.f},  {1.f, 1.f, 1.f}, {cosd(157.5f), 0.f, sind(157.5f)},   {0.875f, 1.f}},
		{{-0.5f,         1.f, 0.f},        {1.f, 1.f, 1.f}, {cosd(157.5f), 0.f, sind(157.5f)},   {1.f, 1.f}   },

		// outer 64-95

		{{-1.f,  -1.f, 0.f},   {1.f, 1.f, 1.f}, {cosd(157.5f), 0.f, sind(157.5f)},   {0.f, 0.f}   },
		{{-sine, -1.f, sine},  {1.f, 1.f, 1.f}, {cosd(157.5f), 0.f, sind(157.5f)},   {0.125f, 0.f}},
		{{-1.f,  1.f, 0.f},    {1.f, 1.f, 1.f}, {cosd(157.5f), 0.f, sind(157.5f)},   {0.f, 1.f}   },
		{{-sine, 1.f, sine},   {1.f, 1.f, 1.f}, {cosd(157.5f), 0.f, sind(157.5f)},   {0.125f, 1.f}},
		{{-sine, -1.f, sine},  {1.f, 1.f, 1.f}, {cosd(112.5f), 0.f, sind(112.5f)},   {0.125f, 0.f}},
		{{0.f,   -1.f, 1.f},   {1.f, 1.f, 1.f}, {cosd(112.5f), 0.f, sind(112.5f)},   {0.25f, 0.f} },
		{{-sine, 1.f, sine},   {1.f, 1.f, 1.f}, {cosd(112.5f), 0.f, sind(112.5f)},   {0.125f, 1.f}},
		{{0.f,   1.f, 1.f},    {1.f, 1.f, 1.f}, {cosd(112.5f), 0.f, sind(112.5f)},   {0.25f, 1.f} },
		{{0.f,   -1.f, 1.f},   {1.f, 1.f, 1.f}, {cosd(67.5f), 0.f, sind(67.5f)},     {0.25f, 0.f} },
		{{sine,  -1.f, sine},  {1.f, 1.f, 1.f}, {cosd(67.5f), 0.f, sind(67.5f)},     {0.375f, 0.f}},
		{{0.f,   1.f, 1.f},    {1.f, 1.f, 1.f}, {cosd(67.5f), 0.f, sind(67.5f)},     {0.25f, 1.f} },
		{{sine,  1.f, sine},   {1.f, 1.f, 1.f}, {cosd(67.5f), 0.f, sind(67.5f)},     {0.375f, 1.f}},
		{{sine,  -1.f, sine},  {1.f, 1.f, 1.f}, {cosd(22.5f), 0.f, sind(22.5f)},     {0.375f, 0.f}},
		{{1.f,   -1.f, 0.f},   {1.f, 1.f, 1.f}, {cosd(22.5f), 0.f, sind(22.5f)},     {0.5f, 0.f}  },
		{{sine,  1.f, sine},   {1.f, 1.f, 1.f}, {cosd(22.5f), 0.f, sind(22.5f)},     {0.375f, 1.f}},
		{{1.f,   1.f, 0.f},    {1.f, 1.f, 1.f}, {cosd(22.5f), 0.f, sind(22.5f)},     {0.5f, 1.f}  },
		{{1.f,   -1.f, 0.f},   {1.f, 1.f, 1.f}, {cosd(-22.5f), 0.f, sind(-22.5f)},   {0.5f, 0.f}  },
		{{sine,  -1.f, -sine}, {1.f, 1.f, 1.f}, {cosd(-22.5f), 0.f, sind(-22.5f)},   {0.625f, 0.f}},
		{{1.f,   1.f, 0.f},    {1.f, 1.f, 1.f}, {cosd(-22.5f), 0.f, sind(-22.5f)},   {0.5f, 1.f}  },
		{{sine,  1.f, -sine},  {1.f, 1.f, 1.f}, {cosd(-22.5f), 0.f, sind(-22.5f)},   {0.625f, 1.f}},
		{{sine,  -1.f, -sine}, {1.f, 1.f, 1.f}, {cosd(-67.5f), 0.f, sind(-67.5f)},   {0.625f, 0.f}},
		{{0.f,   -1.f, -1.f},  {1.f, 1.f, 1.f}, {cosd(-67.5f), 0.f, sind(-67.5f)},   {0.75f, 0.f} },
		{{sine,  1.f, -sine},  {1.f, 1.f, 1.f}, {cosd(-67.5f), 0.f, sind(-67.5f)},   {0.625f, 1.f}},
		{{0.f,   1.f, -1.f},   {1.f, 1.f, 1.f}, {cosd(-67.5f), 0.f, sind(-67.5f)},   {0.75f, 1.f} },
		{{0.f,   -1.f, -1.f},  {1.f, 1.f, 1.f}, {cosd(-112.5f), 0.f, sind(-112.5f)}, {0.75f, 0.f} },
		{{-sine, -1.f, -sine}, {1.f, 1.f, 1.f}, {cosd(-112.5f), 0.f, sind(-112.5f)}, {0.875f, 0.f}},
		{{0.f,   1.f, -1.f},   {1.f, 1.f, 1.f}, {cosd(-112.5f), 0.f, sind(-112.5f)}, {0.75f, 1.f} },
		{{-sine, 1.f, -sine},  {1.f, 1.f, 1.f}, {cosd(-112.5f), 0.f, sind(-112.5f)}, {0.875f, 1.f}},
		{{-sine, -1.f, -sine}, {1.f, 1.f, 1.f}, {cosd(-157.5f), 0.f, sind(-157.5f)}, {0.875f, 0.f}},
		{{-1.f,   -1.f, 0.f},  {1.f, 1.f, 1.f}, {cosd(-157.5f), 0.f, sind(-157.5f)}, {1.f, 0.f}   },
		{{-sine, 1.f, -sine},  {1.f, 1.f, 1.f}, {cosd(-157.5f), 0.f, sind(-157.5f)}, {0.875f, 1.f}},
		{{-1.f,   1.f, 0.f},   {1.f, 1.f, 1.f}, {cosd(-157.5f), 0.f, sind(-157.5f)}, {1.f, 1.f}   },

		{ {0.f, 0.f, 0.f},   {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {1.f, 1.f} }

		});
	cylinderMesh->SetIndices({

		// bottom
		 
		0	,	9	,	8	,
		0	,	1	,	9	,
		1	,	10	,	9	,
		1	,	2	,	10	,
		2	,	11	,	10	,
		2	,	3	,	11	,
		3	,	12	,	11	,
		3	,	4	,	12	,
		4	,	13	,	12	,
		4	,	5	,	13	,
		5	,	14	,	13	,
		5	,	6	,	14	,
		6	,	15	,	14	,
		6	,	7	,	15	,
		7	,	8	,	15	,
		7	,	0	,	8	,

		// top

		16	,	24	,	25	,
		16	,	25	,	17	,
		17	,	25	,	26	,
		17	,	26	,	18	,
		18	,	26	,	27	,
		18	,	27	,	19	,
		19	,	27	,	28	,
		19	,	28	,	20	,
		20	,	28	,	29	,
		20	,	29	,	21	,
		21	,	29	,	30	,
		21	,	30	,	22	,
		22	,	30	,	31	,
		22	,	31	,	23	,
		23	,	31	,	24	,
		23	,	24	,	16	,

		// inner

		32	,	34	,	35	,
		32	,	35	,	33	,
		36	,	38	,	39	,
		36	,	39	,	37	,
		40	,	42	,	43	,
		40	,	43	,	41	,
		44	,	46	,	47	,
		44	,	47	,	45	,
		48	,	50	,	51	,
		48	,	51	,	49	,
		52	,	54	,	55	,
		52	,	55	,	53	,
		56	,	58	,	59	,
		56	,	59	,	57	,
		60	,	62	,	63	,
		60	,	63	,	61	,

		// outer

		64	,	67	,	66	,
		64	,	65	,	67	,
		68	,	71	,	70	,
		68	,	69	,	71	,
		72	,	75	,	74	,
		72	,	73	,	75	,
		76	,	79	,	78	,
		76	,	77	,	79	,
		80	,	83	,	82	,
		80	,	81	,	83	,
		84	,	87	,	86	,
		84	,	85	,	87	,
		88	,	91	,	90	,
		88	,	89	,	91	,
		92	,	95	,	94	,
		92	,	93	,	95	,


		});

	auto cubeMesh = std::make_unique<Mesh>(wnd.Gfx());
	cubeMesh->SetVertices({
		{ {-1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.f, 0.f} },
		{ {1.0f, 1.0f, -1.0f},  {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.f, 0.f} },
		{ {1.0f, 1.0f, 1.0f},   {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.f, 1.f} },
		{ {-1.0f, 1.0f, 1.0f},  {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.f, 1.f} },
		
		{ {-1.0f, -1.0f, -1.0f},{0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.f, 0.f} },
		{ {1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.f, 0.f} },
		{ {1.0f, -1.0f, 1.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.f, 1.f} },
		{ {-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.f, 1.f} },
		
		{ {-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.f, 0.f} },
		{ {-1.0f, -1.0f, -1.0f},{0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.f, 0.f} },
		{ {-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.f, 1.f} },
		{ {-1.0f, 1.0f, 1.0f},  {0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.f, 1.f} },
		
		{ {1.0f, -1.0f, 1.0f},  {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.f, 0.f} },
		{ {1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.f, 0.f} },
		{ {1.0f, 1.0f, -1.0f},  {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.f, 1.f} },
		{ {1.0f, 1.0f, 1.0f},   {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.f, 1.f} },
		
		{ {-1.0f, -1.0f, -1.0f},{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.f, 0.f} },
		{ {1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.f, 0.f} },
		{ {1.0f, 1.0f, -1.0f},  {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.f, 1.f} },
		{ {-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.f, 1.f} },
		
		{ {-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.f, 0.f} },
		{ {1.0f, -1.0f, 1.0f},  {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.f, 0.f} },
		{ {1.0f, 1.0f, 1.0f},   {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.f, 1.f} },
		{ {-1.0f, 1.0f, 1.0f},  {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.f, 1.f} }
		});
	cubeMesh->SetIndices({
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22

		});

	auto sphereMesh = std::make_unique<Mesh>(wnd.Gfx());
	sphereMesh->MakeSphere(3, 3);

	auto obj = scene.CreateObject();
	obj->SetMesh(cylinderMesh.get());
	obj->GetMeshRenderer().SetPixelShader(psTexture.get());
	obj->SetUpdateable<CylinderMovement>();

	obj = scene.CreateObject();
	obj->SetMesh(loadedMesh.get());
	obj->GetMeshRenderer().SetPixelShader(psTexture.get());
	obj->GetTransform().scale = { 30.f, 30.f, 30.f };
	obj->GetTransform().eulerRotation.x = -DirectX::XM_PIDIV2;
	obj->GetTransform().position = {4.f, 4.f, 4.f};

	obj = scene.CreateObject();
	obj->SetMesh(cubeMesh.get());
	obj->GetMeshRenderer().SetPixelShader(psLight.get());
	obj->GetTransform().scale = {0.3f, 0.3f, 0.3f};
	obj->SetUpdateable<CubeMovementTop>();

	obj = scene.CreateObject();
	obj->SetMesh(cubeMesh.get());
	obj->GetMeshRenderer().SetPixelShader(psLight.get());
	obj->GetTransform().scale = { 0.3f, 0.3f, 0.3f };
	obj->SetUpdateable<CubeMovementBottom>();

	obj = scene.CreateObject();
	obj->SetMesh(sphereMesh.get());
	obj->GetMeshRenderer().SetPixelShader(psLight.get());
	obj->GetTransform().position = { -4.f, 4.f, 4.f };

	auto pObject = scene.CreateUIObject();
	pObject->SetMesh(cubeMesh.get());
	pObject->GetMeshRenderer().SetPixelShader(psSolidColor.get());
	pObject->GetTransform().position = {5.f, -2.5f, 0.f};

	bool ttt = false;

	MSG msg{};
	BOOL gResult;
	float stepLeft = 0.f, stepRight = 0.f, stepForward = 0.f, stepBack = 0.f;
	float rotateLeft = 0.f, rotateRight = 0.f, rotateDown = 0.f, rotateUp = 0.f;
	const float step = 0.1f;
	const float astep = 0.03f;
	while (msg.message != WM_QUIT)
	{
		if (gResult = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			auto key = wnd.kbd.ReadKey();

			if (key.has_value())
			{
				if (key.value().IsPress())
				{

					switch (key.value().GetCode())
					{
					case VK_ESCAPE:
						wnd.EnableCursor();
						break;
					case 'a': case 'A':
						stepLeft = -step;
						break;
					case 'd': case 'D':
						stepRight = step;
						break;
					case 's': case 'S':
						stepBack = -step;
						break;
					case 'w': case 'W':
						stepForward = step;
						break;
					case VK_DOWN:
						rotateDown = astep;
						break;
					case VK_UP:
						rotateUp = -astep;
						break;
					case VK_LEFT:
						rotateLeft = -astep;
						break;
					case VK_RIGHT:
						rotateRight = astep;
						break;
					case VK_SPACE:
						ttt = ttt ? false : true;
						break;
					default:
						break;
					}
				}
				else
				{
					switch (key.value().GetCode())
					{
					case 'a': case 'A':
						stepLeft = 0.f;
						break;
					case 'd': case 'D':
						stepRight = 0.f;
						break;
					case 's': case 'S':
						stepBack = 0.f;
						break;
					case 'w': case 'W':
						stepForward = 0.f;
						break;
					case VK_DOWN:
						rotateDown = 0.f;
						break;
					case VK_UP:
						rotateUp = 0.f;
						break;
					case VK_LEFT:
						rotateLeft = 0.f;
						break;
					case VK_RIGHT:
						rotateRight = 0.f;
						break;
					default:
						break;
					}
				}
			}

			auto mouseEvent = wnd.mouse.Read();

			if (mouseEvent.has_value())
			{
				using Event = Mouse::Event::Type;

				switch (mouseEvent.value().GetType())
				{
				case Event::LPress:
					wnd.DisableCursor();
					break;
				case Event::Move:
				{
					std::wstringstream da;

					da << mouseEvent.value().GetPosX() << " " << mouseEvent.value().GetPosY() << '\0';
					wnd.SetTitle(da.view());

					break;
				}
				default:
					break;
				}
			}
		}
		else
		{
			float delta = timer.Mark() * 2;

			wnd.Gfx()->GetCamera().Rotate(rotateDown + rotateUp, rotateLeft + rotateRight, 0.f);
			wnd.Gfx()->GetCamera().Translate(stepLeft + stepRight, 0, stepBack + stepForward);

			wnd.Gfx()->Render(t);

			for (const auto& o : scene.Objects())
			{
				if (o->GetUpdateable() != nullptr)
					o->GetUpdateable()->Update(delta);

				wnd.Gfx()->Draw(*o);
			}

			for (const auto& o : scene.UIObjects())
			{
				if (o->GetUpdateable() != nullptr)
					o->GetUpdateable()->Update(delta);

				wnd.Gfx()->DrawUI(*o);
			}

			if (ttt)
				wnd.Gfx()->DrawText();

			wnd.Gfx()->EndFrame();

			t += delta;
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