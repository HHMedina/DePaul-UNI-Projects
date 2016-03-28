 /////////////////////////////////////////////////////
// Window.cpp: implementation of the Window class. //
/////////////////////////////////////////////////////

#define __WINDOW_CPP__

#include "jmisc.h"
#include <windows.h>
#include <iostream>
using namespace std;

#include "Window.h"
#include "Bitmap.h"
#include "mmsystem.h"

int OverlappedStyle = WS_OVERLAPPEDWINDOW; 

LONG CALLBACK WindowWndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
    Window *win = GetWin(hWnd);
    return win->MessageHandler(hWnd, message, wParam, lParam);
} 

void MessageError(void)
{
     LPVOID lpMsgBuf;

     FormatMessage( 
	  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
	  NULL,
	  GetLastError(),
	  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
	  (LPTSTR) &lpMsgBuf,
	  0,
	  NULL 
	  );

// Display the string.
     MessageBox( NULL, (LPCSTR)lpMsgBuf, "GetLastError", MB_OK|MB_ICONINFORMATION );
	       
// Free the buffer.
     LocalFree( lpMsgBuf );
}

bool WindowClassInfo::operator==(const WindowClassInfo& ci)
{
	bool same = true;

	if (strcmp(name, ci.name) == 0)
		return true;

	if ((wc.style != ci.wc.style) || (wc.hInstance != ci.wc.hInstance) ||
		(wc.hIcon != ci.wc.hIcon) || (wc.hCursor != ci.wc.hCursor) || 
		(wc.hbrBackground != ci.wc.hbrBackground) || (r != ci.r) || 
		(g != ci.g) || (b != ci.b)) return false;

	if ((wc.lpszMenuName == NULL) || (ci.wc.lpszMenuName == NULL))
		return (wc.lpszMenuName == ci.wc.lpszMenuName);
	else
		return !strcmp(wc.lpszMenuName, ci.wc.lpszMenuName);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WNDPROC				Window::Handler = WindowWndProc;
HANDLE				Window::AccelTable;
BOOL				Window::firstWindow = TRUE;
WindowClassInfo		Window::classInfos[MAX_WINDOW_CLASSES];
int					Window::nClasses = 0;

int Window::DefColor = GetSysColor(COLOR_3DFACE);
double Window::defR = GetRValue(DefColor) / 255.0; // .831;
double Window::defG = GetGValue(DefColor) / 255.0; // .815;
double Window::defB = GetBValue(DefColor) / 255.0; // .784;
HINSTANCE Window::appInstance = NULL;

Window::Window()
{
	hWnd = NULL;
	mainMenu = NULL;
	SetupClassInfo(defR, defG, defB, 0);
}

Window::Window(BOOL Visible)
{
	SetupClassInfo(defR, defG, defB, 0);
    Setup(NULL, 0, 0, 500, 500, Visible);
}

Window::Window(int XPos, int YPos, int Width, int Height, BOOL Visible, char *cName)
{
	SetupClassInfo(defR, defG, defB, 0);
    Setup(NULL, XPos, YPos, Width, Height, Visible, cName);
}

Window::Window(int x, int y, int width, int height, double bgr, double bgg, double bgb, 
               BOOL Visible, char *cName)
{
	SetupClassInfo(bgr, bgg, bgb, 0);
    Setup(NULL, x, y, width, height, Visible, cName);
}

Window::Window(Window &Parent, int XPos, int YPos, int Width, int Height, BOOL Visible, char *cName)
{
	SetupClassInfo(defR, defG, defB, 0);
    Setup(&Parent, XPos, YPos, Width, Height, Visible, cName);
}

Window::Window(Window &parent, int x, int y, int width, int height, double bgr, double bgg, double bgb, 
		       BOOL Visible, char *cName)
{
	SetupClassInfo(bgr, bgg, bgb, 0);
    Setup(&parent, x, y, width, height, Visible, cName);
}

Window::Window(const Window& win) 
{ 
	complain("Can't copy windows ... Windows must be passed by reference\n"); 
}

void Window::SetupClassInfo(double bgr, double bgg, double bgb, int StyleExtra)
{
	styleExtra = StyleExtra | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	backgroundR = bgr;
	backgroundB = bgb;
	backgroundG = bgg;
	strcpy(iconName, "");
	iconID = IDI_APPLICATION;
	strcpy(cursorName, "");
	cursorID = IDC_ARROW;
	strcpy(menuName, "");

	strcpy(className, "");

	hbrBackground = NULL;
	hInstance = NULL;
	classStyle = 0;

	BaseStyleEx = WS_EX_CLIENTEDGE;
	BaseStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
}

void Window::BuildWindowClass(void)
{
	if (strcmp(className, "") == 0) 
	{
		WindowClassInfo ci;

		strcpy(ci.name, className);
		ci.wc.lpszClassName = ci.name;
		ci.wc.style = classStyle | styleExtra; 
		ci.wc.lpfnWndProc = (WNDPROC)DefWindowProc;
		ci.wc.cbClsExtra = 0;
		ci.wc.cbWndExtra = 0;
		ci.wc.hInstance = hInstance;
		if (!strcmp(iconName, ""))
			ci.wc.hIcon = LoadIcon(hInstance, iconID);
		else
			ci.wc.hIcon = LoadIcon(hInstance, iconName);

		if (!strcmp(cursorName, ""))
			ci.wc.hCursor = LoadCursor(hInstance, cursorID);
		else
			ci.wc.hCursor = LoadCursor(hInstance, cursorName);

		ci.r = backgroundR;
		ci.g = backgroundG;
		ci.b = backgroundB;

		if (!hbrBackground)
			ci.wc.hbrBackground = CreateSolidBrush(RGB(255 * backgroundR, 255 * backgroundG, 255 * backgroundB));
		else
			ci.wc.hbrBackground = hbrBackground;

		ci.wc.lpszMenuName = menuName;

		int i;
		for (i = 0; i < nClasses; i++)
		{
			if (ci == classInfos[i])
			{
				ci.wc = classInfos[i].wc;
//				strcpy(className, ci.wc.lpszClassName);
				strcpy(className, classInfos[i].wc.lpszClassName);
				strcpy(ci.name, ci.wc.lpszClassName);
				break;
			}
		}

		if (i == nClasses)
		{
			if (nClasses == MAX_WINDOW_CLASSES)
			{
				complain("Too Many Different Types of Windows!");
			}

			strcpy(classInfos[i].name, "");
			sprintf(classInfos[i].name, "CTIWindowClass%d", i);
			classInfos[i].r = backgroundR;
			classInfos[i].g = backgroundG;
			classInfos[i].b = backgroundB;

			strcpy(className, classInfos[i].name);

			ci.wc.lpszClassName = classInfos[i].name;
			strcpy(className, classInfos[i].name);

			classInfos[i].wc = ci.wc;

			if (!RegisterClass(&ci.wc))
			{
				MessageError();
				complain("Couldn't register window class");
			}
			else
				nClasses++;
		}
		else
			strcpy(className, classInfos[i].name);
	}
}

void Window::Setup(Window *Parent, int XPos, int YPos, int Width, int Height, BOOL Visible, char *cName, bool AdjRect)
{
	SetLastError(0);

	if (IsWindow(hWnd))
	{
		TryDestroyWindow();
	}

	if (firstWindow)
	{
		aWinObject = AddAtom("WindowPointer");
		aRefCount = AddAtom("ReferenceCount");
		firstWindow = FALSE;
	}

	if (cName != NULL)
		strcpy(className, cName);
	else
		BuildWindowClass();	

	erasePen = ::CreatePen(PS_SOLID, 0, eraseColor);
	eraseBrush = ::CreateSolidBrush(eraseColor);

	parent = Parent;

	xPos = XPos;
	yPos = YPos;
	width = Width;
	height = Height;

	static int id = 0;
	char *title = "Application Window";
    if (Visible)
        BaseStyle |= WS_VISIBLE;

    RECT winRect;
    winRect.left = XPos; winRect.top = YPos;
    winRect.right = XPos + Width;
    winRect.bottom = YPos + Height;

	if (Parent)
	{
		if (AdjRect)
	        AdjustWindowRectEx(&winRect, BaseStyleEx, false, BaseStyle);
        hWnd = CreateWindowEx(BaseStyleEx, className, title, (WS_CHILD | BaseStyle), 
			XPos, YPos, winRect.right - winRect.left, winRect.bottom - winRect.top,
             Parent->hWnd, (HMENU)id++, appInstance, NULL);
	}
	else
	{
		if (AdjRect)
	        AdjustWindowRect(&winRect, OverlappedStyle | BaseStyle, false);
		hWnd = CreateWindowEx(BaseStyleEx, className, title, OverlappedStyle | BaseStyle, 
			XPos, YPos, winRect.right - winRect.left, winRect.bottom - winRect.top,
            NULL, NULL, appInstance, NULL);
	}

	if (!hWnd)
	{
		MessageError();
		complain("Cannot Create Window");
	}

	dc = ::GetDC(hWnd);
	if (!dc)
	{
		MessageError();
		complain("Cannot Get Window DC");
	}

	SetHandler();
	Initialize();

	InitializeRefCount(hWnd);
	IncrementRefCount(hWnd);

	visible = Visible;
	UpdateWindow(hWnd);
}

void Window::Initialize(void)
{
	buttonHeld[0] = buttonHeld[1] = buttonHeld[2] = FALSE;
	painting = false;

    mainMenu = NULL;
	nListeners = 0; 
}

void Window::SetHandler()
{
     DefaultHandler = (WNDPROC)GetWindowLong(hWnd, GWL_WNDPROC);
     SetWindowLong(hWnd, GWL_WNDPROC, (LONG)Handler);
     SetWin(hWnd, this);
}

void Window::TryDestroyWindow(void)
{
	if (IsWindow(hWnd))
	{
		if (RefCount(hWnd) == 1)
		{
			Show(SW_HIDE);

			HWND hCWnd = GetWindow(hWnd, GW_CHILD);
			while (hCWnd)
			{
				Window *pWin = GetWin(hCWnd);
				if (pWin)
				{
					if (pWin->parent->hWnd == hWnd)
						pWin->TryDestroyWindow();
						// pWin->parent = NULL;  // Remove the window as parent
				}
				hCWnd = GetWindow(hCWnd, GW_HWNDNEXT);
			}

			if (RemoveWin(hWnd) && DefaultHandler)
				SetWindowLong(hWnd, GWL_WNDPROC, (LONG) DefaultHandler);

			ReleaseDC(hWnd, dc);
			DestroyWindow(hWnd);
		}
		else
		{
			DecrementRefCount(hWnd);
		}
	}
	hWnd = NULL;
}

Window::~Window()
{
	TryDestroyWindow();
}

bool Window::control(bool peek)
{
    MSG msg;
	
	if (peek)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			return true;
		else
			return false;
	}
	else
	{
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			return true;
		else
			return false;
	}
}

void Window::SetBorder(bool flag)
{
    if (flag)
        BaseStyle |= WS_BORDER;
    else
        BaseStyle &= (~WS_BORDER);

	SetStyle();
}

void Window::Set3DBorder(bool flag)
{
    if (flag)
        BaseStyleEx |= WS_EX_CLIENTEDGE;
    else
        BaseStyleEx &= (~WS_EX_CLIENTEDGE);

	SetStyle();	
}

void Window::SetStyle(void)
{
	RECT cli;
	GetClientRect(hWnd, &cli);
    if (parent)
        SetWindowLong(hWnd, GWL_STYLE, WS_CHILD | BaseStyle);
    else
        SetWindowLong(hWnd, GWL_STYLE, OverlappedStyle | BaseStyle);
	SetWindowLong(hWnd, GWL_EXSTYLE, BaseStyleEx);

	AdjustWindowRectEx(&cli, BaseStyle, (mainMenu != NULL), BaseStyleEx);
    SetWindowPos(hWnd, HWND_NOTOPMOST, cli.left, cli.top, cli.right - cli.left,
		         cli.bottom - cli.top, 0); // SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED); 

    InvalidateRect(hWnd, NULL, true);
    UpdateWindow(hWnd);
}

void Window::SetParent(Window *win, int x, int y, int w, int h)
{
	if (win == this)
		return;

    Show(SW_HIDE);

    if (parent != NULL)
        RemoveEventListener(parent);
    parent = win;
    if (parent != NULL)
        AddEventListener(parent);

	SetStyle();

    if (parent)
        ::SetParent(hWnd, parent->hWnd);
    else
        ::SetParent(hWnd, NULL);

    if (w == -1)
        Move(x, y, width, height);
    else
        Move(x, y, w, h);

    SelectObject(dc, pen);
    SelectObject(dc, brush);

    Erase();

    Show();
    if (parent)
    {
        InvalidateRect(parent->hWnd, NULL, false);
        parent->Update();
    }
    Resize(width, height);
    InvalidateRect(hWnd, NULL, false);
    Update();
}

void Window::SetTitle(char *text)
{
     SetWindowText(hWnd, text);
}

void Window::AddEventListener(EventListener *l)
{
	if (nListeners < MAX_EVENT_LISTENERS)
	{
		listener[nListeners++] = l;
	}
	else
		complain("Too many listeners for this window");
}

void Window::RemoveEventListener(EventListener *l)
{
	for (int i = 0; i < nListeners; i++)
	{
		if (l == listener[i])
		{
			for (int j = i + 1; j < nListeners; j++)
				listener[j - 1] = listener[j];
			break;
		}
	}
}

LONG Window::MessageHandler(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	Window *win;
	HWND control = HWND(lParam);
	LONG result;

	if (control)
	{
		win = GetWin(control);
		if ((win != NULL) && (win != this))
			if (result = win->ProcessMessage(hWnd, msg, wParam, lParam))
				return result;
	}
	if (result = ProcessMessage(hWnd, msg, wParam, lParam))
		return result;
    else if (DefaultHandler)
        return(CallWindowProc(DefaultHandler, hWnd, msg, wParam, lParam));
    else
        return(CallWindowProc(DefWindowProc, hWnd, msg, wParam, lParam));
}


LONG Window::ProcessMessage(HWND hWnd, size_t msg, size_t wParam, LONG lParam)
{
	LONG result = 0;

	switch (msg)
    {
		case WM_QUIT:
			exit(1);
        case WM_CLOSE:
            return (OnClose());
        case WM_DESTROY:
            if (RemoveWin(hWnd) && DefaultHandler)
                SetWindowLong(hWnd, GWL_WNDPROC, (LONG)DefaultHandler);
            hWnd = 0;
            break;
        case WM_SIZE:
			RECT rect;
			GetClientRect(hWnd, &rect);
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
            RECT rect2;
            GetWindowRect(hWnd, &rect2);
            break;
        case WM_MOVE:
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			break;
        case WM_COMMAND:
        {
            if (LOWORD(wParam) == ID_QUIT)
			{
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				return OnClose();
			}
            else if (LOWORD(wParam) == ID_PRINT)
			{
                Print();
				break;
			}
			else
			{
				if (OnCommand(HWND(lParam), LOWORD(wParam), HIWORD(wParam)))
					return true;
				else break;
			}
        }
        case WM_PAINT:
		{
			HDC tmp = dc;
			PAINTSTRUCT ps;

			BeginPaint(hWnd, &ps);
			dc = ps.hdc;
			painting = true;
			if (!(result = OnPaint(ps.rcPaint)))
			{
				for (int i = 0; i < nListeners; i++)
				{
					if (listener[i]->OnPaint(ps.rcPaint))
					{
						result = true;
						break;
					}
				}
			}
			EndPaint(hWnd, &ps);
			dc = tmp;
			painting = false;
			if (result)
				return true;
			else if (DefaultHandler)
				return(CallWindowProc(DefaultHandler, hWnd, msg, wParam, lParam));
			else
				return(CallWindowProc(DefWindowProc, hWnd, msg, wParam, lParam));
		}
    };
	if (result = EventListener::ProcessMessage(hWnd, msg, wParam, lParam))
		return result;

	for (int i = 0; i < nListeners; i++)
	{
		if (listener[i]->ProcessMessage(hWnd, msg, wParam, lParam))
        {
			result = true;
            break;
        }
	}
	if (result)
		return true;
	else
		return false;
}

void Window::Move(int X, int Y, int Width, int Height, bool repaint, bool AdjRect)
{
    RECT rect;
	BOOL result;

    rect.left = X; rect.top = Y;
    rect.right = X + Width; rect.bottom = Y + Height;

	if (AdjRect)
		AdjustWindowRectEx(&rect, GetWindowLong(hWnd, GWL_STYLE), (mainMenu != NULL), GetWindowLong(hWnd, GWL_EXSTYLE));
    result = MoveWindow(hWnd, X, Y, rect.right - rect.left, rect.bottom - rect.top, repaint); 

	GetWindowRect(hWnd, &rect);
	width = Width;
	height = Height;
	xPos = X;
	yPos = Y;
}

void Window::Resize(int Width, int Height, bool repaint)
{
    Move(xPos, yPos, Width, Height, repaint);
}

void ErrorMessageBox(void)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);		
	
	MessageBox(NULL, (char *)lpMsgBuf, "Error", MB_OK);	

	LocalFree(lpMsgBuf);
}

void Window::Print(void)
{   
    PRINTDLG pd;

	pd.lStructSize = sizeof(PRINTDLG); 
	pd.hDevMode = (HANDLE) NULL; 
	pd.hDevNames = (HANDLE) NULL; 
	pd.Flags = PD_RETURNDC; 
	pd.hwndOwner = hWnd; 
	pd.hDC = (HDC) NULL; 
	pd.nFromPage = 1; 
	pd.nToPage = 1; 
	pd.nMinPage = 0; 
	pd.nMaxPage = 0; 
	pd.nCopies = 1; 
	pd.hInstance = NULL; 
	pd.lCustData = 0L; 
	pd.lpfnPrintHook = (LPPRINTHOOKPROC) NULL; 
	pd.lpfnSetupHook = (LPSETUPHOOKPROC) NULL; 
	pd.lpPrintTemplateName = (LPSTR) NULL; 
	pd.lpSetupTemplateName = (LPSTR)  NULL; 
	pd.hPrintTemplate = (HANDLE) NULL; 
	pd.hSetupTemplate = (HANDLE) NULL; 

	if (PrintDlg(&pd))
	{
		HDC printDC = pd.hDC;

		if (!(GetDeviceCaps(printDC, RASTERCAPS) & RC_BITBLT)) 
		{ 
			DeleteDC(printDC); 
			MessageBox(hWnd, "Printer cannot display bitmaps.", "Device Error", MB_OK); 
			return;
		}  
		
		DOCINFO docInfo;

		docInfo.cbSize = sizeof(DOCINFO); 
		docInfo.lpszDocName = "Window Image"; 
		docInfo.lpszOutput = NULL; 
		docInfo.lpszDatatype = NULL;
		docInfo.fwType = 0;
		
		StartDoc(printDC, &docInfo);
		StartPage(printDC);

		control();

		//HDC memDC = CreateCompatibleDC(dc);
		//HBITMAP bitmap = CreateCompatibleBitmap(printDC, width, height);

		//SelectObject(memDC, bitmap);
		//if (!BitBlt(memDC, 0, 0, width, height, dc, 0, 0, SRCCOPY))
		//{
		//	ErrorMessageBox();
		//}

		double screenHRes = (double) GetDeviceCaps(dc, LOGPIXELSX); 
		double screenVRes = (double) GetDeviceCaps(dc, LOGPIXELSY); 
 
		// Retrieve the number of pixels-per-logical-inch in the 
		// horizontal and vertical directions for the printer upon which 
		// the bitmap will be printed. 
 
		double printerHRes = (double) GetDeviceCaps(printDC, LOGPIXELSX); 
		double printerVRes = (double) GetDeviceCaps(printDC, LOGPIXELSY); 
 
		// Determine the scaling factors required to print the bitmap and 
		// retain its original proportions. 
 
		double scaleX, scaleY;

		scaleX = (printerHRes / screenHRes); 
		scaleY = (printerVRes / screenVRes); 
 
		// Compute the coordinates of the upper left corner of the 
		// centered bitmap. 
 
		int cWidthPels = GetDeviceCaps(printDC, HORZRES); 
		int xLeft = ((cWidthPels / 2) - ((int) (((float) width) * scaleX)) / 2); 
		int cHeightPels = GetDeviceCaps(printDC, VERTRES); 
		int yTop = ((cHeightPels / 2) - ((int) (((float) height) * scaleY)) / 2); 

		if (!StretchBlt(printDC, xLeft, yTop, int(width * scaleX), int(height * scaleY), 
			            dc, 0, 0, width, height, SRCCOPY))
		{
			ErrorMessageBox();
		}

		EndPage(printDC);
		EndDoc(printDC);

		DeleteDC(printDC);
	}
}

void Window::Save(char *filename)
{ 
	Bitmap b(*this, width, height); 
	b.Copy(*this, 0, 0); 
	b.Save(filename); 
}

void Window::Load(char *filename)
{ 
	Bitmap b(filename); 
	Copy(b, 0, 0);
}

