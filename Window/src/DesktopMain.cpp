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

const int WIDTH = 640, HEIGHT = 480;

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

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
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

	// _open_osfhandle();

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
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
		//Paint(hdc, color);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONUP:
		LoadFile("../input/piramide.byu", ReceiveInputData);
		color = (color == 0x0000FF) ? 0x000000 : color + 0x000011;
		//InvalidateRect(hWnd, NULL, FALSE);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

static void Paint(HDC hdc, COLORREF color) {
	int x_start = 0, x_end = WIDTH, y_start = 0, y_end = HEIGHT;

	// loads screen buffer to prevent an absurd amount of
	// requests to gpu
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP memBM = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
	SelectObject(memDC, memBM);

	// draw on buffer
	for (int x = x_start; x < x_end; x++) {
		for (int y = y_start; y < y_end; y++) {
			SetPixel(memDC, x, y, color);
		}
		//PrintProgress(x + 1, x_end);
	}

	// load buffer to actual screen
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

static void LoadCamera(Camera camera) {
	camera.V = camera.V - MathLib::project(camera.V, camera.N);
	camera.U = camera.N.cross(camera.V);

	camera.V = camera.V.normalize();
	camera.U = camera.U.normalize();
	camera.N = camera.N.normalize();

	std::cout << camera.str() << std::endl;
}

static void ReceiveInputData(
	int vertex_count,
	MathLib::Vector3D* vertex_list,
	int triangle_count,
	int** triangle_list,
	Camera c
) {
	LoadCamera(c);

}