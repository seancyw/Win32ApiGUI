
// Used to reduce unnecessary function parsing
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAD_AND_MEAN
#endif

#include <Windows.h>
#include <iostream>

// forward declaration
HINSTANCE hInstance;
HBITMAP hBitmap = NULL;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//setup window class attributes
	WNDCLASSEX wcex;

	ZeroMemory(&wcex, sizeof(wcex));

	::hInstance = hInstance;
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpszClassName = TEXT("IMAGEVIEWCLASS");
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcex.lpfnWndProc = WndProc;

	// Register window and ensure registration sucess
	if (!RegisterClassEx(&wcex))
		return 1;

	// Setup window initialization attributes
	CREATESTRUCT cs;
	ZeroMemory(&cs, sizeof(cs));

	cs.x = 0;
	cs.y = 0;
	cs.cx = 640;
	cs.cy = 480;
	cs.hInstance = hInstance;
	cs.lpszClass = wcex.lpszClassName;
	cs.lpszName = TEXT("Image Window");
	cs.style = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX;

	HWND hWnd = ::CreateWindowEx(cs.dwExStyle, cs.lpszClass, cs.lpszName,
		cs.style, cs.x, cs.y, cs.cx, cs.cy, cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);

	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);

	// Main message loop
	MSG msg;
	while (::GetMessage(&msg, hWnd, 0, 0) > 0)
		::DispatchMessage(&msg);

	// Unregister window class, freeing the memory that was
	// previously allocated for this window.
	::UnregisterClass(wcex.lpszClassName, hInstance);

	return static_cast<int>( msg.wParam );
}

// Window Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		::hBitmap = (HBITMAP)LoadImage(::hInstance, TEXT("c:\\mercury.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (::hBitmap == NULL)
		{
			DWORD dwError = ::GetLastError();
			throw ;
		}

		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc, memoryDC;
		BITMAP bitmap;
		HGDIOBJ oldBitmap;

		hdc = BeginPaint(hWnd, &ps);
		
		// Create memory device compatible with above dc variables
		memoryDC = CreateCompatibleDC(hdc);

		// Select new bitmap
		oldBitmap = SelectObject(memoryDC, ::hBitmap);

		GetObject(::hBitmap, sizeof(bitmap), &bitmap);
		BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, memoryDC, 0, 0, SRCCOPY);

		SelectObject(memoryDC, oldBitmap);

		//Restore old bitmap
		DeleteDC(memoryDC);
		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		::DeleteObject(::hBitmap);
		PostQuitMessage(WM_QUIT);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}