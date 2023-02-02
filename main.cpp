#pragma comment(linker, "/opt:nowin98")
#include <windows.h>

#define SZ_INIT    TEXT("Initializing application...")
#define SZ_LOAD    TEXT("Loading resources...       ")
#define SZ_CLOSE   TEXT("Closing splash window...")
#define SZ_SPLASH  TEXT("Splash window")

#define ID_TIMER_CLOSE	0x1111
#define ID_TIMER_INIT	0x1112
#define ID_TIMER_LOAD	0x1113
#define ID_TIMER_DONE	0x1114

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst = NULL;
TCHAR SplashWndClass[28];
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;

        lstrcpy(SplashWndClass,TEXT("SplashWindow"));

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
	        DispatchMessage(&msg);
	}

	return msg.wParam;
}

// 
//  FUNCTION: MyRegisterClass()
// 
//  PURPOSE: Registers the window class.
// 
//  COMMENTS:
// 
//   This function and its use is only necessary if you want 
//   this code to be compatible with Win32 systems prior to the 
//   'RegisterClassEx' function that was added to Windows 95. It is 
//   important to call this function so that the application will 
//   get 'well formed' small icons associated with it.

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize             = sizeof(WNDCLASSEX); 
	wcex.style              = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc        = (WNDPROC)WndProc;
	wcex.cbClsExtra         = 0;
	wcex.cbWndExtra         = 0;
	wcex.hInstance          = hInstance;
	wcex.hIcon              = NULL; 
	wcex.hCursor            = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground      = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName       = NULL; 
	wcex.lpszClassName      = SplashWndClass;
	wcex.hIconSm            = NULL; 

	return RegisterClassEx(&wcex);
}

// 
//   FUNCTION: InitInstance(HANDLE, int)
// 
//   PURPOSE: Saves instance handle and creates main window
// 
//   COMMENTS:
// 
//   In this function, we save the instance handle in a global    
//   variable and create and display the main program window.
// 
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   RECT rect;
   int  splashwidth  = 350;
   int  splashheight = 350;

   hInst = hInstance; // Store instance handle in this global variable

   SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID) &rect, 0);

   hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, 
                         SplashWndClass,
                         NULL,
                         WS_OVERLAPPED,
                         (rect.right - rect.left - splashwidth)/2,
                         (rect.bottom - rect.top - splashheight)/2,   
		       splashwidth, 
                         splashheight, 
                         NULL,
                         NULL,
                         hInstance, 
                         NULL);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// 
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
// 
//  PURPOSE:  Processes messages for the main window.
// 
//  WM_COMMAND	- Process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- Post a quit message and return
// 
// 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	        case WM_NCCALCSIZE:  //CAPTURE THIS MESSAGE AND RETURN NULL
		     return NULL;
	        case WM_CREATE:
		     SetTimer(hWnd, ID_TIMER_INIT, 1000, NULL);
		     SetTimer(hWnd, ID_TIMER_LOAD, 2000, NULL);
		     SetTimer(hWnd, ID_TIMER_DONE, 4000, NULL);
		     SetTimer(hWnd, ID_TIMER_CLOSE, 5000, NULL);
		     break;
	        case WM_PAINT:
		{
		     PAINTSTRUCT ps  = { 0 };
		     RECT rect  = { 0 };
		     HDC hDC = BeginPaint(hWnd, &ps);
		     GetClientRect(hWnd, &rect);
		     InflateRect(&rect, -2,-2);
		     Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
		     InflateRect(&rect, -15,-15);
		     HFONT hFont = CreateFont(-35,-35,0,0,0,0,0,0,0,0,0,0,0,TEXT("Arial"));
		     HFONT hOldFont = (HFONT) SelectObject(hDC, hFont);
		     DrawText(hDC, SZ_SPLASH, lstrlen(SZ_SPLASH), &rect, DT_WORDBREAK);
		     SelectObject(hDC, hOldFont);
		     EndPaint(hWnd, &ps);
		}
		break;
	        case WM_DESTROY:
		     PostQuitMessage(0);
		     break;
	        case WM_TIMER:
	        {
	             HDC hDC = GetDC(hWnd);
     	             RECT rect = { 0 };
	             GetClientRect(hWnd, &rect);
	             KillTimer(hWnd, wParam);
	             switch (wParam)
	             {
	                  case ID_TIMER_CLOSE:
	                       DestroyWindow(hWnd);
	                       break;
	                  case ID_TIMER_INIT:
	                       TextOut(hDC, rect.right-200, rect.bottom-20, SZ_INIT, lstrlen(SZ_INIT));
	                       break;
	                  case ID_TIMER_LOAD:
	                       TextOut(hDC, rect.right-200, rect.bottom-20, SZ_LOAD, lstrlen(SZ_LOAD));
	                       break;
	                  case ID_TIMER_DONE:
	                       TextOut(hDC, rect.right-200, rect.bottom-20, SZ_CLOSE, lstrlen(SZ_CLOSE));
	                       break;
	             }
	             ReleaseDC(hWnd, hDC);
	        }
	        break;
	        default:
		     return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}
