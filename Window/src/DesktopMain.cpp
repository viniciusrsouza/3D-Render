#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <io.h>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include "ConsoleBinder.h"
#include "FileLoader.h"
#include <Vector3D.h>
#include <Transform.h>
#include "camera/Camera.h"

#define MAX_CONSOLE_LINES 500

const int WIDTH = 600, HEIGHT = 600;

int vertex_count;
int triangle_count;
MathLib::Vector3D* vertex_list;
int** triangle_list;
Camera camera;

int CALLBACK WinMain(
	_In_ HINSTANCE		hInstance,
	_In_opt_ HINSTANCE	hPrevInstance,
	_In_ LPSTR			lpCmdLine,
	_In_ int			nCmdShow
);

LRESULT CALLBACK WndProc(
	_In_ HWND	hWnd,
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

static void Render(HDC, COLORREF);
static void Paint(HDC, COLORREF);
static void PrintProgress(float, float);
static void ReceiveInputData(int, MathLib::Vector3D*, int, int**, Camera);

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("Windows Desktop Guided Tour Application");

HDC hdcMem;
HBITMAP hbmMem, hbmOld;

int CALLBACK WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WIDTH, HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	RedirectIOToConsole();
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

static POINT mp, old_mp;
static COLORREF color = 0x000033;

LRESULT CALLBACK WndProc(HWND hWnd,
						 UINT message,
						 WPARAM wParam,
						 LPARAM lParam
) {
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, Windows desktop!");

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Paint(hdc, 0xffffff);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONUP:
		LoadFile("../input/vaso.byu", ReceiveInputData);
		color = (color == 0x0000FF) ? 0x000000 : color + 0x000011;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

static void Paint(HDC hdc, COLORREF color) {
	std::cout << "Drawing started" << std::endl;
	int x_start = 0, x_end = WIDTH, y_start = 0, y_end = HEIGHT;

	// carrega buffer de tela para evitar uma quantidade absurda
	// de requisições à GPU
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP memBM = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
	SelectObject(memDC, memBM);

	Render(memDC, color);

	// passa o buffer para a tela
	BitBlt(hdc, 0, 0, WIDTH, HEIGHT, memDC, 0, 0, SRCCOPY);
	std::cout << "Drawing complete" << std::endl;
}

static void PrintProgress(float progress, float total) {
	float p = progress / total;
	int barWidth = 70;

	std::cout << "[";
	int pos = barWidth * p;
	for (int i = 0; i < barWidth; ++i) {
		if (i < pos) std::cout << "=";
		else if (i == pos) std::cout << ">";
		else std::cout << " ";
	}
	std::cout << "] " << int(p * 100.0) << " %\r";
	std::cout.flush();

	if (progress == 1.0) std::cout << std::endl;
}

static Camera LoadCamera(Camera camera) {
	camera.V = camera.V - MathLib::project(camera.V, camera.N);
	camera.U = camera.N.cross(camera.V);

	camera.V = camera.V.normalize();
	camera.U = camera.U.normalize();
	camera.N = camera.N.normalize();

	std::cout << camera.str() << std::endl;

	return camera;
}

static bool isBetween(MathLib::Vector3D a, MathLib::Vector3D b, MathLib::Vector3D c) {
	return a.y >= c.y >= b.y || b.y >= c.y >= a.y;
}

static void PaintUpperTriangle(HDC hdc, COLORREF color, MathLib::Vector3D a, MathLib::Vector3D b, MathLib::Vector3D c) {
	float a_min, a_max;
	if (b.x < c.x) {
		a_min = (b.x - a.x) / (b.y - a.y);
		a_max = (c.x - a.x) / (c.y - a.y);
	}
	else {
		a_min = (c.x - a.x) / (c.y - a.y);
		a_max = (b.x - a.x) / (b.y - a.y);
	}

	int x_min = a.x, x_max = a.x;
	

	for (int y = a.y; y < b.y; y++) {
		for (int i = x_min; i <= x_max; i++) {
			SetPixel(hdc, i, y, color);
		}
		x_min += a_min;
		x_max += a_max;
	}
}

static void PaintBottomTriangle(HDC hdc, COLORREF color, MathLib::Vector3D a, MathLib::Vector3D b, MathLib::Vector3D c) {
	float a_min, a_max;

	if (b.x < c.x) {
		a_max = (c.x - a.x) / (c.y - a.y);
		a_min = (c.x - b.x) / (c.y - b.y);
	}
	else {
		a_max = (c.x - b.x) / (c.y - b.y);
		a_min= (c.x - a.x) / (c.y - a.y);
	}

	int x_min = c.x, x_max = c.x;

	for (int y = c.y; y >= b.y; y--) {
		for (int i = x_min; i <= x_max; i++) {
			SetPixel(hdc, i, y, color);
		}
		x_min -= a_min;
		x_max -= a_max;
	}
}

void swap(int* arr, int i, int j) {
	int aux = arr[i];
	arr[i] = arr[j];
	arr[j] = aux;
}

void sort(MathLib::Vector3D* vertex, int i, int j, int k, int* triangle) {
	if (vertex[triangle[0]].y > vertex[triangle[1]].y) {
		swap(triangle, 0, 1);
	}
	if (vertex[triangle[1]].y > vertex[triangle[2]].y) {
		swap(triangle, 1, 2);
	}
	if (vertex[triangle[0]].y > vertex[triangle[1]].y) {
		swap(triangle, 0, 1);
	}
}

static void PaintTriangle(HDC hdc, COLORREF color, int* index) {
	sort(vertex_list, index[0], index[1], index[2], index);

	MathLib::Vector3D a = vertex_list[index[0]];
	MathLib::Vector3D b = vertex_list[index[1]];
	MathLib::Vector3D c = vertex_list[index[2]];

	if (b.y == c.y) {
		PaintUpperTriangle(hdc, color, a, b, c);
	}
	else if (a.y == b.y) {
		PaintBottomTriangle(hdc, color, a, b, c);
	}
	else {
		PaintUpperTriangle(hdc, color, a, b, c);
		PaintBottomTriangle(hdc, color, a, b, c);
	}
}

static void Render(HDC hdc, COLORREF color) {
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			SetPixel(hdc, i, j, 0x000000);
		}
	}

	for (int i = 0; i < triangle_count; i++) {
		PaintTriangle(hdc, color, triangle_list[i]);
	}
}

static void ReceiveInputData(
	int _vc,
	MathLib::Vector3D* _vl,
	int _tc,
	int** _tl,
	Camera _c
) {
	vertex_count = _vc;
	vertex_list = _vl;
	triangle_count = _tc;
	triangle_list = _tl;
	camera = _c;

	camera = LoadCamera(camera);
	float** camera_matrix = camera.getChangeBasisMatrix();
	float** cm = camera_matrix;

	// mudar vetores para coordenadas de tela.
	for (int i = 0; i < vertex_count; i++) {
		MathLib::Vector3D v = (vertex_list[i] - camera.C).multiply_matrix(camera_matrix);
		v.x = camera.d * (v.x / v.z);
		v.y = camera.d * (v.y / v.z);

		v.x = v.x / camera.hx;
		v.y = v.y / camera.hy;

		v.x = (((v.x + 1)/2) * WIDTH) + .5;
		v.y = (HEIGHT - ((v.y + 1) / 2) * HEIGHT) + .5;

		vertex_list[i] = v;
	}
}