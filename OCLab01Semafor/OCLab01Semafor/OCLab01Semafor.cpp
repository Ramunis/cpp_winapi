// OCLab01Semafor.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "OCLab01Semafor.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <filesystem>
#include <windows.h>
#include <process.h>

namespace fs = std::experimental::filesystem;

HANDLE m_hSemaphore = CreateSemaphore(NULL, 0, 1000, NULL);

void reverseFile(fs::path pfile, fs::path const& pdir)
{
	std::fstream ifs(pfile, std::ios::in), ofs(pdir / pfile.filename(), std::ios::out);
	if (ifs.is_open() && ofs.is_open())
	{
		std::vector<std::string>vct;
		std::string str;
		while (std::getline(ifs, str))
		{
			vct.emplace_back(std::move(str));
		}
		std::reverse_copy(vct.begin(), vct.end(), std::ostream_iterator<std::string>(ofs, "\n"));

		ifs.close();
		ofs.close();
	}
	else
	{
		ReleaseSemaphore(m_hSemaphore, 2, 0);
		std::cerr << "Unable to open file(s): " << pfile << "  " << pdir / pfile.filename() << "\n\n";
	}
}




#define MAX_LOADSTRING 100

// Глобальные переменные:
HWND hwndButton;
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    BUTTON(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MainWinProc(HWND, UINT, WPARAM, LPARAM);
#define ID_MYBUTTON 1    /* идентификатор для кнопочки внутри главного окна */

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OCLAB01SEMAFOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OCLAB01SEMAFOR));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OCLAB01SEMAFOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OCLAB01SEMAFOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   HWND hWndEditin = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT("c:\\dir1"), WS_CHILD | WS_VISIBLE, 150, 20, 140, 20, hWnd, (HMENU)1, NULL, NULL);
   HWND hWndEditout = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT("c:\\dir2"), WS_CHILD | WS_VISIBLE, 150, 60, 140, 20, hWnd, (HMENU)2, NULL, NULL);
   HWND lblObj = CreateWindow(TEXT("static"), TEXT("Input directory: "), WS_VISIBLE | WS_CHILD, 5, 20, 110, 20, hWnd, (HMENU)3, NULL, NULL);
   HWND lblObji = CreateWindow(TEXT("static"), TEXT("Output directory: "), WS_VISIBLE | WS_CHILD, 5, 60, 110, 20, hWnd, (HMENU)4, NULL, NULL);
   HWND lblObjio = CreateWindow(TEXT("static"), TEXT("Инвертирование директории с файлами:"), WS_VISIBLE | WS_CHILD, 5, 1, 300, 20, hWnd, (HMENU)4, NULL, NULL);

    hwndButton = CreateWindow(
	   L"BUTTON",  // Predefined class; Unicode assumed 
	   L"Invert",      // Button text 
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
	   150,         // x position 
	   100,         // y position 
	   140,        // Button width
	   20,        // Button height
	   hWnd,     // Parent window
		(HMENU)ID_MYBUTTON,       // No menu.
	   (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
	   NULL);      // Pointer not needed.
   
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//

int DisplayResourceNAMessageBox()
{
	int msgboxID = MessageBox(
		NULL,
		(LPCWSTR)L"Resource not available\nDo you want to try again?",
		(LPCWSTR)L"Account Details",
		MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
	);

	switch (msgboxID)
	{
	case IDCANCEL:
		// TODO: add code
		break;
	case IDTRYAGAIN:
		// TODO: add code
		break;
	case IDCONTINUE:
		// TODO: add code
		break;
	}

	return msgboxID;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		
		return 0;
    case WM_COMMAND:
        {
		
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
			case ID_MYBUTTON:
			{
				int len = GetWindowTextLengthW(GetDlgItem(hWnd, 1)) + 1;
				int len2 = GetWindowTextLengthW(GetDlgItem(hWnd, 2)) + 1;
				char in[20];
				char out[20];			
				GetWindowTextA(GetDlgItem(hWnd, 1),in,len);
				GetWindowTextA(GetDlgItem(hWnd, 2), out, len2);
				
				try
				{
					fs::path pd1(in);
					fs::path pd2(out);
					if (!fs::exists(pd2))
					{
						fs::create_directory(pd2);
					}
					std::vector<std::thread> vt;
					for (fs::directory_iterator ib(pd1), ie; ib != ie; ++ib)
					{
						if (fs::is_regular_file(ib->path()) && ib->path().extension() == ".txt")
						{
							vt.emplace_back(reverseFile, ib->path(), std::cref(pd2));
						}
					}
					for (auto& thr : vt)
					{
						thr.join();
					}
				}
				catch (std::exception const& exc)
				{				
					std::cerr << "Exception: " << exc.what() << std::endl;
				}
				//std::cout << "\nFiles were inverted successfully";
				if ((HIWORD(wParam) == 0) && (LOWORD(wParam) == ID_MYBUTTON))
				{
					MessageBox(hWnd, L"Files were inverted successfully", L"Message Box", MB_OK | MB_ICONWARNING);
				}
			}
				
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;				
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
	
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

BOOL CALLBACK myProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCLOSE) // close button click
			EndDialog(hwnd, 0);
		return TRUE;
	}
	return FALSE;
}




