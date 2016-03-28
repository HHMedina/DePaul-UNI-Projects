#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "OpenGLWin.h" // Contains definition of EventListener
#include "scene.h"
#include "static.h"
#include "bitmapbutton.h"
#include "scrollbar.h"
#include "AppWindow.h"
 
class Application : public EventListener
{
public:

	Application();

	void Resize(int w, int h);
	void SetAnimationLength(double newLength);
	void SetFramesPerSecond(int newCount);

private:

	// Main Windows
	AppWindow		Container;
	OpenGLWin		MainWin;
	Label			lblTime;
	ScrollBar		scrTime;
	BitmapButton	Play;
	BitmapButton	Pause;
	double			animationLength;
	int				framesPerSecond;

	scene mainScene;
	shape *selectedShape;

	void SetupControls(void);

	// Method for Resizing and Updating the window
	void UpdateWindow3D(intRect UpdateRect);
	void ResizeWindow3D(size_t w, size_t h);
	void ResizeWindow(size_t w, size_t h);

	void PlayButtonPressed(void);
	void PauseButtonPressed(void);
	void ScrollTime(int pos);
	void AnimationTimer(size_t id);

	// Button event handlers for the 3D window
	// for performing rotations in 3D.  
	void InitTransform3D(int x, int y); // = ButtonDown
	void Rotate3D(int x, int y);        // = ButtonDrag -- Note: ButtonUp not necessary
	void OnKeyPress(size_t key);
	void OnKeyReleased(size_t key);

	void OnRotate(void);
	void OnTranslate(void);
	void OnInOut(void);

	void OnRender(void);
	void OnNext(void);
	void OnAbout(void);
	void OnOpen(void);
	void OnSave(void);

	int lastX, lastY;
	bool ctrlPressed;
};

#endif