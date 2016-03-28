
#include <time.h>
#include "resource.h"
#include "Application.h"
#include "AVIFile.h"

OpenGLWin *MainWinRef;

Application::Application()
{ 
	Container.Setup(0, 0, 400, 220, false);
	Container.SetSizeEvent(this, &Application::ResizeWindow);

	SetupControls();

	MainWin.Setup(Container, 0, 0, 400, 200);
	MainWin.OpenGLWinSetup();
	Container.SetIcon(IDI_RENDERGL);

	MainWin.SetLButtonDownEvent(this, &Application::InitTransform3D);
	MainWin.SetLButtonDragEvent(this, &Application::Rotate3D);
	MainWin.SetPaintEvent(this, &Application::UpdateWindow3D);
	MainWin.SetSizeEvent(this, &Application::ResizeWindow3D);
	
	Container.AddMainMenu();
	Container.AddFileMenu();
	Container.InsertMenuItem("Open RGL File\tCtrl+O", "File", this, &Application::OnOpen, 0);
	Container.InsertMenuItem("Save Image\tCtrl+S", "File", this, &Application::OnSave, 1);
	Container.InsertMenuSeparator("File", 2);
	Container.AddSubMenu("Animation");
	Container.AddMenuItem("Next Scene\tSpace", "Animation", this, &Application::OnNext);
	Container.AddMenuItem("Render Animation", "Animation", this, &Application::OnRender);
	Container.AddSubMenu("Mouse");
	Container.AddMenuItem("Rotate", "Mouse", this, &Application::OnRotate);
	Container.AddMenuItem("Translate", "Mouse", this, &Application::OnTranslate);
	Container.AddMenuItem("In/Out", "Mouse", this, &Application::OnInOut);
	Container.AddSubMenu("Help");
	Container.AddMenuItem("About", "Help", this, &Application::OnAbout);

	Container.SetMenuItemChecked("Rotate", "Mouse", true);
	Container.SetText("Object Oriented Rendering with OpenGL");

	Container.SetKeyDownEvent(this, &Application::OnKeyPress);
	Container.SetKeyUpEvent(this, &Application::OnKeyReleased);

	Container.Show();

	MainWinRef = &MainWin;

	mainScene.setup(*this, MainWin);

	selectedShape = NULL;

	lastX = 0;
	lastY = 0;
	ctrlPressed = false;
}
 
void Application::SetupControls(void)
{
	int x;

	Play.Setup(Container, 0, 200, 20, 20, "Play.bmp");
	Play.SetClickEvent(this, &Application::PlayButtonPressed);

	x = Play.XPos() + Play.Width();
	Pause.Setup(Container, x, 200, 20, 20, "Pause.bmp");
	Pause.SetClickEvent(this, &Application::PauseButtonPressed);

	x = Pause.XPos() + Pause.Width() + 1;
	lblTime.Setup(Container, x, 200, 99, 20, " t = 0.000", 0, 0, 0, Label::Center, Label::Sunken);

	x = lblTime.XPos() + lblTime.Width() + 2;

	scrTime.Setup(Container, x, 200, Container.TotalWidth() - x - 8, 20, 0, 1000, 0, 100);
	SetAnimationLength(3.0);
	SetFramesPerSecond(30);

	scrTime.SetChangeEvent(this, &Application::ScrollTime);
	scrTime.SetScrollEvent(this, &Application::ScrollTime);
}

void Application::UpdateWindow3D(intRect UpdateRect)
{
	mainScene.Display();
	MainWin.SwapBuffers();
}

void Application::ResizeWindow(size_t w, size_t h)
{
	MainWin.Move(0, 0, Container.Width() - 4, Container.Height() - 24, true);
	Play.Move(Play.XPos(), h - 20);
	Pause.Move(Pause.XPos(), h - 20);
	lblTime.Move(lblTime.XPos(), h - 20);
	scrTime.Move(scrTime.XPos(), h - 20, w - scrTime.XPos(), scrTime.Height());
}

void Application::ResizeWindow3D(size_t w, size_t h)
{
	mainScene.Display();
}

void Application::Resize(int w, int h)
{
	Container.Resize(w, h+20);
}

void Application::OnRotate(void)
{
	Container.SetMenuItemChecked("Rotate", "Mouse", true);
	Container.SetMenuItemChecked("Translate", "Mouse", false);
	Container.SetMenuItemChecked("In/Out", "Mouse", false);
}

void Application::OnTranslate(void)
{
	Container.SetMenuItemChecked("Rotate", "Mouse", false);
	Container.SetMenuItemChecked("Translate", "Mouse", true);
	Container.SetMenuItemChecked("In/Out", "Mouse", false);
}

void Application::OnInOut(void)
{
	Container.SetMenuItemChecked("Rotate", "Mouse", false);
	Container.SetMenuItemChecked("Translate", "Mouse", false);
	Container.SetMenuItemChecked("In/Out", "Mouse", true);
}

void Application::InitTransform3D(int x, int y)
{
	lastX = x;
	lastY = y;
}

void Application::Rotate3D(int x, int y)
{
	float dx = float(x - lastX) / MainWin.Width();
	float dy = float(y - lastY) / MainWin.Width();
	lastX = x; lastY = y;				// Record the point for the next time

	// We get an Invalid Window Handle error here, but it doesn't affect the 
	// operation of the application so we clear the error here.  
	// 
	// Need to investigate.
    SetLastError(0);

	if (Container.IsMenuItemChecked("Rotate", "Mouse"))
		mainScene.Rotate(dx, dy);
	else if (Container.IsMenuItemChecked("Translate", "Mouse"))
		mainScene.Translate(dx, dy);
	else if (Container.IsMenuItemChecked("In/Out", "Mouse"))
		mainScene.InOut(dx, dy);
	mainScene.Display();
}

void Application::SetAnimationLength(double newLength)
{
	int pageSize = int(animationLength * framesPerSecond) / 10;

	animationLength = newLength;
	scrTime.SetPageSize(pageSize);
	scrTime.SetMax(int((animationLength) * framesPerSecond));
	PauseButtonPressed();
}

void Application::SetFramesPerSecond(int newCount)
{
	framesPerSecond = newCount;
	
	int pageSize = int(animationLength * framesPerSecond) / 10;

	scrTime.SetPageSize(pageSize);
	scrTime.SetMax(int(animationLength * framesPerSecond));
	PauseButtonPressed();
}

void Application::ScrollTime(int Pos)
{
	lblTime.SetText(" t = %0.3lf", (float)Pos / framesPerSecond);
	mainScene.SetTime((float)Pos / framesPerSecond);
	mainScene.Display();
}

void Application::PlayButtonPressed(void)
{
	if (scrTime.GetPosition() > scrTime.GetMax() - 1)
	{
		scrTime.SetPosition(0);
		ScrollTime(0);
	}
	Container.SetTimerEvent(this, &Application::AnimationTimer);
	Container.SetTimer(1, 1000 / framesPerSecond);
}

void Application::PauseButtonPressed(void)
{
	Container.KillTimer(1);
}

void Application::AnimationTimer(size_t id)
{
	int inc = 1;
	int p = (scrTime.GetPosition() + inc);
	if (p > scrTime.GetMax()) 
	{
		PauseButtonPressed();
		return;
	}

	scrTime.SetPosition(p);
	ScrollTime(p);
}

void Application::OnRender(void)
{
	OPENFILENAME	FileDialogData;
	TCHAR			file[MAX_PATH] = "\0";
	TCHAR			fileTitle[MAX_PATH] = "\0";
	char			*filter = "AVI - Windows Audio Video Interleaved Format\0*.avi\0\0";

	strcpy (file, "");
	strcpy (fileTitle, "");

	FileDialogData.lStructSize       = sizeof(OPENFILENAME);
	FileDialogData.hwndOwner         = MainWin;
	FileDialogData.hInstance         = Window::GetAppInstance();
	FileDialogData.lpstrFilter       = filter;
	FileDialogData.lpstrCustomFilter = NULL;
	FileDialogData.nMaxCustFilter    = 0;
	FileDialogData.nFilterIndex      = 1;
	FileDialogData.lpstrFile         = file;
	FileDialogData.nMaxFile          = sizeof(file);
	FileDialogData.lpstrFileTitle    = fileTitle;
	FileDialogData.nMaxFileTitle     = sizeof(fileTitle);
	FileDialogData.lpstrInitialDir   = NULL;
	FileDialogData.lpstrTitle        = "Save As";
	FileDialogData.nFileOffset       = 0;
	FileDialogData.nFileExtension    = 0;
	FileDialogData.lpstrDefExt       = NULL;
	FileDialogData.Flags             = OFN_SHOWHELP | OFN_EXPLORER | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&FileDialogData))
	{
		Bitmap frame(MainWin);
		AVIFile animation;

		scrTime.SetPosition(0);
		ScrollTime(0);

		animation.Open(FileDialogData.lpstrFileTitle, frame, framesPerSecond);

		for (int i = 0; i <= scrTime.GetMax(); i++)
		{
			ScrollTime(i);
			MainWin.Copy(frame, 0, 0);
			animation.AddFrame(frame);
		}
		animation.Close();
	}
}

void Application::OnKeyPress(size_t key)
{
	lastX = 0;
	lastY = 0;

	switch (key)
	{
	case VK_LEFT:
		Rotate3D(-1, 0);
		break;
	case VK_RIGHT:
		Rotate3D(1, 0);
		break;
	case VK_UP:
		Rotate3D(0, -1);
		break;
	case VK_DOWN:
		Rotate3D(0, 1);
		break;
	case VK_CONTROL:
		ctrlPressed = true;
		break;
	case 'O':
		if (ctrlPressed)
			OnOpen();
		break;
	case 'S':
		if (ctrlPressed)
			OnSave();
		break;
	case ' ':
		OnNext();
		break;
	}
}
 
void Application::OnKeyReleased(size_t key)
{
	switch (key)
	{
	case VK_CONTROL:
		ctrlPressed = false;
	}
}

void Application::OnNext(void)
{
	mainScene.NextFrame();
}

BOOL _stdcall AboutBoxMessageHandler(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
	switch (message)
	{
	case WM_COMMAND:      
		if(LOWORD(wParam) == IDOK)
			EndDialog(hDlg,TRUE);
		break;
	case WM_CLOSE:
		EndDialog(hDlg,TRUE);
		break;
	}
	return FALSE;
}

void Application::OnAbout(void)
{
	DialogBox(Window::GetAppInstance(), MAKEINTRESOURCE(IDD_DIALOG_ABOUT), 
		      MainWin, AboutBoxMessageHandler);
}

void Application::OnOpen(void)
{
	OPENFILENAME	FileDialogData;
	TCHAR			file[MAX_PATH] = "\0";
	TCHAR			fileTitle[MAX_PATH] = "\0";
	char			*filter = "Render files (*.rgl)\0*.rgl\0\0";

	strcpy (file, "");
	strcpy (fileTitle, "");

	FileDialogData.lStructSize       = sizeof(OPENFILENAME);
	FileDialogData.hwndOwner         = MainWin;
	FileDialogData.hInstance         = Window::GetAppInstance();
	FileDialogData.lpstrFilter       = filter;
	FileDialogData.lpstrCustomFilter = NULL;
	FileDialogData.nMaxCustFilter    = 0;
	FileDialogData.nFilterIndex      = 1;
	FileDialogData.lpstrFile         = file;
	FileDialogData.nMaxFile          = sizeof(file);
	FileDialogData.lpstrFileTitle    = fileTitle;
	FileDialogData.nMaxFileTitle     = sizeof(fileTitle);
	FileDialogData.lpstrInitialDir   = NULL;
	FileDialogData.lpstrTitle        = "Select a file to render";
	FileDialogData.nFileOffset       = 0;
	FileDialogData.nFileExtension    = 0;
	FileDialogData.lpstrDefExt       = NULL;
	FileDialogData.Flags             = OFN_SHOWHELP | OFN_EXPLORER | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&FileDialogData))
		mainScene.Load(FileDialogData.lpstrFileTitle);
}

bool ContainsPeriod(char *filename)
{
	int len = strlen(filename);

	for (int i = 0; i < len; i++)
	{	
		if (filename[i] == '.')
			return true;
	}
	return false;
}
void Application::OnSave(void)
{
	OPENFILENAME	FileDialogData;
	TCHAR			file[MAX_PATH] = "\0";
	TCHAR			fileTitle[MAX_PATH] = "\0";
	char			*filter = "BMP - Windows Bitmap\0*.bmp\0JPG - JPEG Compressed Image\0*.jpg;*.jpeg\0PNG - Portable Network Graphics Image\0*.png\0TGA - Targa Bitmap\0*.tga\0All Files\0*.*\0\0";

	strcpy (file, "");
	strcpy (fileTitle, "");

	FileDialogData.lStructSize       = sizeof(OPENFILENAME);
	FileDialogData.hwndOwner         = MainWin;
	FileDialogData.hInstance         = Window::GetAppInstance();
	FileDialogData.lpstrFilter       = filter;
	FileDialogData.lpstrCustomFilter = NULL;
	FileDialogData.nMaxCustFilter    = 0;
	FileDialogData.nFilterIndex      = 1;
	FileDialogData.lpstrFile         = file;
	FileDialogData.nMaxFile          = sizeof(file);
	FileDialogData.lpstrFileTitle    = fileTitle;
	FileDialogData.nMaxFileTitle     = sizeof(fileTitle);
	FileDialogData.lpstrInitialDir   = NULL;
	FileDialogData.lpstrTitle        = "Save As";
	FileDialogData.nFileOffset       = 0;
	FileDialogData.nFileExtension    = 0;
	FileDialogData.lpstrDefExt       = NULL;
	FileDialogData.Flags             = OFN_SHOWHELP | OFN_EXPLORER | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&FileDialogData))
	{
		TCHAR filename[MAX_PATH];

		if (ContainsPeriod(FileDialogData.lpstrFileTitle))
		{
			sprintf(filename, "%s", FileDialogData.lpstrFileTitle);
		}
		else
		{
			switch (FileDialogData.nFilterIndex)
			{
			case 1:
				sprintf(filename, "%s.%s", FileDialogData.lpstrFileTitle, "bmp");
				break;
			case 2:
				sprintf(filename, "%s.%s", FileDialogData.lpstrFileTitle, "jpg");
				break;
			case 3:
				sprintf(filename, "%s.%s", FileDialogData.lpstrFileTitle, "png");
				break;
			case 4:
				sprintf(filename, "%s.%s", FileDialogData.lpstrFileTitle, "tga");
				break;
			}
		}
		MainWin.Save(filename);
	}
}

