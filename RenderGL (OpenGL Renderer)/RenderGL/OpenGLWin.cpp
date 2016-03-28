
#include <math.h>
#include "jmisc.h"
#include "OpenGLWin.h"
#include "BitmapFile.h"

HGLRC OpenGLWin::CurrentRC = NULL;
 
OpenGLWin::OpenGLWin() : AppWindow()
{
	OpenGLActive = false;
}

OpenGLWin::OpenGLWin(int x, int y, int width, int height, bool Visible) : AppWindow(x, y, width, height, Visible)
{
	OpenGLWinSetup();
}

OpenGLWin::OpenGLWin(Window &parent, int x, int y, int width, int height, bool Visible) :
			 AppWindow(parent, x, y, width, height, Visible)
{
	OpenGLWinSetup();
}

OpenGLWin::~OpenGLWin()
{
    Destroy3D();
}

void OpenGLWin::OpenGLWinSetup()
{
	curFramebuffer = NULL;

	SetupPixelFormat();
	Setup3DGraphics();
}

// This method reads the number of bits used in the screen windows by creating 
// a bitmap compatible with the screen and reading its setup information, namely 
// its bit depth
int OpenGLWin::GetScreenBitDepth(void)
{
	BITMAPINFO m_Display;

	HDC hdcDisplay = CreateDCA("DISPLAY", NULL, NULL, NULL);     
	HBITMAP hbm = CreateCompatibleBitmap(hdcDisplay,1,1);     
    ZeroMemory((PVOID)&m_Display,sizeof(BITMAPINFO));     
	m_Display.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	GetDIBits(hdcDisplay, hbm, 0, 1, NULL, &m_Display, DIB_RGB_COLORS);
	DeleteObject(hbm);     
	DeleteDC(hdcDisplay);      
	return m_Display.bmiHeader.biBitCount; 
}

void OpenGLWin::InitializeExtensions(void)
{
	int pixelformat, result, bitDepth = GetScreenBitDepth();
	
	// Create a temporary invisible window, will be destroyed at end because it is local
	Window win(0, 0, 10, 10, false);  

	PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR), 1, 
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, bitDepth, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 32, 1, 0, PFD_MAIN_PLANE, 0, 0, 0, 0 
	};
	pixelformat = ChoosePixelFormat(win.DC(), &pfd);
	result = SetPixelFormat(win.DC(), pixelformat, &pfd);
	HGLRC tmpRC = wglCreateContext(win.DC());
	wglMakeCurrent(win.DC(), tmpRC);

	GLenum err = glewInit();

	if (GLEW_OK != err)
		complain("Error: %s\n", glewGetErrorString(err));

	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;

	int majorVersion, minorVersion;
	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion); 
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

	if (majorVersion < 3 || (majorVersion == 3 && minorVersion < 2))
		complain("Error: OpenGL 3.2 or better required to run RenderGL");
	else
		cout << "OpenGL " << majorVersion << "." << minorVersion << " supported\n";

	wglMakeCurrent(0,0);
	wglDeleteContext(tmpRC);
}

void OpenGLWin::SetupPixelFormat(void)
{
	int pixelFormat = 0, valid = 0;
	UINT numFormats;

	InitializeExtensions();

	int bitDepth = GetScreenBitDepth();
	if (bitDepth < 24) 
		complain("Cannot work with less than 24-bit color: Curent Color Depth = %d\nPlease change your graphics mode\n", bitDepth);

	PIXELFORMATDESCRIPTOR pfd;
	float fAttributes[] = {0,0};
	int iAttributes[] = 
	{	
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_COLOR_BITS_ARB, 24,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 0,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, 4,						
		0,0
	};

	valid = wglChoosePixelFormatARB(dc, iAttributes, fAttributes, 1, &pixelFormat, &numFormats); 
	if (valid && numFormats != 0)
	{
		int result = SetPixelFormat(dc, pixelFormat, &pfd);
		if (result == FALSE)
		{
			MessageError();
			OpenGLActive = FALSE;
			return;
		}
	}
}

void OpenGLWin::Setup3DGraphics(void)
{
	int contextAttribs[] = 
	{	
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0,0
	};
	OpenGLRC = wglCreateContextAttribsARB(dc, 0, contextAttribs);
	origDC = dc;

	if (OpenGLRC == 0)
		complain("Couldn't initialize OpenGL rendering context");

	if (!wglMakeCurrent(dc, OpenGLRC))
		complain("Couldn't make OpenGL rendering context current");

	OpenGLActive = TRUE;
	CurrentRC = OpenGLRC;

	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth((GLclampd)1.0);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

	glGenVertexArrays(1, &baseVertexArrayObj);
	glBindVertexArray(baseVertexArrayObj);

	curFramebuffer = NULL;

	Clear();
	Refresh();
}

void OpenGLWin::MakeCurrent(void)
{
	if (CurrentRC != OpenGLRC)
	{
		wglMakeCurrent(dc, OpenGLRC);
		CurrentRC = OpenGLRC;
	}
}

bool OpenGLWin::CreateFBuffer(int width, int height, fBufferData &newFBuffer)
{
	RenderToScreen();

	newFBuffer.rContext = OpenGLRC;

	newFBuffer.fBufferW = width;
	newFBuffer.fBufferH = height;

	glGenFramebuffers(1, &newFBuffer.fBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, newFBuffer.fBuffer);

	glGenRenderbuffers(1, &newFBuffer.dBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, newFBuffer.dBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, newFBuffer.dBuffer);

	glGenRenderbuffers(1, &newFBuffer.cBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, newFBuffer.cBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, newFBuffer.cBuffer);

	if (CheckFramebufferError(glCheckFramebufferStatus(GL_FRAMEBUFFER)) == false)
	{
		DestroyFBuffer(newFBuffer);
		return false;
	}
	else 
		return true;
}

bool OpenGLWin::CreateColorTexFBuffer(int width, int height, fBufferData &newFBuffer)
{
	RenderToScreen();

	newFBuffer.rContext = OpenGLRC;

	newFBuffer.fBufferW = width;
	newFBuffer.fBufferH = height;

	glGenFramebuffers(1, &newFBuffer.fBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, newFBuffer.fBuffer);

	glGenRenderbuffers(1, &newFBuffer.dBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, newFBuffer.dBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, newFBuffer.dBuffer);

	glGenTextures(1, &newFBuffer.cTexObject);
	glBindTexture(GL_TEXTURE_2D, newFBuffer.cTexObject);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, newFBuffer.cTexObject, 0);

	if (CheckFramebufferError(glCheckFramebufferStatus(GL_FRAMEBUFFER)) == false)
	{
		DestroyFBuffer(newFBuffer);
		return false;
	}
	else 
		return true;
}

bool OpenGLWin::CreateCubeTexFBuffer(int width, int height, fBufferData &newFBuffer)
{
	RenderToScreen();

	newFBuffer.rContext = OpenGLRC;

	newFBuffer.fBufferW = width;
	newFBuffer.fBufferH = height;

	glGenFramebuffers(1, &newFBuffer.fBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, newFBuffer.fBuffer);

	glGenRenderbuffers(1, &newFBuffer.dBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, newFBuffer.dBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, newFBuffer.dBuffer);

	glGenTextures(1, &newFBuffer.cTexObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, newFBuffer.cTexObject);
	for (int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	newFBuffer.cube = true;
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, newFBuffer.cTexObject, 0);

	if (CheckFramebufferError(glCheckFramebufferStatus(GL_FRAMEBUFFER)) == false)
	{
		DestroyFBuffer(newFBuffer);
		return false;
	}
	else 
		return true;
}

bool OpenGLWin::CreateDepthTexFBuffer(int width, int height, fBufferData &newFBuffer)
{
	RenderToScreen();

	newFBuffer.rContext = OpenGLRC;

	newFBuffer.fBufferW = width;
	newFBuffer.fBufferH = height;

	glGenTextures(1, &newFBuffer.dTexObject);
	glBindTexture(GL_TEXTURE_2D, newFBuffer.dTexObject);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glGenFramebuffers(1, &(newFBuffer.fBuffer));
	glBindFramebuffer(GL_FRAMEBUFFER, newFBuffer.fBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, newFBuffer.dTexObject, 0);

	glGenRenderbuffers(1, &newFBuffer.cBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, newFBuffer.cBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, newFBuffer.cBuffer);

	if (CheckFramebufferError(glCheckFramebufferStatus(GL_FRAMEBUFFER)) == false)
	{
		DestroyFBuffer(newFBuffer);
		return false;
	}
	else 
		return true;
}

bool OpenGLWin::CheckFramebufferError(int error)
{
	if (error != GL_FRAMEBUFFER_COMPLETE)
	{
		switch (error)
		{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			fprintf(stderr, "framebuffer incomplete attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			fprintf(stderr, "framebuffer incomplete missing attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			fprintf(stderr, "framebuffer incomplete dimensions\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			fprintf(stderr, "framebuffer incomplete formats\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			fprintf(stderr, "framebuffer incomplete draw buffer\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			fprintf(stderr, "framebuffer incomplete read buffer\n");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			fprintf(stderr, "framebuffer unsupported\n");
			break;
		default:
			fprintf(stderr, "unknown framebuffer error\n");
		}
		fprintf(stderr, "Problem in framebuffer creation\n, %d", error);
		return false;
	}
	else
		return true;
}

void OpenGLWin::DestroyFBuffer(fBufferData &bufferData)
{
	// Delete the framebuffer object
	glDeleteFramebuffers(1, &bufferData.fBuffer);

	// Delete either the renderbuffer or the texture for the depth attachment
	if (bufferData.dBuffer)
		glDeleteRenderbuffers(1, &bufferData.dBuffer);
	else if (bufferData.dTexObject)
		glDeleteTextures(1, &bufferData.dBuffer);

	// Delete either the renderbuffer or the texture for the color attachment
	if (bufferData.cBuffer)
		glDeleteRenderbuffers(1, &bufferData.cBuffer);
	else if (bufferData.cTexObject)
		glDeleteTextures(1, &bufferData.cBuffer);

	// Reset the values in the buffer data 
	bufferData.fBuffer = bufferData.cBuffer = bufferData.dBuffer = 0;
	bufferData.cTexObject = bufferData.dTexObject = 0;
	bufferData.fBufferW = bufferData.fBufferH = 0; 
	bufferData.cube = false; 
}

void OpenGLWin::RenderToFBuffer(fBufferData &buffer, int cube_face)
{
	assert(buffer.rContext == OpenGLRC);

	RenderToScreen();
	glBindFramebuffer(GL_FRAMEBUFFER, buffer.fBuffer);
	if (cube_face > -1 && buffer.cTexObject > 0 && buffer.cube)
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + cube_face, buffer.cTexObject, 0);
	curFramebuffer = &buffer;
}

void OpenGLWin::RenderToScreen(void)
{
	if (curFramebuffer != NULL)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	curFramebuffer = NULL;
	MakeCurrent();
}

LONG OpenGLWin::ProcessMessage(HWND hWnd, size_t msg, size_t wParam, LONG lParam)
{
	if (msg == WM_ERASEBKGND) // OpenGL likes to erase its own windows
		return true;		  // If we don't have this, the screen will flicker
	else					  // When the window is sized
		return AppWindow::ProcessMessage(hWnd, msg, wParam, lParam);
}

void OpenGLWin::Destroy3D(void)
{
	if (OpenGLActive)
	{
		wglMakeCurrent(0,0);
		wglDeleteContext(OpenGLRC);
	}
}

bool OpenGLWin::OnSize(size_t Width, size_t Height, size_t Type)
{
	glViewport(0, 0, Width, Height);
	return AppWindow::OnSize(Width, Height, Type);
}

void OpenGLWin::Refresh(void)
{
	SwapBuffers();
}

void OpenGLWin::SwapBuffers(void)
{
	::SwapBuffers(dc);
}

void OpenGLWin::Clear(void)
{
	MakeCurrent();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLWin::Save(char *filename)
{
	int w, h;

	if (curFramebuffer == NULL)
	{
		w = Width();
		h = Height();
	}
	else
	{
		w = curFramebuffer->fBufferW;
		h = curFramebuffer->fBufferH;
	}

	BitmapFile output(w, h);
	glReadPixels(0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, output.ImageData());
	output.Write(filename);
}

void OpenGLWin::SaveZ(char *filename)
{
	int w, h;

	if (curFramebuffer == NULL)
	{
		w = curFramebuffer->fBufferW;
		h = curFramebuffer->fBufferH;
	}
	else
	{
		w = Width();
		h = Height();
	}
	BitmapFile output(w, h);
	GLfloat *glImage = new GLfloat[w * h];

	glReadPixels(0, 0, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, glImage);
	
	// The data is not compatible with the bitmap format so we copy in the data
	// directly
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			Pixel p;
			p.r = unsigned char(255 * glImage[w * j + i]);
			p.g = unsigned char(255 * glImage[w * j + i]);
			p.b = unsigned char(255 * glImage[w * j + i]);
			p.alpha = 255;
			output.PutPixel(i, j, p);
		}
	}
	output.WriteBMP(filename);
}

void wglGetLastError()
{
    DWORD err = GetLastError();
    switch(err)
    {
    case ERROR_INVALID_PIXEL_FORMAT:
        cerr << "Win32 Error:  ERROR_INVALID_PIXEL_FORMAT\n";
        break;
    case ERROR_NO_SYSTEM_RESOURCES:
        cerr << "Win32 Error:  ERROR_NO_SYSTEM_RESOURCES\n";
        break;
    case ERROR_INVALID_DATA:
        cerr << "Win32 Error:  ERROR_INVALID_DATA\n";
        break;
    case ERROR_INVALID_WINDOW_HANDLE:
        cerr << "Win32 Error:  ERROR_INVALID_WINDOW_HANDLE\n";
        break;
    case ERROR_RESOURCE_TYPE_NOT_FOUND:
        cerr << "Win32 Error:  ERROR_RESOURCE_TYPE_NOT_FOUND\n";
        break;
    case ERROR_SUCCESS:
        // no error
        break;
	case ERROR_ALREADY_EXISTS:
		// no error ... just trying to create a file over an existing file
		break;
    default:
        cerr << "Win32 Error:  " << err << endl;
        break;
    }
    SetLastError(0);
}

