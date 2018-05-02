// Stratego.cpp : definiuje punkt wejścia dla aplikacji
//

#include "stdafx.h"
#include "Stratego.h"
#include <windowsx.h>

#define MAX_LOADSTRING 100

// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego
int boardSize = 7;								//rozmiar planszy
const int CELL_SIZE = 50;						//wielkość pola
HBRUSH red, green, yellow, blue;					//kolory do gry
int playerTurn = 1;
int gameBoard[7][7];
bool firstTime = true;



// Przekaż dalej deklaracje funkcji dołączonych w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool createGameBoard(HWND hWnd, RECT *pRect) {
	RECT rc;

	if (firstTime == true) {
		for (int i = 0; i < boardSize; i++) {
			for (int j = 0; j < boardSize; j++) {
				gameBoard[i][j] = 0;
			}
		}
		firstTime = false;
	}

	if (GetClientRect(hWnd, &rc)) {
		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;

		pRect->left = (width - CELL_SIZE * boardSize) / 2;
		pRect->top = (height - CELL_SIZE * boardSize) / 2;
		pRect->right = pRect->left + CELL_SIZE * boardSize;
		pRect->bottom = pRect->top + CELL_SIZE * boardSize;
		return true;
	}
	SetRectEmpty(pRect);
	return false;
}

void drawLine(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}


int getRowNumber(HWND hWnd, int  x, int y) {
	RECT rc;
	POINT pt = { x,y };
	if (createGameBoard(hWnd, &rc)) {
		if (PtInRect(&rc, pt)) {
			//user clicked inside the game board
			y = pt.y - rc.top;
			int row = y / CELL_SIZE;
			return row;
		}
	}
	return -1; //outside the game board
}

int getColumnNumber (HWND hWnd, int  x, int y) {
	RECT rc;
	POINT pt = { x,y };
	if (createGameBoard(hWnd, &rc)) {
		if (PtInRect(&rc, pt)) {
			x = pt.x - rc.left;
			int column = x / CELL_SIZE;
			return column;
		}
	}
	return -1;
}

POINT getClickCoordinates(HWND hWnd, int  x, int y) {
	POINT p = { -1,-1 };
	RECT rc;
	POINT pt = { x,y };
	if (createGameBoard(hWnd, &rc)) {
		if (PtInRect(&rc, pt)) {
			x = pt.x - rc.left;
			int column = x / CELL_SIZE;
			y = pt.y - rc.top;
			int row = y / CELL_SIZE;
			p.x = row;
			p.y = column;
		}
	}
	return p;
}

bool GetCellRect(HWND hWnd, int x, int y, RECT *pRect) {
	RECT rcBoard;
	if (x<0 || y<0 || x>boardSize || y>boardSize) {
		return false;
	}

	if (createGameBoard(hWnd, &rcBoard)) {
		pRect->left = rcBoard.left + x * CELL_SIZE+1;
		pRect->top = rcBoard.top + y * CELL_SIZE+1;
		pRect->right = pRect->left + CELL_SIZE-1;
		pRect->bottom = pRect->top + CELL_SIZE-1;

		return true;
	}
	return false;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: W tym miejscu umieść kod.

    // Zainicjuj ciągi globalne
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_STRATEGO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Wykonaj inicjacje aplikacji:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STRATEGO));

    MSG msg;

    // Główna pętla wiadomości:
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
//  FUNKCJA: MyRegisterClass()
//
//  CEL: Rejestruje klasę okna.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STRATEGO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_STRATEGO);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKCJA: InitInstance(HINSTANCE, int)
//
//   CEL: Zapisuje dojście wystąpienia i tworzy okno główne
//
//   KOMENTARZE:
//
//        W tej funkcji dojście wystąpienia jest zapisywane w zmiennej globalnej i
//        jest tworzone i wyświetlane okno główne programu.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Przechowuj dojście wystąpienia w zmiennej globalnej

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNKCJA: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  CEL: Przetwarza wiadomości dla okna głównego.
//
//  WM_COMMAND — przetwarzaj menu aplikacji
//  WM_PAINT — pomaluj okno główne
//  WM_DESTROY — opublikuj komunikat o rezygnacji i wróć
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
		red = CreateSolidBrush(RGB(255, 0, 0));
		green = CreateSolidBrush(RGB(0, 130, 0));
		blue = CreateSolidBrush(RGB(0, 0, 255));
		yellow = CreateSolidBrush(RGB(255, 255, 0));
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizuj zaznaczenia menu:
            switch (wmId)
            {
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
	case WM_LBUTTONDOWN:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		POINT p = getClickCoordinates(hWnd, xPos, yPos);
		HDC hdc = GetDC(hWnd);
		//if (hdc != NULL) {
		//	WCHAR tmp[100];
		//	wsprintf(tmp, L"wiersz: %d", p.x);
		//	//wsprintf(tmp, L"kolumna: %d", p.y);
		//	//TextOut(hdc, xPos, yPos, tmp, lstrlen(tmp));
		//}

		if (p.x != -1 && p.y != -1) {
			RECT rcCell;
			if (gameBoard[p.x][p.y]==0 && GetCellRect(hWnd, p.y, p.x, &rcCell)) {
				gameBoard[p.x][p.y] = playerTurn;
				FillRect(hdc, &rcCell, (playerTurn==1) ? blue:red);
				playerTurn = (playerTurn == 1) ? 2 : 1;
			}
			
		}
		ReleaseDC(hWnd, hdc);
	}

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			RECT rc;

			// TODO: tutaj dodaj kod rysowania używający elementu hdc...
			if (createGameBoard(hWnd, &rc)) {
				//FillRect(hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
			}

			for (int i = 0; i <= boardSize; i++) {
				//vertical line
				drawLine(hdc, rc.left+CELL_SIZE*i, rc.top, rc.left + CELL_SIZE * i, rc.bottom);
				//horizontal line
				drawLine(hdc, rc.left, rc.top + CELL_SIZE * i, rc.right, rc.top + CELL_SIZE * i);
			}	
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		DeleteObject(red);
		DeleteObject(green);
		DeleteObject(blue);
		DeleteObject(yellow);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//Procedura obsługi wiadomości dla okna informacji o programie.
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


