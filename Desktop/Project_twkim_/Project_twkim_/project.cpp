#include <windows.h>
#include <tchar.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI _tWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow) //윈도우프로그램 시작점
{					//응용 프로그램 인스턴스핸들									//창이 보여지는형태 WS_SHOW
	HWND hwnd;
	MSG msg;

	//1. 윈도우 창 초기화(창 생성 화면 출력)

	//1-1. 윈도우클래스 정의 & 등록
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); //구조체 사이즈(크기)
	wcex.style = CS_HREDRAW | CS_VREDRAW; //창 업데이트 가로 세로로 해줌
	wcex.lpfnWndProc = WndProc; //윈도우프로시저함수(중요)
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinstance; //응용프로그램 인스턴스(중요)
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); //아이콘 스타일
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //커서 스타일
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //배경색 스타일
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); //메뉴설정 LoadMenu
	wcex.lpszClassName = _T("MyClass"); //클래스이름(중요)
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wcex); //클래스 등록

	//1-2. 창 생성 화면 출력
	hwnd = CreateWindow(_T("MyClass"), _T("나의 윈도우 프로그램"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hinstance, NULL); //창 생성
	ShowWindow(hwnd, nCmdShow);	//창 출력

	UpdateWindow(hwnd); //창 갱신	


	//2. 메시지 루프(메시지 꺼내서 WndProc 전달)
	while (GetMessage(&msg, NULL, 0, 0)) //WM_QUIT이 들어올때까지
	{
		TranslateMessage(&msg); //키보드 문자키를 누르는 이벤트 (WM_CHAR)
		DispatchMessage(&msg);	//WndProc애 메시지 전송
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) //윈도우 프로시저
{
	HDC hdc;
	PAINTSTRUCT ps;

	static int x, y;
	static POINT point;
	static BOOL bDraw, bSelect, bPaste;
	static HMENU hMenu, hSubMenu;

	HPEN hPen, oldPen;

	//3. 메시지 처리
	switch (iMsg)
	{
	case WM_CREATE:
		hMenu = GetMenu(hwnd);
		hSubMenu = GetSubMenu(hMenu, 0);
		x = 20;
		y = 20;
		bDraw = FALSE;
		bSelect = FALSE;
		bPaste = FALSE;
		break;
	case WM_LBUTTONDOWN:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);

		if (point.x >= x - 20 && point.y >= y - 20 && point.x <= x + 20 && point.y <= y + 20) 
			bDraw = TRUE;

		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);
		if (bDraw) {
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
		}
		if (bDraw)
			TextOut(hdc, 50, 50, _T("True"), _tcslen(_T("True")));
		else
			TextOut(hdc, 50, 50, _T("false"), _tcslen(_T("false")));

		EnableMenuItem(hSubMenu, ID_COPY, bDraw ? MF_ENABLED : MF_GRAYED);
		EnableMenuItem(hSubMenu, ID_PASTE, bSelect ? MF_ENABLED : MF_GRAYED);
		
		if(bPaste)
			Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);

		EndPaint(hwnd, &ps);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case ID_COPY:
			bSelect = TRUE;
			
			break;
		case ID_PASTE:
			bDraw = FALSE;
			bSelect = FALSE;
			bPaste = TRUE;

			x += 40;
			y += 40;
			
			break;
		}
		
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

/*
선택한 원을 복사하여 붙여넣기
1. 중심이(20, 20)이고 반지름이 20인 원을 그리시오.
2. 마우스 클릭으로 원이 선택되면 원의 우측에 복사하여 붙여 넣는 프로그램을 작성하시오.
3. 화면에 붙여 넣기 결과로 나온 원도 다시 선택하여 복사하기 후 붙여 넣기를 할 수 있다.이 과정을 반복하여 최대 10개까지 원이 화면에 나타날 수 있어야 한다.
*/
