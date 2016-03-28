#ifndef __OPENGLWIN_H__
#define __OPENGLWIN_H__

#include "AppWindow.h"
#include "point.h"
#include "gl/glew.h"
#include "gl/wglew.h"
#include <gl/glu.h>

#define GLERR do \
{\
    GLuint glerr;\
    while((glerr = glGetError()) != GL_NO_ERROR)\
        fprintf(stderr, "%s:%d glGetError() = 0x%04x\n", __FILE__, __LINE__, glerr);\
} while (0)

struct fBufferData
{
	fBufferData() { fBuffer = cBuffer = dBuffer = cTexObject = dTexObject = fBufferW = fBufferH = 0; cube = false; }

	HGLRC rContext;
	GLuint fBuffer;
	GLuint cBuffer, cTexObject;
	GLuint dBuffer, dTexObject;
	bool cube;
	int fBufferW, fBufferH;
};

class OpenGLWin : public AppWindow
{
public:
      
	OpenGLWin();
	OpenGLWin(int x, int y, int width, int height, bool Visible = true);
	OpenGLWin(Window &parent, int x, int y, int width, int height, bool Visible = true);
	virtual ~OpenGLWin();

	void OpenGLWinSetup();

	// OpenGL Utilities
	void MakeCurrent(void);
	void SwapBuffers(void);
	void Refresh(void);
	void Clear(void);

	// Methods for managing Framebuffers
	bool CreateFBuffer(int width, int height, fBufferData &newFBuffer);
	bool CreateColorTexFBuffer(int width, int height, fBufferData &newFBuffer);
	bool CreateDepthTexFBuffer(int width, int height, fBufferData &newFBuffer);
	bool CreateCubeTexFBuffer(int width, int height, fBufferData &newFBuffer);
	void DestroyFBuffer(fBufferData &fBuffer);
	void RenderToFBuffer(fBufferData &fBuffer, int cube_face = -1);
	void RenderToScreen(void);
	bool CheckFramebufferError(int error);

	// Methods to Save an Image
 	void Save(char *filename);
	void SaveZ(char *filename);

	float Version(void) { return OpenGLVersion; }

private:

	HGLRC OpenGLRC;
	static HGLRC CurrentRC;
	bool OpenGLActive;
	HDC origDC;
	float OpenGLVersion;
	GLuint baseVertexArrayObj;
	fBufferData *curFramebuffer;

	// Utility Methods
	void Destroy3D(void);
	int GetScreenBitDepth(void);
	void InitializeExtensions(void);
	void SetupPixelFormat(void);
	void Setup3DGraphics(void);
	virtual bool OnSize(size_t Width, size_t Height, size_t Type);

	// Methods for managing framebuffers
	LONG ProcessMessage(HWND hWnd, size_t msg, size_t wParam, LONG lParam);
};

void wglGetLastError();

#endif 
