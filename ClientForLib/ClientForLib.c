#include <windows.h>
#include <tchar.h>
#include "../BinarySearchTree/BinarySearchTree.h"

#pragma comment(lib, "../x64/Debug/BinarySearchTree")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define BTN_0 1000
#define BTN_1 1001
#define BTN_2 1002
#define BTN_3 1003
#define BTN_4 1004
#define BTN_5 1005
#define BTN_6 1006
#define BTN_7 1007

#define USER_BUFF 8

#define PATH_PY \
	_T("C:\\Users\\Ilya\\AppData\\Local\\Programs\\Python\\Python310\\Python.exe \
		C:\\Users\\Ilya\\source\\repos\\BinarySearchTree\\ClientForLib\\StartChart.py")

#define OpenChartSort() \
	_tsystem(PATH_PY _T(" sort.log")) == -1


#define OpenChartSearch() \
	_tsystem(PATH_PY _T(" search.log")) == -1

TCHAR binTreeData[32];

BOOL
BinarySearch(
	PINT arr,
	INT size,
	INT val)
{
	INT r = size - 1, l = 0, m;

	while (l <= r)
	{
		if (val == arr[m = (r + l) >> 1])
			return TRUE;

		if (arr[m] > val)
			r = m - 1;
		else
			l = m + 1;
	}

	return FALSE;
}

VOID
BubbleSort(
	PINT arr,
	INT size)
{
	for (INT i = 0, tmp, cmp; i < size - 1; ++i)
	{
		cmp = FALSE;
		for (INT j = 0; j < size - i - 1; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
				cmp = TRUE;
			}
		}

		if (!cmp) break;
	}
}

BOOL
OutputFile(
	PCTSTR filename,
	PLONGLONG ts,
	INT sz)
{
	HANDLE hFileSearch = CreateFile(
		filename,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (!hFileSearch)
		return EXIT_FAILURE;

	INT n = 0;
	OVERLAPPED olf = { 0 };

	for (INT i = 0; i < sz; ++i)
	{
		TCHAR buff[32];
		_stprintf_s(buff, ARRAYSIZE(buff), _T("%s\t{ \"x\" : %i, \"y\" : %i }%s"), i ? _T("") : _T("[\n"), i, ts[i], i == sz - 1 ? _T("\n]") : _T(",\n"));

		BOOL r = WriteFile(
			hFileSearch,
			buff,
			lstrlen(buff) * sizeof(TCHAR),
			&n,
			&olf);

		olf.Offset += n;

		if (!r)
			return EXIT_FAILURE;
	}

	CloseHandle(hFileSearch);
	return EXIT_SUCCESS;
}

VOID
Bypass(
	INT data)
{
	_stprintf_s(binTreeData + lstrlen(binTreeData), ARRAYSIZE(binTreeData) - lstrlen(binTreeData), _T("%i "), data);
}

LONGLONG
GetTicks(
	VOID)
{
	LARGE_INTEGER ticks;
	QueryPerformanceCounter(&ticks);
	return ticks.QuadPart;
}

LRESULT
WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	static BINARY_TREE binTree;
	static HWND edits[10];
	static INT mode = 0;

	switch (message)
	{
		case WM_CREATE:
		{
			binTree = BTCreateBinTree();

			edits[0] = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_NUMBER, 10, 10, 70, 30, hWnd, NULL, NULL, NULL);
			edits[1] = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_NUMBER, 10, 50, 70, 30, hWnd, NULL, NULL, NULL);
			edits[2] = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_NUMBER, 10, 90, 70, 30, hWnd, NULL, NULL, NULL);
			edits[3] = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_NUMBER, 110, 90, 70, 30, hWnd, NULL, NULL, NULL);
			edits[4] = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_NUMBER, 10, 130, 70, 30, hWnd, NULL, NULL, NULL);
			edits[5] = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_READONLY, 10, 210, 280, 30, hWnd, NULL, NULL, NULL);
			edits[6] = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_NUMBER, 10, 250, 70, 30, hWnd, NULL, NULL, NULL);
			edits[7] = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_READONLY, 90, 290, 95, 30, hWnd, NULL, NULL, NULL);
			edits[8] = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_READONLY, 90, 330, 95, 30, hWnd, NULL, NULL, NULL);
			edits[9] = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_READONLY, 10, 410, 280, 20, hWnd, NULL, NULL, NULL);

			CreateWindow(_T("button"), _T("Insert"), WS_CHILD | WS_VISIBLE, 90, 10, 200, 30, hWnd, BTN_0, NULL, NULL);
			CreateWindow(_T("button"), _T("Delete"), WS_CHILD | WS_VISIBLE, 90, 50, 200, 30, hWnd, BTN_1, NULL, NULL);
			CreateWindow(_T("button"), _T("Replace"), WS_CHILD | WS_VISIBLE, 190, 90, 100, 30, hWnd, BTN_2, NULL, NULL);
			CreateWindow(_T("button"), _T("Find"), WS_CHILD | WS_VISIBLE, 90, 130, 200, 30, hWnd, BTN_3, NULL, NULL);
			CreateWindow(_T("button"), _T("Bypass"), WS_CHILD | WS_VISIBLE, 10, 170, 280, 30, hWnd, BTN_4, NULL, NULL);
			CreateWindow(_T("button"), _T("Search"), WS_CHILD | WS_VISIBLE, 90, 250, 95, 30, hWnd, BTN_5, NULL, NULL);
			CreateWindow(_T("button"), _T("Sorting"), WS_CHILD | WS_VISIBLE, 195, 250, 95, 30, hWnd, BTN_6, NULL, NULL);
			CreateWindow(_T("button"), _T("Open chart"), WS_CHILD | WS_VISIBLE, 10, 370, 280, 30, hWnd, BTN_7, NULL, NULL);
			break;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			PCTSTR txts[] = {
				_T("to"),
				_T("Binary tree"),
				_T("Array"),
				_T("Nanosecond(s)")
			};

			TextOut(hdc, 90, 95, txts[0], lstrlen(txts[0]));
			TextOut(hdc, 10, 295, txts[1], lstrlen(txts[1]));
			TextOut(hdc, 10, 335, txts[2], lstrlen(txts[2]));
			TextOut(hdc, 195, 295, txts[3], lstrlen(txts[3]));
			TextOut(hdc, 195, 335, txts[3], lstrlen(txts[3]));
			break;
		}

		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case BTN_0:
				{
					TCHAR buffer[USER_BUFF];
					if (!GetWindowText(edits[0], buffer, USER_BUFF))
					{
						SetWindowText(edits[9], _T("Empty line"));
						break;
					}

					if (BTInsert(binTree, _tstoi(buffer)))
					{
						SetWindowText(edits[9], _T("System error"));
						break;
					}

					SetWindowText(edits[9], _T("Success"));
					break;
				}

				case BTN_1:
				{
					TCHAR buffer[USER_BUFF];
					if (!GetWindowText(edits[1], buffer, USER_BUFF))
					{
						SetWindowText(edits[9], _T("Empty line"));
						break;
					}

					if (BTDelete(binTree, _tstoi(buffer)))
					{
						SetWindowText(edits[9], _T("System error"));
						break;
					}

					SetWindowText(edits[9], _T("Success"));
					break;
				}

				case BTN_2:
				{
					TCHAR buffer_1[USER_BUFF];
					TCHAR buffer_2[USER_BUFF];
					if (!(GetWindowText(edits[2], buffer_1, USER_BUFF) &&
						GetWindowText(edits[3], buffer_2, USER_BUFF)))
					{
						SetWindowText(edits[9], _T("Empty line"));
						break;
					}

					if (BTReplace(binTree, _tstoi(buffer_1), _tstoi(buffer_2)))
					{
						SetWindowText(edits[9], _T("System error"));
						break;
					}

					SetWindowText(edits[9], _T("Success"));
					break;
				}

				case BTN_3:
				{
					TCHAR buffer[USER_BUFF];
					if (!GetWindowText(edits[4], buffer, USER_BUFF))
					{
						SetWindowText(edits[9], _T("Empty line"));
						break;
					}

					BOOL res;
					BTFind(binTree, _tstoi(buffer), &res);

					SetWindowText(edits[9], res ? _T("Data found") : _T("Data not found"));
					break;
				}

				case BTN_4:
				{
					ZeroMemory(binTreeData, sizeof binTreeData);
					BTBypass(binTree, Bypass);
					SetWindowText(edits[5], binTreeData);
					SetWindowText(edits[9], _T("Success"));
					break;
				}

				case BTN_5:
				{
					TCHAR buffer[USER_BUFF];
					if (!GetWindowText(edits[6], buffer, USER_BUFF))
					{
						SetWindowText(edits[9], _T("Empty line"));
						break;
					}

					INT len = _tstoi(buffer);

					BINARY_TREE binTreeSearch = BTCreateBinTree();
					if (!binTreeSearch)
					{
						SetWindowText(edits[9], _T("System Error"));
						break;
					}

					PINT arr = malloc(len * sizeof(INT));
					if (!arr)
					{
						SetWindowText(edits[9], _T("System Error"));
						BTDeleteBinTree(binTreeSearch);
						break;
					}

					for (INT i = 0; i < len; ++i)
						BTInsert(binTreeSearch, arr[i] = rand());

					INT val = *arr;
					BOOL res;
					LONGLONG t[2];

					t[0] = GetTicks();
					BTFind(binTreeSearch, val, &res);
					t[0] = GetTicks() - t[0];

					t[1] = GetTicks();
					BinarySearch(arr, len, val);
					t[1] = GetTicks() - t[1];

					for (INT i = 0; i < 2; ++i)
					{
						_stprintf_s(buffer, USER_BUFF, _T("%I64d"), t[i]);
						SetWindowText(edits[7 + i], buffer);
					}

					SetWindowText(edits[9], _T("Success"));

					mode = 1;

					BTDeleteBinTree(binTreeSearch);
					free(arr);
					break;
				}

				case BTN_6:
				{
					TCHAR buffer[USER_BUFF];
					if (!GetWindowText(edits[6], buffer, USER_BUFF))
					{
						SetWindowText(edits[9], _T("Empty line"));
						break;
					}

					INT len = _tstoi(buffer);
					BINARY_TREE binTreeSort = BTCreateBinTree();
					PINT arr = malloc(len * sizeof(INT));

					if (!(binTreeSort && arr))
					{
						SetWindowText(edits[9], _T("System Error"));
						break;
					}

					for (INT i = 0; i < len; ++i)
						arr[i] = rand();

					LONGLONG t[2];

					t[0] = GetTicks();
					for (INT i = 0; i < len; ++i)
						BTInsert(binTreeSort, arr[i]);
					t[0] = GetTicks() - t[0];

					t[1] = GetTicks();
					BubbleSort(arr, len);
					t[1] = GetTicks() - t[1];

					for (INT i = 0; i < 2; ++i)
					{
						_stprintf_s(buffer, USER_BUFF, _T("%I64d"), t[i]);
						SetWindowText(edits[7 + i], buffer);
					}

					SetWindowText(edits[9], _T("Success"));

					mode = 2;

					BTDeleteBinTree(binTreeSort);
					free(arr);
					break;
				}

				case BTN_7:
				{
					if (!mode)
					{
						SetWindowText(edits[9], _T("No chart type selected"));
						break;
					}

					TCHAR buffer[USER_BUFF];
					if (!GetWindowText(edits[6], buffer, USER_BUFF))
					{
						SetWindowText(edits[9], _T("Empty line"));
						break;
					}

					INT len = _tstoi(buffer);
					INT step = len / (len / 10);
					BINARY_TREE binTreeChart = BTCreateBinTree();
					PLONGLONG t = malloc(step * sizeof(LONGLONG));

					if (!(binTreeChart && t))
					{
						SetWindowText(edits[9], _T("System Error"));
						break;
					}

					if (mode == 1)
					{
						PINT arr = malloc(len * sizeof(INT));
						if (!arr)
						{
							SetWindowText(edits[9], _T("Chart opening error"));
							free(t);
							break;
						}

						BOOL r;
						INT v;

						for (INT i = 0; i < len; ++i)
						{
							BTInsert(binTreeChart, arr[i] = rand());
						}

						for (INT i = 0; i < step; ++i)
						{
							v = arr[rand() % (step * (i + 1))];

							t[i] = GetTicks();
							BTFind(binTreeChart, v, &r);
							t[i] = GetTicks() - t[i];
						}

						if (OutputFile(_T("search.log"), t, step))
						{
							SetWindowText(edits[9], _T("Error writing to file"));
							free(t);
							free(arr);
							BTDeleteBinTree(binTreeChart);
							break;
						}

						if (OpenChartSearch())
						{
							SetWindowText(edits[9], _T("Chart opening error"));
							free(t);
							free(arr);
							BTDeleteBinTree(binTreeChart);
							break;
						}

						free(arr);
						BTDeleteBinTree(binTreeChart);
					}
					else
					{
						for (INT i = 0; i < step; ++i)
						{
							INT correntLen = step * i;

							t[i] = GetTicks();
							for (INT j = 0; j < correntLen; ++j)
								BTInsert(binTreeChart, rand());
							t[i] = GetTicks() - t[i];

							BTDeleteBinTree(binTreeChart);
						}

						if (OutputFile(_T("sort.log"), t, step))
						{
							SetWindowText(edits[9], _T("Error writing to file"));
							free(t);
							break;
						}

						if (OpenChartSort())
						{
							SetWindowText(edits[9], _T("Chart opening error"));
							free(t);
							break;
						}
					}

					SetWindowText(edits[9], _T("Success"));
					free(t);
					
					break;
				}
			}
			break;
		}

		case WM_DESTROY:
		{
			BTDeleteBinTree(binTree);
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
		700, 300, 315, 480,
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