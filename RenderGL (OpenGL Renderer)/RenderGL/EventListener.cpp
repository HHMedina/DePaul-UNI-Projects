 
#include "jmisc.h"
#include "Window.h"
#include "EventListener.h"
#include <iostream>
using namespace std;
 
template <class Item>
Item max(const Item& x, const Item& y)
{
	return (x < y ? y : x);
}

template <class Type>
Type min(const Type &x, const Type& y)
{
	return (x < y ? x : y);
}

template <class Type>
inline void swap(Type &x, Type &y)
{
	Type tmp = y;
	y = x; x = tmp;
}

LONG EventListener::ProcessMessage(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
    switch (msg)
    {
		case WM_CREATE:
			return (OnCreate((LPCREATESTRUCT *) lParam));
		case WM_QUIT:
			return (OnQuit());
        case WM_CLOSE:
            return (OnClose());
        case WM_SIZE:
            return (OnSize(LOWORD(lParam), HIWORD(lParam), wParam));
        case WM_MOVE:
            return (OnMove(LOWORD(lParam), HIWORD(lParam)));
        case WM_SETFOCUS:
            return (OnSetFocus((HWND)wParam));
        case WM_KILLFOCUS:
            return (OnLoseFocus((HWND)wParam));
        case WM_CHAR:
            return (OnChar(wParam, LOWORD(lParam)));
        case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
            return (OnKeyDown(wParam, *(KEYCODES *)&lParam));
        case WM_KEYUP:
		case WM_SYSKEYUP:
            return (OnKeyUp(wParam, *(KEYCODES *)&lParam));
        case WM_HSCROLL:
            return (OnHScroll((HWND)lParam, (UINT)LOWORD(wParam), (unsigned short)HIWORD(wParam)));
        case WM_VSCROLL:
            return (OnVScroll((HWND)lParam, (UINT)LOWORD(wParam), (unsigned short)HIWORD(wParam)));
        case WM_LBUTTONDOWN:
            buttonHeld[0] = true;
            return (OnLButtonDown(LOWORD(lParam), HIWORD(lParam), wParam));
        case WM_LBUTTONUP:
            buttonHeld[0] = false;
            return (OnLButtonUp(LOWORD(lParam), HIWORD(lParam), wParam));
        case WM_LBUTTONDBLCLK:
            return (OnLButtonDblClick(LOWORD(lParam), HIWORD(lParam), wParam));
        case WM_RBUTTONDOWN:
            buttonHeld[1] = true;
            return (OnRButtonDown(LOWORD(lParam), HIWORD(lParam), wParam));
        case WM_RBUTTONUP:
            buttonHeld[1] = false;
            return (OnRButtonUp(LOWORD(lParam), HIWORD(lParam), wParam));
        case WM_RBUTTONDBLCLK:
            return (OnRButtonDblClick(LOWORD(lParam), HIWORD(lParam), wParam));
        case WM_MBUTTONDOWN:
            buttonHeld[2] = true;
            return (OnMButtonDown(LOWORD(lParam), HIWORD(lParam), wParam));
        case WM_MBUTTONUP:
            buttonHeld[2] = false;
            return (OnMButtonUp(LOWORD(lParam), HIWORD(lParam), wParam));
        case WM_MBUTTONDBLCLK:
            return (OnMButtonDblClick(LOWORD(lParam), HIWORD(lParam), wParam));
        case WM_MOUSEMOVE:
        {
            bool result = false;

            if (buttonHeld[0])
                result |= OnLButtonDrag(short(LOWORD(lParam)), short(HIWORD(lParam)), wParam);
            else if (buttonHeld[2])
                result |= OnMButtonDrag(short(LOWORD(lParam)), short(HIWORD(lParam)), wParam);
            else if (buttonHeld[1])
                result |= OnRButtonDrag(short(LOWORD(lParam)), short(HIWORD(lParam)), wParam);

            result |= OnMouseMove(short(LOWORD(lParam)), short(HIWORD(lParam)), wParam);

            return (result);
        }
        case WM_MOUSEACTIVATE:
            return (OnMouseActivate((HWND)wParam, LOWORD(lParam)));
        case WM_COMMAND:
		{
			return (OnCommand((HWND)lParam, LOWORD(wParam), HIWORD(wParam)));
		}
        case WM_TIMER:
            return (OnTimer(wParam));
		// Note that paint must get handled in another way because of the update rectangle, and is 
		// Really only applicable to windows anyway.  So we will let the source call the listener's
		// Paint handler directly.
		default:
			return false;
    };
}	

bool EventListener::OnHScroll(HWND ScrollBar, size_t Code, unsigned short Pos)
{
	SCROLLINFO inf;

	Window *source = GetWin(ScrollBar);

	inf.cbSize = sizeof SCROLLINFO;
	inf.fMask = SIF_ALL;

	if (ScrollBar != NULL)
		GetScrollInfo(ScrollBar, SB_CTL, &inf);
	else
		GetScrollInfo(ScrollBar, SB_HORZ, &inf);

	int pos = inf.nPos;

	int Direction;
	if (inf.nMax >= inf.nMin)
		Direction = 1;
	else
		Direction = -1;

	switch (Code)
	{
		case SB_THUMBPOSITION:
			pos = inf.nTrackPos;
			break;
		case SB_LINELEFT:
			pos -= Direction;
			break;
		case SB_LINERIGHT:
			pos += Direction;
			break;
		case SB_PAGELEFT:
			pos -= Direction * inf.nPage;
			break;
		case SB_PAGERIGHT:
			pos += Direction * inf.nPage;
			break;
	}
	if (inf.nMax >= inf.nMin)
	{
		if (pos > inf.nMax)
			cout << "Max Data: " << pos << "\t" << inf.nMax << endl;
		pos = min(max(pos, inf.nMin), inf.nMax);
	}
	else // If the min and max are reversed!
		pos = min(max(pos, inf.nMax), inf.nMin);

	if (Code != SB_THUMBTRACK)
	{
		inf.nPos = pos;
		if (ScrollBar == NULL)
			SetScrollInfo(ScrollBar, SB_HORZ, &inf, true);
		else
			SetScrollInfo(ScrollBar, SB_CTL, &inf, true);

		if (ScrollBar == NULL)
			return OnHScrollChange(pos);
		else if (source != this)
			return OnScrollChange(source, pos);
		else if (source == this)
			return OnScrollChange(pos);
	}
	else
	{
		if (ScrollBar == NULL)
			SetScrollPos(ScrollBar, SB_VERT, inf.nTrackPos, true);
		else
			SetScrollPos(ScrollBar, SB_CTL, inf.nTrackPos, true);

		if (ScrollBar == NULL)
			return OnHScrollWindow(inf.nTrackPos);
		if (source != this)
			return OnScroll(source, inf.nTrackPos);
		else
			return OnScroll(inf.nTrackPos);
	}
	return false;
}

bool EventListener::OnVScroll(HWND ScrollBar, size_t Code, unsigned short Pos)
{
	SCROLLINFO inf;

	Window *source = GetWin(ScrollBar);

	inf.cbSize = sizeof SCROLLINFO;
	inf.fMask = SIF_ALL;

	if (ScrollBar != NULL)
		GetScrollInfo(ScrollBar, SB_CTL, &inf);
	else
		GetScrollInfo(ScrollBar, SB_VERT, &inf);

	int pos = inf.nPos;

	int Direction;
	if (inf.nMax >= inf.nMin)
		Direction = 1;
	else
		Direction = -1;

	switch (Code)
	{
		case SB_THUMBPOSITION:
			pos = inf.nTrackPos;
			break;
		case SB_LINEUP:
			pos -= Direction;
			break;
		case SB_LINEDOWN:
			pos += Direction;
			break;
		case SB_PAGEUP:
			pos -= Direction * inf.nPage;
			break;
		case SB_PAGEDOWN:
			pos += Direction * inf.nPage;
			break;
	}
	if (inf.nMax >= inf.nMin)
		pos = min(max(pos, inf.nMin), inf.nMax);
	else // If the min and max are reversed!
		pos = min(max(pos, inf.nMax), inf.nMin);

	if (Code != SB_THUMBTRACK)
	{
		if (ScrollBar == NULL)
			SetScrollPos(ScrollBar, SB_VERT, pos, true);
		else
			SetScrollPos(ScrollBar, SB_CTL, pos, true);

		if (ScrollBar == NULL)
			return OnVScrollChange(pos);
		else if (source != this)
			return OnScrollChange(source, pos);
		else if (source == this)
			return OnScrollChange(pos);
	}
	else
	{
		if (ScrollBar == NULL)
			SetScrollPos(ScrollBar, SB_VERT, inf.nTrackPos, true);
		else
			SetScrollPos(ScrollBar, SB_CTL, inf.nTrackPos, true);

		if (ScrollBar == NULL)
			return OnVScrollWindow(inf.nTrackPos);
		if (source != this)
			return OnScroll(source, inf.nTrackPos);
		else
			return OnScroll(inf.nTrackPos);
	}
	return false;
}

