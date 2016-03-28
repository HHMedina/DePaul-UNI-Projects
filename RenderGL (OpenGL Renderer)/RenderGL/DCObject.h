 
#ifndef __DCOBJECT_H__
#define __DCOBJECT_H__

#include <windows.h>

typedef RECT intRect;
typedef POINT intPoint;

class Bitmap;
 
class DCObject
{
public:

	DCObject();
	virtual ~DCObject();

    HDC DC(void) { return dc; }

// Drawing and painting functions
	virtual void MoveTo(int x, int y) { MoveToEx(dc, x, y, NULL); }

	virtual void LineTo(int x, int y) { ::LineTo(dc, x, y); }
	virtual void Line(int x1, int y1, int x2, int y2) { MoveToEx(dc, x1, y1, NULL); ::LineTo(dc, x2, y2); }

// Lines is an alias for LineStrip
	virtual void Lines(int *x, int *y, int nPoints) { LineStrip(x, y, nPoints); }
	virtual void Lines(intPoint *points, int nPoints) { LineStrip(points, nPoints); }
	virtual void LineStrip(int *x, int *y, int nPoints);
	virtual void LineStrip(intPoint *points, int nPoints) { Polyline(dc, points, nPoints); }
	virtual void LineStripTo(intPoint *points, int nPoints) { PolylineTo(dc, points, nPoints); }

	virtual void PolyLineStrip(intPoint *points, unsigned long *pointsInEach, int totalPoints)
		{ PolyPolyline(dc, points, pointsInEach, totalPoints); }

	virtual void Bezier(intPoint *points, int nPoints) { PolyBezier(dc, points, nPoints); }
	virtual void Bezier(int *x, int *y, int nPoints);
	virtual void BezierTo(intPoint *points, int nPoints) { PolyBezierTo(dc, points, nPoints); }

	virtual void ArcTo(int x1, int y1, int x2, int y2, double angle1, double angle2);
	virtual void Arc(int x1, int y1, int x2, int y2, double angle1, double angle2);
	virtual void CircleArcTo(int centerX, int centerY, int radius, double angle1, double angle2);
	virtual void CircleArc(int centerX, int centerY, int radius, double angle1, double angle2);

	virtual void Ellipse(int x1, int y1, int x2, int y2);
	virtual void Circle(int x, int y, int radius);
	virtual void Rectangle(int x1, int y1, int x2, int y2);
	virtual void Rectangle(const intRect& rect);
	virtual void RoundRectangle(int x1, int y1, int x2, int y2, int width, int height);
	virtual void Polygon(intPoint *points, int nPoints);
	virtual void PolyPolygon(intPoint *points, int *pointsInEach, int nPoints);

	virtual void FillEllipse(int x1, int y1, int x2, int y2);
	virtual void FillCircle(int x, int y, int radius);
	virtual void FillRectangle(int x1, int y1, int x2, int y2) { ::Rectangle(dc, x1, y1, x2, y2); }
	virtual void FillRectangle(const intRect& rect) 
		{ ::Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom); }
	virtual void FillRoundRectangle(int x1, int y1, int x2, int y2, int width, int height)
		{ ::RoundRect(dc, x1, y1, x2, y2, width, height); }
	virtual void FillPolygon(intPoint *points, int nPoints) { ::Polygon(dc, points, nPoints); }
	virtual void FillPolyPolygon(intPoint *points, int *pointsInEach, int nPoints)
		{ ::PolyPolygon(dc, points, pointsInEach, nPoints); }

	virtual void BeginDragRectangle(int x, int y);
	virtual void DragRectangle(int x, int y);
	virtual void EndDragRectangle();

	virtual void Point(intPoint point) { SetPixelV(dc, point.x, point.y, drawColor); }
	virtual void Point(int x, int y) { SetPixelV(dc, x, y, drawColor); }
	virtual void Point(intPoint point, double r, double g, double b) 
		{ SetPixelV(dc, point.x, point.y, RGB(r * 255, g * 255, b * 255)); }
	virtual void Point(int x, int y, double r, double g, double b) 
		{ SetPixelV(dc, x, y, RGB(r * 255, g * 255, b * 255)); }
	virtual void Point(intPoint point, COLORREF color) 
		{ SetPixelV(dc, point.x, point.y, color); }
	virtual void Point(int x, int y, COLORREF color) 
		{ SetPixelV(dc, x, y, color); }

	virtual void printf(int x, int y, char *format, ...);

	virtual COLORREF GetPoint(int x, int y) { return GetPixel(dc, x, y); }

// Drawing Attribute Functions
// All Colors values should be between 0 and 1

	virtual void SetDrawColor(double r, double g, double b);
	virtual void SetColor(double r, double g, double b) { SetDrawColor(r, g, b); }

	virtual void SetFillColor(double r, double g, double b);
	virtual void SetPaintBackground(double r, double g, double b);
	void SetTextBackground(double r, double g, double b) { SetPaintBackground(r, g, b); }
	virtual void SetBackgroundMode(bool transparent = false);
	virtual void SetTextColor(double r, double g, double b);

	virtual void SetDrawWidth(int width);
	virtual void SetDrawDotted(void);
	virtual void SetDrawDashed(void);
	virtual void SetDrawNormal(void);
	virtual void SetDrawStyle(double r, double g, double b, int width, bool dotted = FALSE, 
							  bool dashed = FALSE);
	virtual void SetXor(bool value) { xor = value; if (xor) SetROP2(dc, R2_XORPEN); else SetROP2(dc, R2_COPYPEN); }


	virtual void SetFont(const char *name, int size = -1, bool bold = false, bool italic = false, bool underline = false, 
						 double angle = 0.0, double charAngle = 0.0);
	virtual void SetFontName(char *name);
	virtual void SetFontSize(int size);
	virtual void SetBold(bool value);
	virtual void SetItalic(bool value);
	virtual void SetUnderline(bool value);
	virtual void SetFontAngle(double angle);
	virtual void SetFontCharAngle(double angle);

	void GetFontName(char *buffer) { strcpy(buffer, fontName); }
	int GetFontSize(void) { return fontSize; }
	bool GetBold(void) { return fontBold; }
	bool GetItalic(void) { return fontItalic; } 
	bool GetUnderline(void) { return fontUnderline; }
	double GetFontAngle(void) { return fontAngle / 10.0; }
	double GetFontCharAngle(void) { return fontCharAngle / 10.0; }

    virtual void SetEraseColor(COLORREF newColor);
	virtual void SetEraseColor(double r, double g, double b);

	virtual int GetTextWidth(char *str);
	virtual int GetTextHeight(char *str);
	virtual void SetTextAlignment(char *horizontal, char *vertical);

	virtual void Erase(void);
	COLORREF GetEraseColor(void) { return eraseColor; }

	virtual int Width(void) { return 0; }
	virtual int Height(void) { return 0; }

	virtual void Copy(DCObject &ob, int destX, int destY);
	virtual void Copy(DCObject &ob, int destX, int destY, int w, int h, int srcX, int srcY);
	virtual void StretchCopy(DCObject &ob, int destX, int destY, int destW, int destH, int srcX, int srcY);
	virtual void StretchCopy(DCObject &ob, int destX, int destY, int destW, int destH, int srcX, int srcY, int srcW, int srcH);

   	virtual void Draw(DCObject &ob, int destX, int destY);
	virtual void Draw(DCObject &ob, int destX, int destY, int w, int h, int srcX, int srcY);
	virtual void Stretch(DCObject &ob, int destX, int destY, int destW, int destH, int srcX, int srcY);
	virtual void Stretch(DCObject &ob, int destX, int destY, int destW, int destH, int srcX, int srcY, int srcW, int srcH);

	virtual void Update(void) { };

protected:

	HDC dc;

	COLORREF drawColor;
	COLORREF fillColor;
	COLORREF eraseColor;

	int lineWidth;
	bool lineDotted;
	bool lineDashed;
	bool xor;
	HPEN pen;
	HBRUSH brush;
    HPEN erasePen;
    HBRUSH eraseBrush;

	virtual void CreatePen(void);
	virtual void CreateBrush(void);

	char fontName[LF_FACESIZE];
	int fontSize;
	bool fontBold;
	bool fontItalic;
	bool fontUnderline;
	int fontAngle;
	int fontCharAngle;
	HFONT font;

	virtual void CreateFont();

	int figureCorrection; // 0 = normal, 1 = Make rectangles, ellipses and circles include the ending corrdinates

	int lastX1, lastY1, lastX2, lastY2;
};

#endif