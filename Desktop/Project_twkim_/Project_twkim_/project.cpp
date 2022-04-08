#include <windows.h>
#include <tchar.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI _tWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow) //���������α׷� ������
{					//���� ���α׷� �ν��Ͻ��ڵ�									//â�� ������������ WS_SHOW
	HWND hwnd;
	MSG msg;

	//1. ������ â �ʱ�ȭ(â ���� ȭ�� ���)

	//1-1. ������Ŭ���� ���� & ���
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); //����ü ������(ũ��)
	wcex.style = CS_HREDRAW | CS_VREDRAW; //â ������Ʈ ���� ���η� ����
	wcex.lpfnWndProc = WndProc; //���������ν����Լ�(�߿�)
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinstance; //�������α׷� �ν��Ͻ�(�߿�)
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); //������ ��Ÿ��
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //Ŀ�� ��Ÿ��
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //���� ��Ÿ��
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); //�޴����� LoadMenu
	wcex.lpszClassName = _T("MyClass"); //Ŭ�����̸�(�߿�)
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wcex); //Ŭ���� ���

	//1-2. â ���� ȭ�� ���
	hwnd = CreateWindow(_T("MyClass"), _T("���� ������ ���α׷�"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hinstance, NULL); //â ����
	ShowWindow(hwnd, nCmdShow);	//â ���

	UpdateWindow(hwnd); //â ����	


	//2. �޽��� ����(�޽��� ������ WndProc ����)
	while (GetMessage(&msg, NULL, 0, 0)) //WM_QUIT�� ���ö�����
	{
		TranslateMessage(&msg); //Ű���� ����Ű�� ������ �̺�Ʈ (WM_CHAR)
		DispatchMessage(&msg);	//WndProc�� �޽��� ����
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) //������ ���ν���
{
	HDC hdc;
	PAINTSTRUCT ps;

	static int x, y;
	static POINT point;
	static BOOL bDraw, bSelect, bPaste;
	static HMENU hMenu, hSubMenu;

	HPEN hPen, oldPen;

	//3. �޽��� ó��
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
������ ���� �����Ͽ� �ٿ��ֱ�
1. �߽���(20, 20)�̰� �������� 20�� ���� �׸��ÿ�.
2. ���콺 Ŭ������ ���� ���õǸ� ���� ������ �����Ͽ� �ٿ� �ִ� ���α׷��� �ۼ��Ͻÿ�.
3. ȭ�鿡 �ٿ� �ֱ� ����� ���� ���� �ٽ� �����Ͽ� �����ϱ� �� �ٿ� �ֱ⸦ �� �� �ִ�.�� ������ �ݺ��Ͽ� �ִ� 10������ ���� ȭ�鿡 ��Ÿ�� �� �־�� �Ѵ�.
*/
