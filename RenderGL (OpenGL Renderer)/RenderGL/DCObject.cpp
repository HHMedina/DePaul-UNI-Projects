  
#define NOMINMAX

#include <math.h>
#include "DCObject.h"
#include "jmisc.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
 
DCObject::DCObject()
{
	dc = NULL;
	SetDrawStyle(0.0, 0.0, 0.0, 1);
	SetFillColor(0.0, 0.0, 0.0);
	SetPaintBackground(1.0, 1.0, 1.0);
	SetEraseColor(1.0, 1.0, 1.0);
	// SetFont("Times New Roman", 10);
	fontSize = 10;
	fontAngle = 0;
	fontCharAngle = 0;
	fontItalic = false;
	fontBold = false;
	fontUnderline = false;
}

DCObject::~DCObject()
{
	SelectObject(dc, GetStockObject(BLACK_PEN));
	SelectObject(dc, GetStockObject(BLACK_BRUSH));
	SelectObject(dc, GetStockObject(SYSTEM_FONT));

	DeleteObject(pen);
	DeleteObject(brush);
	DeleteObject(font);

	// Note that we do not release or delete the DC here, because the procedure
	// For deleting the DC depends on the type of DC.  Either DeleteDC or ReleaseDC
}

void DCObject::SetEraseColor(double r, double g, double b)
{
    SetEraseColor(RGB(r * 255, g * 255, b * 255));
}

void DCObject::SetEraseColor(COLORREF newColor)
{
    eraseColor = newColor;

    SelectObject(dc, pen);
    SelectObject(dc, brush);

    DeleteObject(erasePen);
    DeleteObject(eraseBrush);

	erasePen = ::CreatePen(PS_SOLID, 0, eraseColor);
	eraseBrush = ::CreateSolidBrush(eraseColor);
}

void DCObject::Erase(void)
{
    SelectObject(dc, erasePen);
    SelectObject(dc, eraseBrush);

	FillRectangle(0, 0, Width(), Height());

    SelectObject(dc, pen);
    SelectObject(dc, brush);
}

void DCObject::LineStrip(int *x, int *y, int nPoints)
{
	intPoint *points = new intPoint[nPoints];

	for (int i = 0; i < nPoints; i++)
	{
		points[i].x = x[i];
		points[i].y = y[i];
	}

	LineStrip(points, nPoints);
}

void DCObject::Bezier(int *x, int *y, int nPoints)
{
	intPoint *points = new intPoint[nPoints];

	for (int i = 0; i < nPoints; i++)
	{
		points[i].x = x[i];
		points[i].y = y[i];
	}

	Bezier(points, nPoints);
}

static const int Granularity = 10000;

void DCObject::ArcTo(int x1, int y1, int x2, int y2, double angle1, double angle2)
{
	int cx = (x1 + x2) / 2;
	int cy = (y1 + y2) / 2;

	int beginX = cx + int(Granularity * cos(angle1)); // go out far so granularity for ints is fine.
	int beginY = cy + int(Granularity * sin(angle1));
	int endX = cx + int(Granularity * cos(angle1));
	int endY = cy + int(Granularity * sin(angle1));

	::ArcTo(dc, x1, y1, x2, y2, beginX, beginY, endX, endY);
}

void DCObject::Arc(int x1, int y1, int x2, int y2, double angle1, double angle2)
{
	int cx = (x1 + x2) / 2;
	int cy = (y1 + y2) / 2;

	int beginX = cx + int(Granularity * cos(angle1)); // go out far so granularity for ints is fine.
	int beginY = cy + int(Granularity * sin(angle1));
	int endX = cx + int(Granularity * cos(angle1));
	int endY = cy + int(Granularity * sin(angle1));

	::Arc(dc, x1, y1, x2, y2, beginX, beginY, endX, endY);
}

void DCObject::CircleArcTo(int centerX, int centerY, int radius, double angle1, double angle2)
{
	int beginX = centerX + int(Granularity * cos(angle1)); // go out far so granularity for ints is fine.
	int beginY = centerY + int(Granularity * sin(angle1));
	int endX = centerX + int(Granularity * cos(angle1));
	int endY = centerY + int(Granularity * sin(angle1));

	::ArcTo(dc, centerX - radius, centerY - radius, centerX + radius, centerY + radius, beginX, beginY, endX, endY);
}

void DCObject::CircleArc(int centerX, int centerY, int radius, double angle1, double angle2)
{
	int beginX = centerX + int(Granularity * cos(angle1)); // go out far so granularity for ints is fine.
	int beginY = centerY + int(Granularity * sin(angle1));
	int endX = centerX + int(Granularity * cos(angle1));
	int endY = centerY + int(Granularity * sin(angle1));

	::Arc(dc, centerX - radius, centerY - radius, centerX + radius, centerY + radius, beginX, beginY, endX, endY);
}

void DCObject::Ellipse(int x1, int y1, int x2, int y2)
{
	SelectObject(dc, GetStockObject(NULL_BRUSH));
	::Ellipse(dc, x1, y1, x2, y2);
	SelectObject(dc, brush);
}

void DCObject::Circle(int x, int y, int radius)
{
	SelectObject(dc, GetStockObject(NULL_BRUSH));
	::Ellipse(dc, x - radius, y - radius, x + radius, y + radius);
	SelectObject(dc, brush);
}

void DCObject::FillEllipse(int x1, int y1, int x2, int y2)
{
	::Ellipse(dc, x1, y1, x2, y2);
}

void DCObject::FillCircle(int x, int y, int radius)
{
	::Ellipse(dc, x - radius, y - radius, x + radius, y + radius);
}

void DCObject::Rectangle(int x1, int y1, int x2, int y2)
{
	SelectObject(dc, GetStockObject(NULL_BRUSH));
	::Rectangle(dc, x1, y1, x2, y2);
	SelectObject(dc, brush);
}

void DCObject::Rectangle(const intRect& rect)
{
	SelectObject(dc, GetStockObject(NULL_BRUSH));
	::Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
	SelectObject(dc, brush);
}

void DCObject::RoundRectangle(int x1, int y1, int x2, int y2, int width, int height)
{ 
	SelectObject(dc, GetStockObject(NULL_BRUSH)); 
	RoundRect(dc, x1, y1, x2, y2, width, height); 
	SelectObject(dc, brush); 
}

void DCObject::Polygon(intPoint *points, int nPoints) 
{ 
	SelectObject(dc, GetStockObject(NULL_BRUSH)); 
	::Polygon(dc, points, nPoints); 
	SelectObject(dc, brush); 
}

void DCObject::PolyPolygon(intPoint *points, int *pointsInEach, int nPoints)
{ 
	SelectObject(dc, GetStockObject(NULL_BRUSH)); 
	::PolyPolygon(dc, points, pointsInEach, nPoints); 
	SelectObject(dc, brush); 
}

void DCObject::BeginDragRectangle(int x, int y)
{
	lastX1 = x; lastY1 = y;
	lastX2 = x; lastY2 = y;
	SetROP2(dc, R2_XORPEN);
	::Rectangle(dc, x, y, x, y);
	if (!xor)
		SetROP2(dc, R2_COPYPEN);
}

void DCObject::DragRectangle(int x, int y)
{
	SetROP2(dc, R2_XORPEN);
	::Rectangle(dc, lastX1, lastY1, lastX2, lastY2); 
	lastX2 = x; lastY2 = y;
	::Rectangle(dc, lastX1, lastY1, x, y);
	if (!xor)
		SetROP2(dc, R2_COPYPEN);
}

void DCObject::EndDragRectangle()
{
	SetROP2(dc, R2_XORPEN);
	::Rectangle(dc, lastX1, lastY1, lastX2, lastY2); 
	if (!xor)
		SetROP2(dc, R2_COPYPEN);
}

void DCObject::SetTextColor(double r, double g, double b)
{
	::SetTextColor(dc, RGB(r * 255, g * 255, b * 255));
}

void DCObject::printf(int x, int y, char *format, ...)
{
	char s[1024];

	va_list list;
	va_start(list, format);
	vsprintf(s, format, list);
	TextOut(dc, x, y, s, strlen(s));
	va_end(list);
}

void DCObject::Draw(DCObject &ob, int destX, int destY)
{
    Copy(ob, destX, destY); 
}

void DCObject::Draw(DCObject &ob, int destX, int destY, int w, int h, int srcX, int srcY)
{
    Copy(ob, destX, destY, w, h, srcX, srcY);
}

void DCObject::Stretch(DCObject &ob, int destX, int destY, int destW, int destH, int srcX, int srcY)
{
    StretchCopy(ob, destX, destY, destW, destH, srcX, srcY);
}

void DCObject::Stretch(DCObject &ob, int destX, int destY, int destW, int destH, int srcX, int srcY, int srcW, int srcH)
{
    StretchCopy(ob, destX, destY, destW, destH, srcX, srcY, srcW, srcH);
}

void DCObject::Copy(DCObject &ob, int destX, int destY)
{
	BitBlt(ob.dc, destX, destY, ob.Width(), ob.Height(), dc, 0, 0, SRCCOPY);
}

void DCObject::Copy(DCObject &ob, int destX, int destY, int w, int h, int srcX, int srcY)
{
	BitBlt(ob.dc, destX, destY, w, h, dc, srcX, srcY, SRCCOPY);
}

void DCObject::StretchCopy(DCObject &ob, int destX, int destY, int destW, int destH, int srcX, int srcY)
{
	StretchBlt(ob.dc, destX, destY, destW, destH, dc, srcX, srcY, Width(), Height(), SRCCOPY);
}

void DCObject::StretchCopy(DCObject &ob, int destX, int destY, int destW, int destH, int srcX, int srcY, int srcW, int srcH)
{
	StretchBlt(ob.dc, destX, destY, destW, destH, dc, srcX, srcY, srcW, srcH, SRCCOPY);
}

void DCObject::SetDrawColor(double r, double g, double b)
{
	drawColor = RGB(r * 255, g * 255, b * 255);
	CreatePen();
}

void DCObject::SetFillColor(double r, double g, double b)
{
	fillColor = RGB(r * 255, g * 255, b * 255);

	CreateBrush();
}

void DCObject::CreateBrush(void)
{
	SelectObject(dc, GetStockObject(BLACK_BRUSH));
	DeleteObject(brush);

	brush = CreateSolidBrush(fillColor);
	if (brush == 0)
		complain("Couldn't create brush for filling objects");

	if (!dc)
		return;

	if (!SelectObject(dc, brush))
		complain("Couldn't make brush for painting active");
}

void DCObject::SetPaintBackground(double r, double g, double b)
{
	SetBkColor(dc, RGB(r * 255, g * 255, b * 255));
}

void DCObject::SetBackgroundMode(bool transparent)
{
	SetBkMode(dc, (transparent == true ? TRANSPARENT : OPAQUE));
}

void DCObject::SetDrawWidth(int w)
{
	lineWidth = w;
	CreatePen();
}

void DCObject::SetDrawDotted(void)
{
	lineDotted = true;
	CreatePen();
}

void DCObject::SetDrawDashed(void)
{
	lineDashed = true;
	CreatePen();
}

void DCObject::SetDrawNormal(void)
{
	lineDotted = false;
	lineDashed = false;
	CreatePen();
}

void DCObject::SetDrawStyle(double r, double g, double b, int w, bool dotted, bool dashed)
{
	drawColor = RGB(r * 255, g * 255, b * 255);
	lineWidth = w;
	lineDotted = dotted;
	lineDashed = dashed;
	CreatePen();
}

void DCObject::CreatePen()
{
	SelectObject(dc, GetStockObject(BLACK_PEN));
	DeleteObject(pen);

	int style = 0;

	if ((lineDotted) && (lineDashed))
		style = PS_DASHDOT;
	else if (lineDotted)
		style = PS_DOT;
	else if (lineDashed)
		style = PS_DASH;
	else
		style = PS_SOLID;

	pen = ::CreatePen(style, lineWidth, drawColor);
	if (pen == NULL)
		complain("Couldn't create drawing pen");

	if (!dc)
		return;

	if (SelectObject(dc, pen) == NULL)
		complain("Couldn't make drawing pen active");
}

void DCObject::SetFont(const char *name, int size, bool bold, bool italic, bool underline, double angle, double charAngle)
{
	strcpy(fontName, name);
	if (size != -1)
	{
		fontSize = size;
		fontBold = bold;
		fontItalic = italic;
		fontUnderline = underline;
		fontAngle = int(angle * 10);
		fontCharAngle = int(charAngle * 10);
	}
	CreateFont();
}

void DCObject::SetFontName(char *name)
{
	strcpy(fontName, name);
	CreateFont();
}

void DCObject::SetFontSize(int size)
{
	fontSize = size;
	CreateFont();
}

void DCObject::SetBold(bool value)
{
	fontBold = value;
	CreateFont();
}

void DCObject::SetItalic(bool value)
{
	fontItalic = value;
	CreateFont();
}

void DCObject::SetUnderline(bool value)
{
	fontUnderline = value;
	CreateFont();
}

void DCObject::SetFontAngle(double angle)
{
	fontAngle = int(angle * 10);
	CreateFont();
}

void DCObject::SetFontCharAngle(double angle)
{
	fontCharAngle = int(angle * 10);
	CreateFont();
}

void DCObject::CreateFont()
{
	HFONT newFont;
	newFont = ::CreateFont(fontSize, 0, fontAngle, fontCharAngle, ( fontBold ? FW_BOLD : FW_NORMAL ), 
		                   fontItalic, fontUnderline, 0, 0, 0, 0, 0, 0, fontName);

	SelectObject(dc, newFont);
	DeleteObject(font);
	font = newFont;
}

int DCObject::GetTextWidth(char *str)
{
	SIZE s;

	GetTextExtentPoint32(dc, str, strlen(str), &s);
	return s.cx;
}

int DCObject::GetTextHeight(char *str)
{
	SIZE s;

	GetTextExtentPoint32(dc, str, strlen(str), &s);
	return s.cy;
}

void DCObject::SetTextAlignment(char *horizontal, char *vertical)
{
	int h, v;

	if (strncmp(horizontal, "l", 1) == 0 || strncmp(horizontal, "L", 1) == 0)
		h = TA_LEFT;
	else if (strncmp(horizontal, "r", 1) == 0 || strncmp(horizontal, "R", 1) == 0)
		h = TA_RIGHT;
	else if (strncmp(horizontal, "c", 1) == 0 || strncmp(horizontal, "C", 1) == 0)
		h = TA_CENTER;

	if (strncmp(vertical, "bo", 2) == 0 || strncmp(vertical, "Bo", 2) == 0 || 
		strncmp(vertical, "BO", 2) == 0)
		v = TA_BOTTOM;
	else if (strncmp(vertical, "ba", 2) == 0 || strncmp(vertical, "Ba", 2) == 0 ||
			 strncmp(vertical, "BA", 2) == 0)
		v = TA_BASELINE;
	else if (strncmp(vertical, "t", 1) == 0 || strncmp(vertical, "T", 1) == 0)
		v = TA_TOP;

	SetTextAlign(dc, h | v);
}