#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__

#include "AppWindow.h"
#include "window.h"
#include "limits.h"
 
class ScrollBar : public AppWindow
{
public:

	ScrollBar() { }
	ScrollBar(Window &parent, size_t xPos, size_t yPos, size_t w, size_t h, int min = 0, int max = 100, 
		      int val = -INT_MIN, int pageSize = 10, bool vertical = false);
	ScrollBar(Window &parent, size_t xPos, size_t yPos, size_t w, size_t h, int min, int max, 
		      int val, int pageSize, bool vertical, void (*changeHandler)(int Pos),
			  void (*scrollHandler)(int Pos) = NULL);
	
	void Setup(Window &parent, size_t xPos, size_t yPos, size_t w, size_t h, int min = 0, int max = 100, 
		      int val = -INT_MIN, int pageSize = 10, bool vertical = false);

	void SetScrollEvent(void (*handler)(int Pos)) { onScroll = handler; }
	void SetChangeEvent(void (*handler)(int Pos)) { onChange = handler; }

	template <class T>
	void SetScrollEvent(T *ScrollListener, void (T::*OnScroll)(int Pos))
	{ scrollListener = (EventListener *)ScrollListener; dOnScroll = (void (EventListener::*)(int))OnScroll; }
	
	template <class T>
	void SetChangeEvent(T *ChangeListener, void (T::*OnChange)(int Pos))
	{ changeListener = (EventListener *)ChangeListener; dOnChange = (void (EventListener::*)(int))OnChange; }

	void SetPosition(int newPos);
	int GetPosition(void);
	int Value(void) { return GetPosition(); }
	int Pos(void) { return GetPosition(); }
	int GetTrackPosition(void);

	void SetMax(int max);
	void SetMin(int min);
	void SetRange(int min, int max);

	int GetMax(void);
	int GetMin(void);
	void GetRange(int &max, int &min);

	void SetPageSize(int size);
	int GetPageSize(void);

protected:

	void (*onScroll)(int Pos);
	void (*onChange)(int Pos);

	EventListener *scrollListener;
	void (EventListener::*dOnScroll)(int Pos);
	EventListener *changeListener;
	void (EventListener::*dOnChange)(int Pos);

	bool OnScroll(int Pos);
	bool OnScrollChange(int Pos);

	LONG MessageHandler(HWND hWnd, UINT msg, UINT wParam, LONG lParam);
	LONG ProcessMessage(HWND hWnd, size_t msg, size_t wParam, LONG lParam);
};

#endif