#ifndef __EVENTLISTENER_H__
#define __EVENTLISTENER_H__

#include <windows.h>

struct KEYCODES
{
	int RepeatCount : 16,
		ScanCode : 8,
		fExtended : 1,
		reserved1 : 4,
		fAlt : 1,
		fPrev : 1,
		fTransition : 1;
};
 
const int KeyLeftArrow = 37;
const int KeyUpArrow = 38;
const int KeyRightArrow = 39;
const int KeyDownArrow = 40;

typedef RECT intRect;

class WinObject;

class EventListener
{
public:

	virtual LONG ProcessMessage(HWND hWnd, size_t msg, size_t wParam, LONG lParam);

	virtual bool OnCreate(LPCREATESTRUCT *cs) { return false; }
	virtual bool OnQuit(void) { delete this; exit(0); return false; }
	virtual bool OnClose(void) { return false; }
	virtual bool OnSize(size_t Width, size_t Height, size_t Type) { return false; }
	virtual bool OnMove(size_t XPos, size_t YPos) { return false; }
	virtual bool OnChar(size_t Value, size_t Repeat) { return false; }
	virtual bool OnKeyDown(size_t VirtKey, KEYCODES Codes) { return false; }
	virtual bool OnKeyUp(size_t VirtKey, KEYCODES Codes) { return false; }

	virtual bool OnLButtonDown(int x, int y, size_t Keys) { return false; }
	virtual bool OnLButtonUp(int x, int y, size_t Keys) { return false; }
	virtual bool OnLButtonDblClick(int x, int y, size_t Keys) { return false; }
	virtual bool OnLButtonDrag(int x, int y, size_t Keys) { return false; }
	virtual bool OnRButtonDown(int x, int y, size_t Keys) { return false; }
	virtual bool OnRButtonUp(int x, int y, size_t Keys) { return false; }
	virtual bool OnRButtonDblClick(int x, int y, size_t Keys) { return false; }
	virtual bool OnRButtonDrag(int x, int y, size_t Keys) { return false; }
	virtual bool OnMButtonDown(int x, int y, size_t Keys) { return false; }
	virtual bool OnMButtonUp(int x, int y, size_t Keys) { return false; }
	virtual bool OnMButtonDblClick(int x, int y, size_t Keys) { return false; }
	virtual bool OnMButtonDrag(int x, int y, size_t Keys) { return false; }
	virtual bool OnMouseMove(int x, int y, size_t Keys) { return false; }
	virtual bool OnMouseActivate(HWND hTop, size_t HitTest) { return false; }
	virtual bool OnPaint(intRect rect) { return false; }
	virtual bool OnSetFocus(HWND hPrev) { return false; }
	virtual bool OnLoseFocus(HWND hNext) { return false; }
	virtual bool OnTimer(size_t timerId) { return false; }

	// ScrollBar Control Functions
	virtual bool OnHScroll(HWND Scrollbar, size_t Code, unsigned short Pos); 
	virtual bool OnVScroll(HWND Scrollbar, size_t Code, unsigned short Pos);
	virtual bool OnScroll(WinObject *Source, int Pos) { return false; }
	virtual bool OnScrollChange(WinObject *Source, int Pos) { return false; }
	// Handlers for the window scroll bars
	virtual bool OnScroll(int Pos) { return false; }
	virtual bool OnHScrollWindow(int Pos) { return false; }
	virtual bool OnVScrollWindow(int Pos) { return false; }
	virtual bool OnScrollChange(int Pos) { return false; }
	virtual bool OnHScrollChange(int Pos) { return false; }
	virtual bool OnVScrollChange(int Pos) { return false; }

	// TextBox Control Functions
	virtual bool OnTextChange(WinObject *Source, char *newText) { return false; }
	virtual bool OnTextAccept(WinObject *Source, char *newText) { return false; }
	
	// Control Command Functions
	virtual bool OnCommand(HWND hControl, size_t Id, size_t Code) { return false; }
	virtual bool OnCommand(WinObject *Source, size_t Id, size_t Code) { return false; }

protected:

	bool buttonHeld[3];
};

#endif