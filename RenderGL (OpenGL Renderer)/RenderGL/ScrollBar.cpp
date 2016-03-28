 
#include "ScrollBar.h"
#include "window.h"

ScrollBar::ScrollBar(Window &parent, size_t xPos, size_t yPos, size_t w, size_t h, int min, int max,  
		  int val, int pageSize, bool vertical)
{
	Setup(parent, xPos, yPos, w, h, min, max, val, pageSize, vertical);
}

ScrollBar::ScrollBar(Window &parent, size_t xPos, size_t yPos, size_t w, size_t h, int min, int max, 
		  int val, int pageSize, bool vertical, void (*changeHandler)(int Pos),
		  void (*scrollHandler)(int Pos))
{
	Setup(parent, xPos, yPos, w, h, min, max, val, pageSize, vertical);
	SetScrollEvent(scrollHandler);
	SetScrollEvent(changeHandler);
}

//ScrollBar::ScrollBar(Window &parent, size_t xPos, size_t yPos, size_t w, size_t h, int min, int max, 
//		  int val, int pageSize, bool vertical, 
//		  EventListener *ChangeHObj, void (EventListener::*changeHandler)(int Pos),
//		  EventListener *ScrollHObj, void (EventListener::*scrollHandler)(int Pos))
//{
//	Setup(parent, xPos, yPos, w, h, min, max, val, pageSize, vertical);
//	SetScrollEvent(ChangeHObj, scrollHandler);
//	SetScrollEvent(ScrollHObj, changeHandler);
//}
 
void ScrollBar::Setup(Window &parent, size_t xPos, size_t yPos, size_t w, size_t h, int min, int max,  
		  int val, int pageSize, bool vertical)
{
	if (!vertical)
		BaseStyle = SBS_HORZ;
	else
		BaseStyle = SBS_VERT;

	BaseStyle &= (!WS_BORDER);
	BaseStyleEx &= (!WS_EX_CLIENTEDGE);
	styleExtra &= ~CS_OWNDC;

	Window::Setup(&parent, xPos, yPos, w, h, true, "scrollbar");

	SetRange(min, max);
	SetPageSize(pageSize);
	SetRange(min, max);
	if (val == INT_MIN)
		SetPosition(min);
	else
		SetPosition(val);

	onScroll = NULL;
	onChange = NULL;
	dOnScroll = NULL;
	dOnChange = NULL;

}

void ScrollBar::SetPosition(int newPos)
{
	SCROLLINFO inf;

	inf.cbSize = sizeof(SCROLLINFO);
	inf.fMask = SIF_POS;
	inf.nPos = newPos;
	SetScrollInfo(hWnd, SB_CTL, &inf, true);
}

int ScrollBar::GetPosition(void)
{
	SCROLLINFO inf;

	inf.cbSize = sizeof(SCROLLINFO);
	inf.fMask = SIF_POS;
	GetScrollInfo(hWnd, SB_CTL, &inf);
	return inf.nPos;
}

int ScrollBar::GetTrackPosition(void)
{
	SCROLLINFO inf;

	inf.cbSize = sizeof(SCROLLINFO);
	inf.fMask = SIF_TRACKPOS;
	GetScrollInfo(hWnd, SB_CTL, &inf);
	return inf.nTrackPos;
}

void ScrollBar::SetMax(int max)
{
	SCROLLINFO inf;

	inf.cbSize = sizeof(SCROLLINFO);
	inf.fMask = SIF_RANGE;
	GetScrollInfo(hWnd, SB_CTL, &inf);

	inf.nMax = max + GetPageSize() - 1;
	SetScrollInfo(hWnd, SB_CTL, &inf, true);
}

void ScrollBar::SetMin(int min)
{
	SCROLLINFO inf;

	inf.cbSize = sizeof(SCROLLINFO);
	inf.fMask = SIF_RANGE;
	GetScrollInfo(hWnd, SB_CTL, &inf);

	inf.nMin = min;
	SetScrollInfo(hWnd, SB_CTL, &inf, true);
}
 
void ScrollBar::SetRange(int min, int max)
{
	SCROLLINFO inf;

	inf.cbSize = sizeof(SCROLLINFO);
	inf.fMask = SIF_RANGE;
	GetScrollInfo(hWnd, SB_CTL, &inf);

	inf.nMin = min;
	inf.nMax = max + GetPageSize() - 1;
	SetScrollInfo(hWnd, SB_CTL, &inf, true);
}

int ScrollBar::GetMax(void)
{
	SCROLLINFO inf;

	inf.cbSize = sizeof(SCROLLINFO);
	inf.fMask = SIF_RANGE;
	GetScrollInfo(hWnd, SB_CTL, &inf);

	return inf.nMax - GetPageSize() + 1;
}

int ScrollBar::GetMin(void)
{
	SCROLLINFO inf;

	inf.cbSize = sizeof(SCROLLINFO);
	inf.fMask = SIF_RANGE;
	GetScrollInfo(hWnd, SB_CTL, &inf);

	return inf.nMin;
}

void ScrollBar::GetRange(int &max, int &min)
{
	SCROLLINFO inf;

	inf.cbSize = sizeof(SCROLLINFO);
	inf.fMask = SIF_RANGE;
	GetScrollInfo(hWnd, SB_CTL, &inf);

	max = inf.nMax - GetPageSize() + 1;
	min = inf.nMin;
}

void ScrollBar::SetPageSize(int size)
{
	SCROLLINFO inf;

	inf.cbSize = sizeof(SCROLLINFO);
	inf.fMask = SIF_PAGE;
	inf.nPage = size;
	SetScrollInfo(hWnd, SB_CTL, &inf, true);
}

int ScrollBar::GetPageSize(void)
{
	SCROLLINFO inf;

	inf.cbSize = sizeof(SCROLLINFO);
	inf.fMask = SIF_PAGE;
	GetScrollInfo(hWnd, SB_CTL, &inf);

	return inf.nPage;
}

LONG ScrollBar::MessageHandler(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	if (msg != WM_PAINT)
		return Window::MessageHandler(hWnd, msg, wParam, lParam);
	else
		return DefaultHandler(hWnd, msg, wParam, lParam);
}

LONG ScrollBar::ProcessMessage(HWND hWnd, size_t msg, size_t wParam, LONG lParam)
{
	// We might be able to fix windows wierd behavior with 
	// Dragging the thumb and moving off the scroll bar
	// Here!

	// Don't remember why I had this, but it makes the control flicker!
/*	if (msg == WM_ERASEBKGND)
	{
		HBRUSH hOldBrush;
		HPEN hOldPen;
		RECT rect;
		HDC hDC;

		hDC = GetDC(hWnd);
		hOldBrush = (HBRUSH)SelectObject(hDC, parent->EraseBrush());
		hOldPen = (HPEN)SelectObject(hDC, parent->ErasePen());

		GetClientRect(hWnd, &rect);
		::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);

		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);

		InvalidateRect(hWnd, NULL, FALSE);
		return true;
	}
*/
	if ((msg != WM_PAINT) && (msg != WM_LBUTTONDOWN))
	{
		return Window::ProcessMessage(hWnd, msg, wParam, lParam);
	}
	else
	{
		DefaultHandler(hWnd, msg, wParam, lParam);
		return true;
	}
}

bool ScrollBar::OnScroll(int Pos)
{
	if (onScroll)
	{
		onScroll(Pos);
		return true;
	}
	else if (dOnScroll)
	{
		(scrollListener->*dOnScroll)(Pos);
		return true;
	}
	else
		return false;
}

bool ScrollBar::OnScrollChange(int Pos)
{
	if (onChange)
	{
		onChange(Pos);
		return true;
	}
	else if (dOnChange)
	{
		(changeListener->*dOnChange)(Pos);
		return true;
	}
	else
		return false;
}

