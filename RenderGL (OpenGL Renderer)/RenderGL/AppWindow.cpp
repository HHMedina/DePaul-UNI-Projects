       
#include "AppWindow.h"
#include "jmisc.h"
#include <stdio.h>
 
HHOOK keyboardHook = 0;
static AppWindow *KeyboardEventHandler = NULL;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
	if (KeyboardEventHandler)
	{
		if (lParam < 0)
			KeyboardEventHandler->OnKeyUp(wParam, *(KEYCODES *)&lParam);
		else
			KeyboardEventHandler->OnKeyDown(wParam, *(KEYCODES *)&lParam);
	}
	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

AppWindow::AppWindow()
{
	Initialize();

	if (keyboardHook == 0)
	{
		keyboardHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, (HINSTANCE) NULL, GetCurrentThreadId());
//		SetKeyPreview(true);
	}
}

AppWindow::AppWindow(BOOL Visible) : Window(Visible)
{
	Initialize();
}

AppWindow::AppWindow(int XPos, int YPos, int Width, int Height, BOOL Visible, char *cName, bool AdjRect, bool border)
	: Window()
{
	Initialize();
	if (!border)
		BaseStyleEx &= !WS_EX_CLIENTEDGE;
	Setup(XPos, YPos, Width, Height, Visible, cName, AdjRect);
}

AppWindow::AppWindow(int XPos, int YPos, int Width, int Height, double bgr, double bgg, double bgb, 
					 BOOL Visible, char *cName, bool AdjRect, bool border) : Window()
{
	Initialize();
	backgroundR = bgr;
	backgroundG = bgg;
	backgroundB = bgb;
	if (!border)
		BaseStyleEx &= !WS_EX_CLIENTEDGE;
	Setup(XPos, YPos, Width, Height, Visible, cName, AdjRect);
}

AppWindow::AppWindow(Window &Parent, int XPos, int YPos, int Width, int Height, BOOL Visible, char *cName, bool AdjRect, bool border)
{
	Initialize();
	if (!border)
		BaseStyleEx &= !WS_EX_CLIENTEDGE;
	Setup(&Parent, XPos, YPos, Width, Height, Visible, cName, AdjRect);
}

AppWindow::AppWindow(Window &Parent, int XPos, int YPos, int Width, int Height, double bgr, double bgg, double bgb, 
					 BOOL Visible, char *cName, bool AdjRect, bool border) 
{
	Initialize();
	backgroundR = bgr;
	backgroundG = bgg;
	backgroundB = bgb;
	if (!border)
		BaseStyleEx &= !WS_EX_CLIENTEDGE;
	Setup(&Parent, XPos, YPos, Width, Height, Visible, cName, AdjRect);
}

AppWindow::~AppWindow()
{
	if (IsWindow(hWnd))
	{
		if (RefCount(hWnd) == 1) // Window will be deleted
		{
			for (int i = 0; i < nSubMenus; i++)
				DeleteObject(subMenu[i]);
			if (IsMenu(mainMenu))  // So delete menu if it exists
				DeleteObject(mainMenu);
		}
	}
}

void AppWindow::Initialize(void)
{
	styleExtra |= CS_OWNDC;
	BaseStyle |= WS_CLIPCHILDREN;

	close = NULL;
	sized = NULL;
	move = NULL;    
	charPressed = NULL;
	keyDown = NULL;
	keyUp = NULL;
    lButtonDown = NULL;
    lButtonUp = NULL;
    lButtonDblClick = NULL;
    lButtonDrag = NULL;
    rButtonDown = NULL;
    rButtonUp = NULL;
    rButtonDblClick = NULL;
    rButtonDrag = NULL;
    mButtonDown = NULL;
    mButtonUp = NULL;
    mButtonDblClick = NULL;
    mButtonDrag = NULL;
    mouseMove = NULL;
	paint = NULL;
	gotFocus = NULL;
	lostFocus = NULL;
	command = NULL;
	timer = NULL;

	dClose = NULL;
	dSized = NULL;
	dMove = NULL;
	dCharPressed = NULL;
	dKeyDown = NULL;
	dKeyUp = NULL;
    dLButtonDown = NULL;
    dLButtonUp = NULL;
    dLButtonDblClick = NULL;
    dLButtonDrag = NULL;
    dRButtonDown = NULL;
    dRButtonUp = NULL;
    dRButtonDblClick = NULL;
    dRButtonDrag = NULL;
    dMButtonDown = NULL;
    dMButtonUp = NULL;
    dMButtonDblClick = NULL;
    dMButtonDrag = NULL;
    dMouseMove = NULL;
	dPaint = NULL;
	dGotFocus = NULL;
	dLostFocus = NULL;
	dCommand = NULL;
	dTimer = NULL;

	closeHandler = NULL;
	sizeHandler = NULL;
	moveHandler = NULL;
	charHandler = NULL;
	keyDownHandler = NULL;
	keyUpHandler = NULL;
    lDownHandler = NULL;
    lUpHandler = NULL;
    lDblClickHandler = NULL;
    lDragHandler = NULL;
    rDownHandler = NULL;
    rUpHandler = NULL;
    rDblClickHandler = NULL;
    rDragHandler = NULL;
    mDownHandler = NULL;
    mUpHandler = NULL;
    mDblClickHandler = NULL;
    mDragHandler = NULL;
    moveHandler = NULL;
	paintHandler = NULL;
	gotFocusHandler = NULL;
	lostFocusHandler = NULL;
	timerHandler = NULL;
	
	mainMenu = NULL;
	nSubMenus = 0;
	nMenuItems = 0;
	for (int i = 0; i < MAX_SUBMENUS; i++)
		subMenuNames[i][0] = '\0';
	
	for (int i = 0; i < 100; i++)
	{
		commandHandler[i] = NULL;
		cItemHandler[i] = NULL;
		dCommandItem[i] = NULL;
	}
}

void AppWindow::SetKeyPreview(bool flag)
{
	if (flag)
		KeyboardEventHandler = this;
	else
		KeyboardEventHandler = NULL;
}

void AppWindow::AddMainMenu(void)
{
    if (!mainMenu)
    {
        mainMenu = CreateMenu();
        SetMenu(hWnd, mainMenu);
    }
}

void AppWindow::AddFileMenu(void)
{
	AddSubMenu("File");

	AppendMenu(subMenu[0], MF_STRING, ID_PRINT, "Print");
	AppendMenu(subMenu[0], MF_SEPARATOR, NULL, NULL);
    AppendMenu(subMenu[0], MF_STRING, ID_QUIT, "Exit");
}

void AppWindow::AddSubMenu(char *name)
{
	if (!mainMenu)
		complain("No menu exists for window");

	if (nSubMenus < MAX_SUBMENUS)
	{
		subMenu[nSubMenus] = CreatePopupMenu();
		AppendMenu(mainMenu, MF_POPUP, unsigned int(subMenu[nSubMenus]), name);
		strncpy(subMenuNames[nSubMenus], name, MAX_SUBMENU_NAME);
		nSubMenus++;
		Resize(width, height);
		DrawMenuBar(hWnd);
	}
	else
		complain("Too many sub-menus (Max = %d)", MAX_SUBMENUS);
}

void AppWindow::AddMenuSeparator(char *subMenuName)
{
	int subMenuNumber;
	for (subMenuNumber = 0; subMenuNumber < nSubMenus; subMenuNumber++)
	{
		if (strncmp(subMenuNames[subMenuNumber], subMenuName, MAX_SUBMENU_NAME - 1) == 0)
		{
			AppendMenu(subMenu[subMenuNumber], MF_SEPARATOR, NULL, NULL);
			Resize(width, height);
			return;
		}
	}
	complain("No submenu exists with named \"%s\"", subMenuName);
}

void AppWindow::InsertMenuSeparator(char *subMenuName, int Location)
{
	int subMenuNumber;
	for (subMenuNumber = 0; subMenuNumber < nSubMenus; subMenuNumber++)
	{
		if (strncmp(subMenuNames[subMenuNumber], subMenuName, MAX_SUBMENU_NAME - 1) == 0)
		{
			InsertMenu(subMenu[subMenuNumber], Location, MF_BYPOSITION | MF_SEPARATOR, NULL, NULL);
			Resize(width, height);
			return;
		}
	}
	complain("No submenu exists with named \"%s\"", subMenuName);
}

void AppWindow::AddMenuItem(char *name, char *subMenuName, void (*handler)(void))
{
	int subMenuNumber;
	for (subMenuNumber = 0; subMenuNumber < nSubMenus; subMenuNumber++)
	{
		if (strncmp(subMenuNames[subMenuNumber], subMenuName, MAX_SUBMENU_NAME - 1) == 0)
		{
			AppendMenu(subMenu[subMenuNumber], MF_STRING, ID_OFFSET + nMenuItems, name);
			commandHandler[nMenuItems] = handler;
			nMenuItems++;
			Resize(width, height);
			return;
		}
	}
	complain("No submenu exists with named \"%s\"", subMenuName);
}

void AppWindow::InsertMenuItem(char *name, char *subMenuName, void (*handler)(void), int Location)
{
	int subMenuNumber;
	for (subMenuNumber = 0; subMenuNumber < nSubMenus; subMenuNumber++)
	{
		if (strncmp(subMenuNames[subMenuNumber], subMenuName, MAX_SUBMENU_NAME - 1) == 0)
		{
			InsertMenu(subMenu[subMenuNumber], Location, MF_BYPOSITION | MF_STRING, ID_OFFSET + nMenuItems, name);
			commandHandler[nMenuItems] = handler;
			nMenuItems++;
			Resize(width, height);
			return;
		}
	}
	complain("No submenu exists with named \"%s\"", subMenuName);
}

void AppWindow::AddMenuItem(char *name, char *subMenuName, EventListener *hObj, 
							void (EventListener::*handler)(void))
{
	int subMenuNumber;
	for (subMenuNumber = 0; subMenuNumber < nSubMenus; subMenuNumber++)
	{
		if (strncmp(subMenuNames[subMenuNumber], subMenuName, MAX_SUBMENU_NAME - 1) == 0)
		{
			AppendMenu(subMenu[subMenuNumber], MF_STRING, ID_OFFSET + nMenuItems, name);
			cItemHandler[nMenuItems] = hObj;
			dCommandItem[nMenuItems] = handler;
			nMenuItems++;
			return;
		}
	}
	complain("No submenu exists with named \"%s\"", subMenuName);
}

void AppWindow::InsertMenuItem(char *name, char *subMenuName, EventListener *hObj, 
							void (EventListener::*handler)(void), int Location)
{
	int subMenuNumber;
	for (subMenuNumber = 0; subMenuNumber < nSubMenus; subMenuNumber++)
	{
		if (strncmp(subMenuNames[subMenuNumber], subMenuName, MAX_SUBMENU_NAME - 1) == 0)
		{
			InsertMenu(subMenu[subMenuNumber], Location, MF_BYPOSITION | MF_STRING, ID_OFFSET + nMenuItems, name);
			cItemHandler[nMenuItems] = hObj;
			dCommandItem[nMenuItems] = handler;
			nMenuItems++;
			return;
		}
	}
	complain("No submenu exists with named \"%s\"", subMenuName);
}

void AppWindow::AddMenuItem(char *name, char *subMenuName, int ID)
{
	int subMenuNumber;
	for (subMenuNumber = 0; subMenuNumber < nSubMenus; subMenuNumber++)
	{
		if (strncmp(subMenuNames[subMenuNumber], subMenuName, MAX_SUBMENU_NAME - 1) == 0)
		{
			AppendMenu(subMenu[subMenuNumber], MF_STRING, ID_OFFSET + ID, name);
			return;
		}
	}
	complain("No submenu exists with named \"%s\"", subMenuName);
}

void AppWindow::InsertMenuItem(char *name, char *subMenuName, int ID, int Location)
{
	int subMenuNumber;
	for (subMenuNumber = 0; subMenuNumber < nSubMenus; subMenuNumber++)
	{
		if (strncmp(subMenuNames[subMenuNumber], subMenuName, MAX_SUBMENU_NAME - 2) == 0)
		{
			InsertMenu(subMenu[subMenuNumber], Location, MF_BYPOSITION | MF_STRING, ID_OFFSET + ID, name);
			return;
		}
	}
	complain("No submenu exists with named \"%s\"", subMenuName);
}

void AppWindow::SetMenuItemChecked(char *name, char *subMenuName, bool checked)
{
	int subMenuNumber;
	for (subMenuNumber = 0; subMenuNumber < nSubMenus; subMenuNumber++)
	{
		if (strncmp(subMenuNames[subMenuNumber], subMenuName, MAX_SUBMENU_NAME - 1) == 0)
		{
			int count = GetMenuItemCount(subMenu[subMenuNumber]);
			for (int i = 0; i < count; i++)
			{
				char menuName[MAX_SUBMENU_NAME];
				GetMenuString(subMenu[subMenuNumber], i, menuName, MAX_SUBMENU_NAME, MF_BYPOSITION);
				if (strncmp(menuName, name, MAX_SUBMENU_NAME - 1) == 0)
				{
					if (checked)
						CheckMenuItem(subMenu[subMenuNumber], i, MF_BYPOSITION | MF_CHECKED);
					else
						CheckMenuItem(subMenu[subMenuNumber], i, MF_BYPOSITION | MF_UNCHECKED);
					return;
				}
			}
		}
	}
	complain("Couldn't fined menu item named \"%s\" in menu \"%s\"", name, subMenuName);
}

bool AppWindow::IsMenuItemChecked(char *name, char *subMenuName)
{
	int subMenuNumber;
	for (subMenuNumber = 0; subMenuNumber < nSubMenus; subMenuNumber++)
	{
		if (strncmp(subMenuNames[subMenuNumber], subMenuName, MAX_SUBMENU_NAME - 1) == 0)
		{
			int count = GetMenuItemCount(subMenu[subMenuNumber]);
			for (int i = 0; i < count; i++)
			{
				char menuName[MAX_SUBMENU_NAME];
				GetMenuString(subMenu[subMenuNumber], i, menuName, MAX_SUBMENU_NAME, MF_BYPOSITION);
				if (strncmp(menuName, name, MAX_SUBMENU_NAME - 1) == 0)
				{
					int state = GetMenuState(subMenu[subMenuNumber], i, MF_BYPOSITION);
					if ((state & MF_CHECKED) == 0)
						return false;
					else
						return true;
				}
			}
		}
	}
	complain("Couldn't fined menu item named \"%s\" in menu \"%s\"", name, subMenuName);
	return false;
}

void AppWindow::SetMenuItemEnabled(char *name, char *subMenuName, bool enabled)
{
	int subMenuNumber;
	for (subMenuNumber = 0; subMenuNumber < nSubMenus; subMenuNumber++)
	{
		if (strncmp(subMenuNames[subMenuNumber], subMenuName, MAX_SUBMENU_NAME - 1) == 0)
		{
			int count = GetMenuItemCount(subMenu[subMenuNumber]);
			for (int i = 0; i < count; i++)
			{
				char menuName[MAX_SUBMENU_NAME];
				GetMenuString(subMenu[subMenuNumber], i, menuName, MAX_SUBMENU_NAME, MF_BYPOSITION);
				if (strncmp(menuName, name, MAX_SUBMENU_NAME - 1) == 0)
				{
					int ID = GetMenuItemID(subMenu[subMenuNumber], i);
					if (enabled)
						EnableMenuItem(subMenu[subMenuNumber], i, MF_BYPOSITION | MF_ENABLED);
					else
						EnableMenuItem(subMenu[subMenuNumber], i, MF_BYPOSITION | MF_GRAYED);
					return;
				}
			}
		}
	}
	complain("Couldn't fined menu item named \"%s\" in menu \"%s\"", name, subMenuName);
}

bool AppWindow::IsMenuItemEnabled(char *name, char *subMenuName)
{
	int subMenuNumber;
	for (subMenuNumber = 0; subMenuNumber < nSubMenus; subMenuNumber++)
	{
		if (strncmp(subMenuNames[subMenuNumber], subMenuName, MAX_SUBMENU_NAME - 1) == 0)
		{
			int count = GetMenuItemCount(subMenu[subMenuNumber]);
			for (int i = 0; i < count; i++)
			{
				char menuName[MAX_SUBMENU_NAME];
				GetMenuString(subMenu[subMenuNumber], i, menuName, MAX_SUBMENU_NAME, MF_BYPOSITION);
				if (strncmp(menuName, name, MAX_SUBMENU_NAME - 1) == 0)
				{
					int state = GetMenuState(subMenu[subMenuNumber], i, MF_BYPOSITION);
					if (((state & MF_GRAYED) == 0) && (state & MF_DISABLED))
						return true;
					else
						return false;
				}
			}
		}
	}
	complain("Couldn't fined menu item named \"%s\" in menu \"%s\"", name, subMenuName);
	return false;
}

bool AppWindow::OnClose(void)
{
	if (close)
	{
		close();
		return true;
	}
	else if (dClose)
	{
		(closeHandler->*dClose)();
		return true;
	}
	else
    {
		if (!GetParent(hWnd))
		{
			Quit();
		}

		return false;
    }
}

bool AppWindow::OnSize(size_t Width, size_t Height, size_t Type)
{
	RECT rect;

	GetClientRect(hWnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	xPos = rect.top;
	yPos = rect.left;

	if (sized)
	{
		sized(int(Width), int(Height));
		return true;
	}
	else if (dSized)
	{
		(sizeHandler->*dSized)(int(Width), int(Height));
		return true;
	}
	else
		return false;
}

bool AppWindow::OnMove(size_t XPos, size_t YPos)
{
	if (move)
	{
		move(int(XPos), int(yPos));
		return true;
	}
	else if (dMove)
	{
		(moveHandler->*dMove)(int(XPos), int(YPos));
		return true;
	}
	else
		return false;
}

bool AppWindow::OnChar(size_t Value, size_t Repeat)
{
	if (charPressed)
	{
		charPressed(char(Value), int(Repeat));
		return true;
	}
	else if (dCharPressed)
	{
		(charHandler->*dCharPressed)(char(Value), int(Repeat));
		return true;
	}
	else
		return false;
}

bool AppWindow::OnKeyDown(size_t VirtKey, KEYCODES Codes)
{
	if (keyDown)
	{
		keyDown(VirtKey);
		return true;
	}
	else if (dKeyDown)
	{
		(keyDownHandler->*dKeyDown)(VirtKey);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnKeyUp(size_t VirtKey, KEYCODES Codes)
{
	if (keyUp)
	{
		keyUp(VirtKey);
		return true;
	}
	else if (dKeyUp)
	{
		(keyUpHandler->*dKeyUp)(VirtKey);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnLButtonDown(int x, int y, size_t Keys)
{
	SetCapture(hWnd);
	if (lButtonDown)
	{
		lButtonDown(x, y);
		return true;
	}
	else if (dLButtonDown)
	{
		(lDownHandler->*dLButtonDown)(x, y);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnLButtonUp(int x, int y, size_t Keys)
{
	ReleaseCapture();
	if (lButtonUp)
	{
		lButtonUp(x, y);
		return true;
	}
	else if (dLButtonUp)
	{
		(lUpHandler->*dLButtonUp)(x, y);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnLButtonDblClick(int x, int y, size_t Keys)
{
	if (lButtonDblClick)
	{
		lButtonDblClick(x, y);
		return true;
	}
	else if (dLButtonDblClick)
	{
		(lDblClickHandler->*dLButtonDblClick)(x, y);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnLButtonDrag(int x, int y, size_t Keys)
{
	if (lButtonDrag)
	{
		lButtonDrag(x, y);
		return true;
	}
	else if (dLButtonDrag)
	{
		(lDragHandler->*dLButtonDrag)(x, y);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnRButtonDown(int x, int y, size_t Keys)
{
	SetCapture(hWnd);
	if (rButtonDown)
	{
		rButtonDown(x, y);
		return true;
	}
	else if (dRButtonDown)
	{
		(rDownHandler->*dRButtonDown)(x, y);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnRButtonUp(int x, int y, size_t Keys)
{
	ReleaseCapture();
	if (rButtonUp)
	{
		rButtonUp(x, y);
		return true;
	}
	else if (dRButtonUp)
	{
		(rUpHandler->*dRButtonUp)(x, y);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnRButtonDblClick(int x, int y, size_t Keys)
{
	if (rButtonDblClick)
	{
		rButtonDblClick(x, y);
		return true;
	}
	else if (dRButtonDblClick)
	{
		(rDblClickHandler->*dRButtonDblClick)(x, y);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnRButtonDrag(int x, int y, size_t Keys)
{
	if (rButtonDrag)
	{
		lButtonDrag(x, y);
		return true;
	}
	else if (dRButtonDrag)
	{
		(rDragHandler->*dRButtonDrag)(x, y);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnMButtonDown(int x, int y, size_t Keys)
{
	SetCapture(hWnd);
	if (mButtonDown)
	{
		mButtonDown(x, y);
		return true;
	}
	else if (dMButtonDown)
	{
		(mDownHandler->*dMButtonDown)(x, y);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnMButtonUp(int x, int y, size_t Keys)
{
	ReleaseCapture();
	if (mButtonUp)
	{
		mButtonUp(x, y);
		return true;
	}
	else if (dMButtonUp)
	{
		(mUpHandler->*dMButtonUp)(x, y);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnMButtonDblClick(int x, int y, size_t Keys)
{
	if (mButtonDblClick)
	{
		mButtonDblClick(x, y);
		return true;
	}
	else if (dMButtonDblClick)
	{
		(mDblClickHandler->*dMButtonDblClick)(x, y);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnMButtonDrag(int x, int y, size_t Keys)
{
	if (mButtonDrag)
	{
		mButtonDrag(x, y);
		return true;
	}
	else if (dMButtonDrag)
	{
		(mDragHandler->*dMButtonDrag)(x, y);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnMouseMove(int x, int y, size_t Keys)
{
	if (mouseMove)
	{
		mouseMove(x, y);
		return true;
	}
	else if (dMouseMove)
	{
		(mMoveHandler->*dMouseMove)(x, y);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnMouseActivate(HWND hTop, size_t HitTest)
{
    return false;
}

bool AppWindow::OnPaint(intRect rect)
{
	if (paint)
	{
		paint(rect);
		return true;
	}
	else if (dPaint)
	{
		(paintHandler->*dPaint)(rect);
		return true;
	}
	else
		return false;
}

bool AppWindow::OnSetFocus(HWND hPrev)
{
	if (gotFocus)
	{
		gotFocus();
		return true;
	}
	else if (dGotFocus)
	{
		(gotFocusHandler->*dGotFocus)();
		return true;
	}
	else
		return false;
}

bool AppWindow::OnLoseFocus(HWND hNext)
{
	if (lostFocus)
	{
		lostFocus();
		return true;
	}
	else if (dLostFocus)
	{
		(lostFocusHandler->*dLostFocus)();
		return true;
	}
	else
		return false;
}

bool AppWindow::OnCommand(HWND hControl, size_t Id, size_t Code)
{
	if (command)
	{
		command(Id);
		return true;
	}
	else if (dCommand)
	{
		(cHandler->*dCommand)(Id - ID_OFFSET);
		return true;
	}
	else
	{
		if ((Id >= ID_OFFSET) && (Id < ID_OFFSET + MAX_COMMAND_HANDLERS))
		{
			Id -= ID_OFFSET;
			if (commandHandler[Id])
			{
				commandHandler[Id]();
				return true;
			}
			else if (dCommandItem[Id])
			{
				((cItemHandler[Id])->*dCommandItem[Id])();
				return true;
			}
			Id += ID_OFFSET;
		}
	}
	return Window::OnCommand(hControl, Id, Code);
}

bool AppWindow::OnTimer(size_t timerId)
{
	if (timer)
	{
		timer(timerId);
		return true;
	}
	else if (dTimer)
	{
		(timerHandler->*dTimer)(timerId); 
		return true;
	}
	else
		return false;
}

