#ifndef __APPWINDOW_H__
#define __APPWINDOW_H__

#include "Window.h"

#define MAX_SUBMENUS 10
#define MAX_SUBMENU_NAME 50
 
class AppWindow : public Window
{
public:

	AppWindow();
	AppWindow(BOOL Visible);
	AppWindow(int x, int y, int width, int height, BOOL Visible = TRUE, char *cName = NULL, 
		bool AdjRect = true, bool border = true);
	AppWindow(int x, int y, int width, int height, double bgr, double bgg, double bgb, 
		BOOL Visible = TRUE, char *cName = NULL, bool AdjRect = true, bool border = true);
	AppWindow(Window &parent, int x, int y, int width, int height, BOOL Visible = TRUE, char *cName = NULL, 
		bool AdjRect = true, bool border = true);
	AppWindow(Window &parent, int x, int y, int width, int height, double bgr, double bgg, double bgb, 
		BOOL Visible = TRUE, char *cName = NULL, bool AdjRect = true, bool border = true);

	~AppWindow();

	void Initialize(void);

	void SetKeyPreview(bool flag);

	virtual void SetCloseEvent(void (*handler)(void))
	{ close = handler; }
	virtual void SetSizeEvent(void (*handler)(size_t w, size_t h))
	{ sized = handler; }
	virtual void SetMoveEvent(void (*handler)(size_t xPos, size_t yPos))
	{ move = handler; }
	virtual void SetCharPressedEvent(void (*handler)(char c, size_t repeat))
	{ charPressed = handler; }
	virtual void SetKeyDownEvent(void (*handler)(size_t key))
	{ keyDown = handler; }
	virtual void SetKeyUpEvent(void (*handler)(size_t key))
	{ keyUp = handler; }
	virtual void SetLButtonDownEvent(void (*handler)(int x, int y))
    { lButtonDown = handler; }
    virtual void SetLButtonUpEvent(void (*handler)(int x, int y))
    { lButtonUp = handler; }
    virtual void SetLButtonDblClickEvent(void (*handler)(int x, int y))
    { lButtonDblClick = handler; }
    virtual void SetLButtonDragEvent(void (*handler)(int x, int y))
    { lButtonDrag = handler; }
    virtual void SetRButtonDownEvent(void (*handler)(int x, int y))
    { rButtonDown = handler; }
    virtual void SetRButtonUpEvent(void (*handler)(int x, int y))
    { rButtonUp = handler; }
    virtual void SetRButtonDblClickEvent(void (*handler)(int x, int y))
    { rButtonDblClick = handler; }
    virtual void SetRButtonDragEvent(void (*handler)(int x, int y))
    { rButtonDrag = handler; }
    virtual void SetMButtonDownEvent(void (*handler)(int x, int y))
    { mButtonDrag = handler; }
    virtual void SetMButtonUpEvent(void (*handler)(int x, int y))
    { mButtonUp = handler; }
    virtual void SetMButtonDblClickEvent(void (*handler)(int x, int y))
    { mButtonDblClick = handler; }
    virtual void SetMButtonDragEvent(void (*handler)(int x, int y))
    { mButtonDrag = handler; }
    virtual void SetMouseMoveEvent(void (*handler)(int x, int y))
    { mouseMove = handler; }
	virtual void SetPaintEvent(void (*handler)(intRect rect))
	{ paint = handler; }
	virtual void SetGotFocusEvent(void (*handler)(void))
	{ gotFocus = handler; }
	virtual void SetLostFocusEvent(void (*handler)(void))
	{ lostFocus = handler; }
	virtual void SetCommandEvent(void (*handler)(size_t id))
	{ command = handler; }
	virtual void SetTimerEvent(void (*handler)(size_t id))
	{ timer = handler; }

	template <class T> void SetCloseEvent(EventListener *hObj, void (T::*handler)(void))
	{ closeHandler = hObj, dClose = (void (EventListener::*)(void))handler; }
	template <class T> void SetSizeEvent(EventListener *hObj, void (T::*handler)(size_t w, size_t h))
	{ sizeHandler = hObj, dSized = (void (EventListener::*)(size_t, size_t))handler; }
	template <class T> void SetMoveEvent(EventListener *hObj, void (T::*handler)(size_t xPos, size_t yPos))
	{ moveHandler = hObj, dMove = (void (EventListener::*)(size_t, size_t))handler; }
	template <class T> void SetCharPressedEvent(EventListener *hObj, void (T::*handler)(char c, size_t repeat))
	{ charHandler = hObj, dCharPressed = (void (EventListener::*)(char, size_t))handler; }
	template <class T> void SetKeyDownEvent(EventListener *hObj, void (T::*handler)(size_t key))
	{ keyDownHandler = hObj, dKeyDown = (void (EventListener::*)(size_t))handler; }
	template <class T> void SetKeyUpEvent(EventListener *hObj, void (T::*handler)(size_t key))
	{ keyUpHandler = hObj, dKeyUp = (void (EventListener::*)(size_t))handler; }
	template <class T> void SetLButtonDownEvent(EventListener *hObj, void (T::*handler)(int x, int y))
    { lDownHandler = hObj, dLButtonDown = (void (EventListener::*)(int, int))handler; }
    template <class T> void SetLButtonUpEvent(EventListener *hObj, void (T::*handler)(int x, int y))
    { lUpHandler = hObj, dLButtonUp = (void (EventListener::*)(int, int))handler; }
    template <class T> void SetLButtonDblClickEvent(EventListener *hObj, void (T::*handler)(int x, int y))
    { lDblClickHandler = hObj, dLButtonDblClick = (void (EventListener::*)(int, int))handler; }
    template <class T> void SetLButtonDragEvent(EventListener *hObj, void (T::*handler)(int x, int y))
    { lDragHandler = hObj, dLButtonDrag = (void (EventListener::*)(int, int))handler; }
    template <class T> void SetRButtonDownEvent(EventListener *hObj, void (T::*handler)(int x, int y))
    { rDownHandler = hObj, dRButtonDown = (void (EventListener::*)(int, int))handler; }
    template <class T> void SetRButtonUpEvent(EventListener *hObj, void (T::*handler)(int x, int y))
    { rUpHandler = hObj, dRButtonUp = (void (EventListener::*)(int, int))handler; }
    template <class T> void SetRButtonDblClickEvent(EventListener *hObj, void (T::*handler)(int x, int y))
    { rDblClickHandler = hObj, dRButtonDblClick = (void (EventListener::*)(int, int))handler; }
    template <class T> void SetRButtonDragEvent(EventListener *hObj, void (T::*handler)(int x, int y))
    { rDragHandler = hObj, dRButtonDrag = (void (EventListener::*)(int, int))handler; }
    template <class T> void SetMButtonDownEvent(EventListener *hObj, void (T::*handler)(int x, int y))
    { mDownHandler = hObj, dMButtonDrag = (void (EventListener::*)(int, int))handler; }
    template <class T> void SetMButtonUpEvent(EventListener *hObj, void (T::*handler)(int x, int y))
    { mUpHandler = hObj, dMButtonUp = (void (EventListener::*)(int, int))handler; }
    template <class T> void SetMButtonDblClickEvent(EventListener *hObj, void (T::*handler)(int x, int y))
    { mDblClickHandler = hObj, dMButtonDblClick = (void (EventListener::*)(int, int))handler; }
    template <class T> void SetMButtonDragEvent(EventListener *hObj, void (T::*handler)(int x, int y))
    { mDragHandler = hObj, dMButtonDrag = (void (EventListener::*)(int, int))handler; }
    template <class T> void SetMouseMoveEvent(EventListener *hObj, void (T::*handler)(int x, int y))
    { mMoveHandler = hObj, dMouseMove = (void (EventListener::*)(int, int))handler; }
	template <class T> void SetPaintEvent(EventListener *hObj, void (T::*handler)(intRect rect))
	{ paintHandler = hObj, dPaint = (void (EventListener::*)(intRect))handler; }
	template <class T> void SetGotFocusEvent(EventListener *hObj, void (T::*handler)(void))
	{ gotFocusHandler = hObj, dGotFocus = (void (EventListener::*)(void))handler; }
	template <class T> void SetLostFocusEvent(EventListener *hObj, void (T::*handler)(void))
	{ lostFocusHandler = hObj, dLostFocus = (void (EventListener::*)(void))handler; }
	template <class T> void SetCommandEvent(EventListener *hObj, void (T::*handler)(size_t id))
	{ cHandler = hObj, dCommand = (void (EventListener::*)(size_t))handler; }
	template <class T> void SetTimerEvent(EventListener *hObj, void (T::*handler)(size_t id))
	{ timerHandler = hObj, dTimer = (void (EventListener::*)(size_t))handler; }

    virtual void AddMainMenu(void);
	virtual void AddSubMenu(char *name);
	virtual void AddFileMenu(void);
	virtual void AddMenuSeparator(char *subMenuName);
	virtual void InsertMenuSeparator(char *subMenuName, int Location);
	virtual void AddMenuItem(char *name, char *subMenuName, void (*handler)(void));	
	virtual void AddMenuItem(char *name, char *subMenuName, EventListener *hObj,
							 void (EventListener::*handler)(void));

	virtual void InsertMenuItem(char *name, char *subMenuName, void (*handler)(void), int Location);
	virtual void InsertMenuItem(char *name, char *subMenuName, EventListener *hObj, 
							void (EventListener::*handler)(void), int Location);

	template <class T>
	void AddMenuItem(char *name, char *subMenuName, T *hObj, void (T::*handler)(void))
	{ 	AddMenuItem(name, subMenuName, hObj, (void (EventListener::*)(void))(handler)); }

	template <class T>
	void InsertMenuItem(char *name, char *subMenuName, T *hObj, void (T::*handler)(void), int Location)
	{ 	InsertMenuItem(name, subMenuName, hObj, (void (EventListener::*)(void))(handler), Location); }

	// The following may be used if you will be handling command events manually.
	virtual void AddMenuItem(char *name, char *subMenuName, int ID);
	virtual void InsertMenuItem(char *name, char *subMenuName, int ID, int Location);

	virtual void SetMenuItemChecked(char *name, char *subMenuName, bool checked);
	virtual bool IsMenuItemChecked(char *name, char *subMenuName);
	virtual void SetMenuItemEnabled(char *name, char *subMenuName, bool enabled);
	virtual bool IsMenuItemEnabled(char *name, char *subMenuName);

protected:

    // HMENU mainMenu;
    HMENU subMenu[MAX_SUBMENUS];
	char subMenuNames[MAX_SUBMENUS][MAX_SUBMENU_NAME + 1];
	int nSubMenus;
	int nMenuItems;

	virtual bool OnClose(void);
	virtual bool OnSize(size_t Width, size_t Height, size_t Type);
	virtual bool OnMove(size_t XPos, size_t YPos);
	virtual bool OnChar(size_t Value, size_t Repeat);
	virtual bool OnKeyDown(size_t VirtKey, KEYCODES Codes);
	virtual bool OnKeyUp(size_t VirtKey, KEYCODES Codes);
	virtual bool OnLButtonDown(int x, int y, size_t Keys);
	virtual bool OnLButtonUp(int x, int y, size_t Keys);
	virtual bool OnLButtonDblClick(int x, int y, size_t Keys);
	virtual bool OnLButtonDrag(int x, int y, size_t Keys);
	virtual bool OnRButtonDown(int x, int y, size_t Keys);
	virtual bool OnRButtonUp(int x, int y, size_t Keys);
	virtual bool OnRButtonDblClick(int x, int y, size_t Keys);
	virtual bool OnRButtonDrag(int x, int y, size_t Keys);
	virtual bool OnMButtonDown(int x, int y, size_t Keys);
	virtual bool OnMButtonUp(int x, int y, size_t Keys);
	virtual bool OnMButtonDblClick(int x, int y, size_t Keys);
	virtual bool OnMButtonDrag(int x, int y, size_t Keys);
	virtual bool OnMouseMove(int x, int y, size_t Keys);
	virtual bool OnMouseActivate(HWND hTop, size_t HitTest);
	virtual bool OnPaint(intRect rect);
	virtual bool OnSetFocus(HWND hPrev);
	virtual bool OnLoseFocus(HWND hNext);
	virtual bool OnCommand(HWND hControl, size_t Id, size_t Code);
	virtual bool OnTimer(size_t timerId);

	void (*close)(void);
	void (*sized)(size_t w, size_t h);
	void (*move)(size_t xPos, size_t yPos);
	void (*charPressed)(char c, size_t repeat);
	void (*keyDown)(size_t key);
	void (*keyUp)(size_t key);
    void (*lButtonDown)(int x, int y);
    void (*lButtonUp)(int x, int y);
    void (*lButtonDblClick)(int x, int y);
    void (*lButtonDrag)(int x, int y);
    void (*rButtonDown)(int x, int y);
    void (*rButtonUp)(int x, int y);
    void (*rButtonDblClick)(int x, int y);
    void (*rButtonDrag)(int x, int y);
    void (*mButtonDown)(int x, int y);
    void (*mButtonUp)(int x, int y);
    void (*mButtonDblClick)(int x, int y);
    void (*mButtonDrag)(int x, int y);
    void (*mouseMove)(int x, int y);
	void (*paint)(intRect rect);
	void (*gotFocus)(void);
	void (*lostFocus)(void);
	void (*timer)(size_t id);
	void (*command)(size_t id);
	void (*scrollChange)(size_t pos);
	void (*scrollScroll)(size_t pos);
	void (*commandHandler[MAX_COMMAND_HANDLERS])(void);


	EventListener *closeHandler;
	void (EventListener::*dClose)(void);
	EventListener *sizeHandler;
	void (EventListener::*dSized)(size_t w, size_t h);
	EventListener *moveHandler;
	void (EventListener::*dMove)(size_t xPos, size_t yPos);
	EventListener *charHandler;
	void (EventListener::*dCharPressed)(char c, size_t repeat);
	EventListener *keyDownHandler;
	void (EventListener::*dKeyDown)(size_t key);
	EventListener *keyUpHandler;
	void (EventListener::*dKeyUp)(size_t key);
	EventListener *lDownHandler;
    void (EventListener::*dLButtonDown)(int x, int y);
	EventListener *lUpHandler;
    void (EventListener::*dLButtonUp)(int x, int y);
	EventListener *lDblClickHandler;
    void (EventListener::*dLButtonDblClick)(int x, int y);
	EventListener *lDragHandler;
    void (EventListener::*dLButtonDrag)(int x, int y);
	EventListener *rDownHandler;
    void (EventListener::*dRButtonDown)(int x, int y);
	EventListener *rUpHandler;
    void (EventListener::*dRButtonUp)(int x, int y);
	EventListener *rDblClickHandler;
    void (EventListener::*dRButtonDblClick)(int x, int y);
	EventListener *rDragHandler;
    void (EventListener::*dRButtonDrag)(int x, int y);
	EventListener *mDownHandler;
    void (EventListener::*dMButtonDown)(int x, int y);
	EventListener *mUpHandler;
    void (EventListener::*dMButtonUp)(int x, int y);
	EventListener *mDblClickHandler;
    void (EventListener::*dMButtonDblClick)(int x, int y);
	EventListener *mDragHandler;
    void (EventListener::*dMButtonDrag)(int x, int y);
	EventListener *mMoveHandler;
    void (EventListener::*dMouseMove)(int x, int y);
	EventListener *paintHandler;
	void (EventListener::*dPaint)(intRect rect);
	EventListener *gotFocusHandler;
	void (EventListener::*dGotFocus)(void);
	EventListener *lostFocusHandler;
	void (EventListener::*dLostFocus)(void);
	EventListener *cHandler;
	void (EventListener::*dCommand)(size_t id);
	EventListener *timerHandler;
	void (EventListener::*dTimer)(size_t id);
	EventListener *sChangeHandler;
	void (EventListener::*dScrollChange)(size_t pos);
	EventListener *sScrollHandler;
	void (EventListener::*dScrollScroll)(size_t pos);
	EventListener *cItemHandler[MAX_COMMAND_HANDLERS];
	void (EventListener::*dCommandItem[MAX_COMMAND_HANDLERS])(void);

	friend LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
};

#endif