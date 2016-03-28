#ifndef __BITMAPBUTTON_H__
#define __BITMAPBUTTON_H__

#include "Window.h"
#include "Bitmap.h"
 
class BitmapButton : public Window
{
public:
	
	BitmapButton() { }
	BitmapButton(Window &parent, int xPos, int yPos, int w, int h, char *upBitName, 
				 char *downBitName = "", char *disableBitName = "");

	void Setup(Window &parent, int xPos, int yPos, int w, int h, char *upBitName, 
				 char *downBitName = "", char *disableBitName = "");

	virtual void Enable(bool value) { EnableWindow(hWnd, value); InvalidateRect(hWnd, NULL, false); }

	void Click(void);
	void SetClickEvent(void (*ClickHandler)(void)) { clickHandler = ClickHandler; }

	template <class T>
	void SetClickEvent(T *ClickListener, void (T::*ClickHandler)(void))
	{ clickListener = (EventListener *)ClickListener; dClickHandler = (void (EventListener::*)(void))ClickHandler; }

private:

	void OnCommand(size_t Id, size_t Code, HWND hControl);

	Bitmap *upBitmap;
	Bitmap *downBitmap;
	Bitmap *disabledBitmap;

	void (*clickHandler)(void);
	EventListener *clickListener;
	void (EventListener::*dClickHandler)(void);

	LONG MessageHandler(HWND hWnd, UINT msg, UINT wParam, LONG lParam);
	LONG ProcessMessage(HWND hWnd, UINT msg, UINT wParam, LONG lParam);
};


#endif