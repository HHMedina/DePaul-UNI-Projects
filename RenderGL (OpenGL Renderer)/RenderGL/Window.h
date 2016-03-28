// Window.h: interface for the Window class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __WINDOW_H__
#define __WINDOW_H__

#define NOMINMAX 
 
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "DCObject.h"
#include "EventListener.h"
#include "WinObject.h"

#define MAX_SCREEN_WIDTH 1600
#define MAX_SCREEN_HEIGHT 1200
#define MAX_EVENT_LISTENERS 10

class Bitmap;

inline COLORREF color(double r, double g, double b)
{ 
	return RGB(255 * r, 255 * g, 255 * b);
}

extern "C" 
{
     LONG CALLBACK WindowWndProc(HWND hWnd, UINT msg, 
				 UINT wParam, LONG lParam);
}

void MessageError(void);

class WindowClassInfo
{
public:

	bool operator==(const WindowClassInfo& ci);

	char name[256];
	WNDCLASS wc;
	double r, g, b;
};

class Window : public EventListener, public DCObject, public WinObject
{
public:

	enum DefaultMenuChoices { MAX_COMMAND_HANDLERS = 100, 
							  ID_OFFSET = 0x8000, 
							  ID_QUIT = ID_OFFSET + MAX_COMMAND_HANDLERS, 
							  ID_PRINT = ID_OFFSET + MAX_COMMAND_HANDLERS + 1,
							  ID_SYSEND = ID_OFFSET + MAX_COMMAND_HANDLERS + 2 };

	Window();
	Window(BOOL Visible);
	Window(int x, int y, int width, int height, BOOL Visible = TRUE, char *cName = NULL);
	Window(int x, int y, int width, int height, double bgr, double bgg, double bgb, 
		BOOL Visible = TRUE, char *cName = NULL);
	Window(Window &parent, int x, int y, int width, int height, BOOL Visible = TRUE, char *cName = NULL);
	Window(Window &parent, int x, int y, int width, int height, double bgr, double bgg, double bgb, 
		BOOL Visible = TRUE, char *cName = NULL);
	Window(const Window& win);
	virtual ~Window();

	virtual void Setup(Window &Parent, int x, int y, int width, int height, BOOL Visible = TRUE, char *cName = NULL, bool AdjRect = TRUE)
	{ Setup(&Parent, x, y, width, height, Visible, cName); }
	virtual void Setup(Window *Parent, int XPos, int YPos, int Width, int Height, BOOL Visible = TRUE, char *cName = NULL, bool AdjRect = TRUE);
	virtual void Setup(int x, int y, int width, int height, BOOL Visible = TRUE, char *cName = NULL, bool AdjRect = TRUE)
	{ Setup(NULL, x, y, width, height, Visible, cName); }

	virtual void Setup(Window &Parent, int x, int y, int width, int height, double bgR, double bgG,
		               double bgB, BOOL Visible = TRUE, char *cName = NULL)
	{ SetupClassInfo(bgR, bgG, bgB, 0); Setup(&Parent, x, y, width, height, Visible, cName); }
	virtual void Setup(int x, int y, int width, int height, double bgR, double bgG, double bgB,
				       BOOL Visible = TRUE, char *cName = NULL)
	{ SetupClassInfo(bgR, bgG, bgB, 0);  Setup(NULL, x, y, width, height, Visible, cName); }

	virtual int XPos() { return(xPos); }
	virtual int YPos() { return(yPos); }
	virtual int Width() { RECT r; GetClientRect(hWnd, &r); return(r.right - r.left); }
	virtual int Height() { RECT r; GetClientRect(hWnd, &r); return(r.bottom - r.top); }
	virtual int TotalWidth() { RECT r; GetWindowRect(hWnd, &r); return(r.right - r.left); }
	virtual int TotalHeight() { RECT r; GetWindowRect(hWnd, &r); return(r.bottom - r.top); }

	virtual void Enable(bool value) { EnableWindow(hWnd, value); }
	virtual bool IsEnabled(void) { return (IsWindowEnabled(hWnd) == TRUE); }

	void Quit() { PostMessage(hWnd, WM_QUIT, 0, 0); }

	static bool HandleEvents(bool peek = true) { return control(peek); }
	static bool control(bool peek = true);
	
    virtual void SetBorder(bool flag = true);
	virtual void Set3DBorder(bool flag = true);
    virtual void SetParent(Window &win, int x = -1, int y = -1, int w = -1, int h = -1)
    { SetParent(&win, x, y, w, h); }
    virtual void SetParent(int x = -1, int y = -1, int w = -1, int h = -1)
    { SetParent(NULL, x, y, w, h); }
    virtual void SetParent(Window *win, int x = -1, int y = -1, int w = -1, int h = -1);
	virtual void SetTitle(char *text);

	virtual int SetText(char *Format, ...)
	{ va_list l; va_start(l, Format); char text[255]; vsprintf(text, Format, l); 
	  return SendMessage(hWnd, WM_SETTEXT, 0, (LONG)text); }

	virtual int SetIcon(char *IconName, bool large = true)
	{ if (hIcon) DestroyIcon(hIcon); 
	  hIcon = LoadIcon(appInstance, IconName); 
	  return SendMessage(hWnd, WM_SETICON, (large ? ICON_BIG : ICON_SMALL), (LONG)hIcon); } 
	virtual int SetIcon(int IconID, bool large = true)
	{ return SetIcon(MAKEINTRESOURCE(IconID), large); }

	virtual void Move(int X, int Y, int Width, int Height, bool repaint = TRUE, bool AdjRect = true);
	virtual void Move(int X, int Y, bool repaint = true)
	{ Move(X, Y, width, height, repaint); }
	virtual void Resize(int Width, int Height, bool repaint = TRUE);
	virtual BOOL Show(int Cmd = SW_SHOW) 
    { if (Cmd != SW_HIDE) BaseStyle |= WS_VISIBLE; else BaseStyle &= (~WS_VISIBLE); 
      return(ShowWindow(hWnd, Cmd)); }
	virtual void Refresh(void) { if(!painting) InvalidateRect(hWnd, NULL, false); }
	virtual void Update(void) { }
	virtual void Print(void);

	virtual void Save(char *filename);
	virtual void Load(char *filename);

	HBRUSH EraseBrush(void) { return eraseBrush; }
	HPEN ErasePen(void) { return erasePen; }

	virtual void GetDefColor(double &r, double &g, double &b) { r = defR; g = defG; b = defB; }

	virtual void AddEventListener(EventListener *l);
	virtual void RemoveEventListener(EventListener *l);

	void SetTimer(size_t TimerID, size_t millisec)
	{ StartTimer(TimerID, millisec); }
	virtual void StartTimer(size_t TimerID, size_t millisec)
	{ ::SetTimer(hWnd, TimerID, millisec, NULL); }
	void KillTimer(size_t TimerID)
	{ StopTimer(TimerID); }
	virtual void StopTimer(size_t TimerID)
	{ ::KillTimer(hWnd, TimerID); }

	operator HWND() { return(hWnd); }

	static void SetAppInstance(HINSTANCE newInst) { appInstance = newInst; }
	static HINSTANCE GetAppInstance(void) { return appInstance; }
     
protected:
	HWND hWnd;
	HMENU mainMenu;
	static HINSTANCE appInstance;

	long classStyle;
	HINSTANCE hInstance;
	char *iconID;
	char iconName[32];
	HICON hIcon;
	char *cursorID;
	char cursorName[32];
	HCURSOR hCursor;
	HBRUSH hbrBackground;
	char menuName[32];

	void SetupClassInfo(double bgr, double bgg, double bgb, int StyleExtra);
	void BuildWindowClass(void);
	void TryDestroyWindow(void);

	Window *parent;
	char className[32];
    long BaseStyle;
	long BaseStyleEx;

	void Initialize(void);
	void SetStyle(void);

	enum { MAX_WINDOW_CLASSES = 100 };

	static WindowClassInfo classInfos[MAX_WINDOW_CLASSES];
	static int nClasses;

	int xPos, yPos;
	int width, height;
	BOOL visible;
	DWORD styleExtra;
	double backgroundR;
	double backgroundG;
	double backgroundB;

	static int DefColor;
	static double defR;
	static double defG;
	static double defB;

	static BOOL firstWindow;

	WNDPROC DefaultHandler;
	static WNDPROC Handler;

	void SetHandler();
	bool painting;

	static HANDLE AccelTable;

	EventListener *listener[MAX_EVENT_LISTENERS];
	int nListeners;

	virtual LONG ProcessMessage(HWND hWnd, size_t msg, size_t wParam, LONG lParam);
	virtual LONG MessageHandler(HWND hWnd, UINT msg, UINT wParam, LONG lParam);

	friend LONG CALLBACK WindowWndProc(HWND hWnd, UINT msg, 
				UINT wParam, LONG lParam);
};	  

inline int MessageBox(LPCSTR message, LPCSTR caption, UINT type)
{
	return MessageBox(NULL, message, caption, type);
}

inline void ControlLoop()
{
    while (!Window::HandleEvents(false))
		Sleep(1);
}

inline void EventLoop()
{
	while (!Window::HandleEvents(false)) // false = wait for messages, don't just peek
		Sleep(1);
}

inline bool Control()
{
     return Window::HandleEvents();
}

inline bool HandleEvents()
{
	return Window::HandleEvents();
}

#ifdef __WINDOW_CPP__
     ATOM aWinObject;
	 ATOM aRefCount;
#else
     extern ATOM aWinObject;
	 extern ATOM aRefCount;
#endif

inline void SetWin(HWND hWnd, Window *pWnd)
{
     SetProp(hWnd, (LPSTR)MAKELONG(aWinObject, 0), (HANDLE)pWnd);
}

inline void InitializeRefCount(HWND hWnd)
{
	SetProp(hWnd, (LPSTR)MAKELONG(aRefCount, 0), (HANDLE)0);
}

inline int RefCount(HWND hWnd)
{
	return (int)GetProp(hWnd, (LPSTR)MAKELONG(aRefCount, 0));
}

inline void IncrementRefCount(HWND hWnd)
{
	int count = RefCount(hWnd);
	SetProp(hWnd, (LPSTR)MAKELONG(aRefCount, 0), (HANDLE)(count + 1));
}

inline void DecrementRefCount(HWND hWnd)
{
	int count = RefCount(hWnd);
	SetProp(hWnd, (LPSTR)MAKELONG(aRefCount, 0), (HANDLE)(count - 1));
}

inline Window *GetWin(HWND hWnd)
{
     return((Window *)GetProp(hWnd, (LPSTR)MAKELONG(aWinObject, 0)));
}

inline Window *RemoveWin(HWND hWnd)
{
     return((Window *)RemoveProp(hWnd, (LPSTR)MAKELONG(aWinObject, 0)));
}

#endif //__WINDOW_H__
