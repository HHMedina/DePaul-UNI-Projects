  
#include "Window.h"
#include "Static.h"

Static::Static(Window &parent, int xPos, int yPos, int w, int h, char *text, 
			   int Alignment, int Border) 
{ 
	Setup(parent, xPos, yPos, w, h, text, Alignment, Border);
}

void Static::Setup(Window &parent, int xPos, int yPos, int w, int h, char *text, 
				   double backR, double backG, double backB, int Alignment, int Border)
{
	SetupClassInfo(backR, backG, backB, 0); 
	Setup(parent, xPos, yPos, w, h, text, Alignment, Border); 
}

 
void Static::Setup(Window &parent, int xPos, int yPos, int w, int h, char *text, 
			   int Alignment, int Border) 
{
	if (hWnd)
		return;

	BaseStyleEx &= ~WS_EX_CLIENTEDGE;

	SetAlignment(Alignment);
	if (Border == Flat)
	{
		BaseStyle |= SS_BLACKFRAME;
	}
	else if (Border == Sunken)
	{
		BaseStyle |= SS_SUNKEN;
	}
	else if (Border == Raised)
	{
		BaseStyle |= SS_ETCHEDFRAME;
	}

	styleExtra &= ~CS_OWNDC;
	Window::Setup(&parent, xPos, yPos, w, h, true, "static", false);

	SetPaintBackground(defR, defG, defB);
	SetTextColor(0, 0, 0);

	SetText(text); 
}

void Static::SetBorder(int border)
{
	switch (border)
	{
	case Raised:
		BaseStyle &= ~SS_BLACKFRAME;
		BaseStyle &= ~SS_SUNKEN;
		BaseStyle |= SS_ETCHEDFRAME;
		break;
	case Flat:
		BaseStyle &= ~SS_SUNKEN;
		BaseStyle &= ~SS_ETCHEDFRAME;
		BaseStyle |= SS_BLACKFRAME;
		break;
	case Sunken:
		BaseStyle &= ~SS_BLACKFRAME;
		BaseStyle &= ~SS_ETCHEDFRAME;
		BaseStyle |= SS_SUNKEN;
		break;
	default:
		BaseStyle &= ~SS_BLACKFRAME;
		BaseStyle &= ~SS_SUNKEN;
		BaseStyle &= ~SS_ETCHEDFRAME;
		break;
	}
}

void Static::SetAlignment(int align)
{
	switch (align)
	{
	case LeftNoWrap:
		BaseStyle |= SS_LEFTNOWORDWRAP;
		break;
	case Left:
		BaseStyle |= SS_LEFT;
		break;
	case Right:
		BaseStyle |= SS_RIGHT;
		break;
	case Center:
		BaseStyle |= SS_CENTER;
		break;
	case Simple:
		BaseStyle |= SS_SIMPLE;
		break;
	};
	SetStyle();
}

void Static::SetDefaultBackground(void)
{
	if (backBrush != NULL)
		DeleteObject(backBrush);
	backBrush = NULL;
	Refresh();
}

void Static::SetPaintBackground(double r, double g, double b)
{
	if (backBrush != NULL)
		DeleteObject(backBrush);
	backColor = RGB(r * 255, g * 255, b * 255);
	backBrush = CreateSolidBrush(RGB(r * 255, g * 255, b * 255));
	Refresh();
}

void Static::SetTextColor(double r, double g, double b)
{
	textColor = RGB(r * 255, g * 255, b * 255);
	Refresh();
}

LONG Static::MessageHandler(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	if (msg != WM_PAINT)
		return Window::MessageHandler(hWnd, msg, wParam, lParam);
	else
		return DefaultHandler(hWnd, msg, wParam, lParam);
}

LONG Static::ProcessMessage(HWND hWnd, size_t msg, size_t wParam, LONG lParam)
{
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
	else
*/
	if (msg == WM_CTLCOLORSTATIC)
	{
		if (backBrush != NULL)
		{
			HDC staticDC = (HDC)wParam;
			::SetTextColor(staticDC, textColor);
			SetBkColor(staticDC, backColor);
			return LONG(backBrush);
		}
		else
			return 0;
	}
	else
		DefaultHandler(hWnd, msg, wParam, lParam);

	return true;
}

