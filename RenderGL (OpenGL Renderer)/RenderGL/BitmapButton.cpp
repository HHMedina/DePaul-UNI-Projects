  
#include "Window.h"
#include "BitmapButton.h"
 
BitmapButton::BitmapButton(Window &parent, int xPos, int yPos, int w, int h, char *upBitName, 
				 char *downBitName, char *disableBitName)
{ 
	upBitmap = NULL;
	downBitmap = NULL;
	disabledBitmap = NULL;
	Setup(parent, xPos, yPos, w, h, upBitName, downBitName, disableBitName);
}

void BitmapButton::Setup(Window &parent, int xPos, int yPos, int w, int h, char *upBitName, 
				 char *downBitName, char *disableBitName)
{
	if (hWnd)
		return;

	upBitmap = new Bitmap(upBitName);
	if (strcmp(downBitName, "") == 0)
		downBitmap = upBitmap;
	else 
		downBitmap = new Bitmap(downBitName);

	if (strcmp(disableBitName, "") == 0)
		disabledBitmap = upBitmap;
	else 
		disabledBitmap = new Bitmap(disableBitName);

	BaseStyleEx &= ~WS_EX_CLIENTEDGE;
	BaseStyle |= BS_PUSHBUTTON | BS_OWNERDRAW;
	styleExtra &= ~CS_OWNDC;

	Window::Setup(&parent, xPos, yPos, w, h, true, "Button", false);
	SetEraseColor(GetSysColor(COLOR_BTNFACE));

	clickHandler = NULL;
	dClickHandler = NULL;
}

void BitmapButton::Click(void)
{
	SendMessage(hWnd, BM_CLICK, 0, 0);
}

LONG BitmapButton::MessageHandler(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	if ((msg == WM_COMMAND) || (msg == BN_CLICKED) || (msg == WM_ERASEBKGND) || (msg == WM_PAINT) || 
		(msg == WM_LBUTTONDOWN) || (msg == WM_LBUTTONUP))
		return Window::MessageHandler(hWnd, msg, wParam, lParam);
	else
		return CallWindowProc(DefaultHandler, hWnd, msg, wParam, lParam);
}

LONG BitmapButton::ProcessMessage(HWND hWnd, size_t msg, size_t wParam, LONG lParam)
{
	if (msg == WM_LBUTTONDOWN)
	{
		long res = CallWindowProc(DefaultHandler, hWnd, msg, wParam, lParam);
		SendMessage(hWnd, BM_SETSTATE, (WPARAM)BST_PUSHED, 0);
		InvalidateRect(hWnd, NULL, false);
		return res;
	}
	else if (msg == WM_LBUTTONUP)
	{
		long res = CallWindowProc(DefaultHandler, hWnd, msg, wParam, lParam);
		SendMessage(hWnd, BM_SETSTATE, (WPARAM)0, 0);
		InvalidateRect(hWnd, NULL, false);
		return res;
	}
	else if ((msg == WM_COMMAND) || (msg == BN_CLICKED))
	{
		if (clickHandler)
		{
			clickHandler();
			return true;
		}
		else if (dClickHandler)
		{
			(clickListener->*dClickHandler)();
			return true;
		}
		else 
			return false;
	}
	else if (msg == WM_ERASEBKGND)
	{
/*
		HBRUSH hOldBrush;
		HPEN hOldPen;
		RECT rect;
		HDC hDC;

		hDC = GetDC(hWnd);
		hOldBrush = (HBRUSH)SelectObject(hDC, erasePen);
		hOldPen = (HPEN)SelectObject(hDC, eraseBrush);

		GetUpdateRect(hWnd, &rect, FALSE);
		::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);

		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);

		InvalidateRect(hWnd, NULL, FALSE);
*/
		return true;	
	}
	else if (msg == WM_PAINT)
	{
		HDC tmp = dc;
		PAINTSTRUCT ps;
		RECT rect;

		BeginPaint(hWnd, &ps);

		rect.left = 0; rect.right = Width();
		rect.top = 0; rect.bottom = Height();

		HBRUSH hOldBrush = (HBRUSH)SelectObject(dc, erasePen);
		HPEN hOldPen = (HPEN)SelectObject(dc, eraseBrush);
		::Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);

		SelectObject(dc, hOldPen);
		SelectObject(dc, hOldBrush);

		int state = SendMessage(hWnd, BM_GETSTATE, 0, 0);
		Bitmap *bit;
		int offset;
		if ((state & BST_PUSHED) == 0)
		{
			DrawEdge(dc, &rect, EDGE_RAISED, BF_ADJUST | BF_RECT);
			if (IsEnabled())
				bit = upBitmap;
			else
				bit = disabledBitmap;
			offset = 0;
		}
		else
		{
			DrawEdge(dc, &rect, EDGE_SUNKEN, BF_ADJUST | BF_RECT);
			bit = downBitmap;
			offset = 1;
		}

		bit->Copy(*this, (Width() - bit->Width()) / 2 + offset, (Height() - bit->Height()) / 2 + offset, bit->Width(), 
				  bit->Height(), 0, 0);

		dc = tmp;
		return false;
	}

	return false;
}

