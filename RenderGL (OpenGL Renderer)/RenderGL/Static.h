
#ifndef __STATIC_H__
#define __STATIC_H__

#include "Window.h"
#include "AppWindow.h"

class Static : public AppWindow
{
public:
	
	enum BorderType { None = 0, Flat = 1, Sunken = 2, Raised = 3 };
	enum AlignmentType { LeftNoWrap = 0, Left = 1, Right = 2, Center = 3, Simple = 4 };

	Static() { }
	Static(Window &parent, int xPos, int yPos, int w, int h,
		char *text, int Alignment = Left, int Border = None);

	void Setup(Window &parent, int xPos, int yPos, int w, int h, char *text, 
		int Alignment = Left, int Border = None);
	void Setup(Window &parent, int xPos, int yPos, int w, int h, char *text, 
		double backR, double backG, double backB, int Alignment = Left, int Border = None);

	void SetAlignment(int align);
	void SetBorder(int border);

	void SetTextColor(double r, double g, double b);
	void SetDefaultBackground(void);
	void SetPaintBackground(double r, double g, double b);

private:

	COLORREF backColor;
	COLORREF textColor;
	HBRUSH backBrush;

	LONG MessageHandler(HWND hWnd, UINT msg, UINT wParam, LONG lParam);
	LONG ProcessMessage(HWND hWnd, size_t msg, size_t wParam, LONG lParam);
};

typedef Static Label;

#endif