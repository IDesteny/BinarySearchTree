#include <Windows.h>
#include <Tchar.h>
#include "../BinarySearchTree/BinarySearchTree.h"

#pragma comment(lib, "../x64/Debug/BinarySearchTree")

LRESULT
WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			break;
		}

		case WM_PAINT:
		{
			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(EXIT_SUCCESS);
			break;
		}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return EXIT_SUCCESS;
}

INT
_tWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ INT nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = _T("MainWindow");

	if (!RegisterClass(&wc))
		return EXIT_FAILURE;

	HWND hWnd = CreateWindow(
		wc.lpszClassName,
		NULL, WS_OVERLAPPEDWINDOW,
		700, 300, 357, 420,
		NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return EXIT_FAILURE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (INT)msg.wParam;
}