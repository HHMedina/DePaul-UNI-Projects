#ifndef __WINOBJECT_H__
#define __WINOBJECT_H__

class Window;

#include <stdarg.h>
 
class WinObject
{
public:

	virtual int XPos() = 0;
	virtual int YPos() = 0;

    void SetParent(Window &win, int x = -1, int y = -1, int w = -1, int h = -1)
    { SetParent(&win, x, y, w, h); }
    void SetParent(int x = -1, int y = -1, int w = -1, int h = -1)
    { SetParent(NULL, x, y, w, h); }
	virtual void SetParent(Window *win, int x = -1, int y = -1, int w = -1, int h = -1) = 0;
	virtual void SetTitle(char *text) = 0;

	virtual int SetText(char *Format, ...) = 0;

    virtual void SetBorder(bool flag = true) = 0;
	virtual void Move(int X, int Y, int Width, int Height, bool repaint = TRUE, bool AdjRect = true) = 0;
	virtual void Move(int X, int Y, bool repaint = TRUE) = 0;
	virtual void Resize(int Width, int Height, bool repaint = TRUE) = 0;
	virtual BOOL Show(int Cmd = SW_SHOW) = 0;
	virtual void Update(void) = 0;

	virtual void Print(void) = 0;

	virtual void AddEventListener(EventListener *l) = 0;
	virtual void RemoveEventListener(EventListener *l) = 0;

	virtual operator HWND() = 0;
};

#endif