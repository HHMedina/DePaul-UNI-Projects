#ifndef __SCENE_H__
#define __SCENE_H__

#include <iostream>
#include <fstream>
using namespace std;

#include "point.h"
#include "vector.h"
#include "shape.h"
#include "OpenGLWin.h"
#include "Loader.h"
#include "Color.h"
#include "Matrix.h"
#include "LinkedList.h"

class Application;

typedef LinkedList<shape *> ShapeList;

class shadowMap;

// The data here is in floats (so is the rgb class!) because OpenGL only has light data functions
// that take float arrays.
struct LightData
{
	LightData() { color = specularColor = rgb::white; position = point(50, 100, 50); attenuation = vector(1, 0, 0); 
	spotDirection = vector(0, 0, 1); spotCutoff = 30.0f; spotFalloff = 5.0f; shadowmap = NULL; }

	void RegisterUniformParameters(GLuint curProgram, matrix view, int lightID);

	rgb color;
	rgb specularColor;
	point position;
	vector attenuation;
	vector spotDirection;
	float spotCutoff;
	float spotFalloff;
	shadowMap *shadowmap;
};

class scene
{
public:

	scene();
	scene(Application &app, OpenGLWin &targetWin);
	void setup(Application &app, OpenGLWin &targetWin);

	void Load(char *newFilename);
	void NextFrame(void);
	void SetupScene(bool show = true, char *fileName = "");
	void Display(bool show = true, char *fileName = "");

	void RenderScene(shape *scene, int Width, int Height, 
					 point cameraPos, point cameraTarget, vector cameraUp, float cameraFov,
					 float cameraNear, float cameraFar, const vector &nearNormal = vector(0, 0, 0),
					 const matrix &worldTrans = matrix(4, 4, 1));

	point CameraPos(void) { return Camera; }
	const point &GetCamera(void) { return Camera; }
	const point &GetInterest(void) { return Interest; }
	const vector &GetUpVector(void) { return Up; }
	
	// Get the current view and projection matrices
	matrix GetViewTransform(void) { return view; }
	matrix GetProjection(void) { return projection; }
	matrix GetViewRotationInverse(void) { return viewRotInverse; }
	matrix GetViewInverse(void) { return viewInverse; }
	matrix GetViewProjection(void) { return viewProjection; }

	// These methods move the viewer/world
	void Rotate(float dx, float dy);
	void InOut(float dx, float dy);
	void Translate(float dx, float dy);
	void RotateViewer(float dx, float dy);
	void FlyViewer(float dx, float dy);
	void BankViewer(float dx, float dy);

	// These methods set up the camera and projection and their pre-compiled versions
	matrix Perspective(float Fov, float AspectRatio, float NearZ, float FarZ);
	matrix Ortho(float height, float AspectRatio, float NearZ, float FarZ);
	matrix LookAt(point camera, point interest, vector up);

	matrix Perspective_PreComp(float Fov, float AspectRatio, float NearZ, float FarZ);
	matrix LookAt_PreComp(point camera, point interest, vector up);

	// Methods for accessing lights
	int NLights(void) { return nLights; }
	LightData GetLight(int i) { return lights[i]; }
	rgb GetGlobalAmbient(void) { return GlobalAmbient; }
	
	// Methods for manipulating time
	void SetTime(float newTime) { time = newTime; }
	float GetTime(void) { return time; }

	// Methods for retreiving camera data
	point GetCameraPos(void) { return Camera; }
	point GetCameraTarget(void) { return Interest; }
	vector GetCameraUp(void) { return Up; }
	float GetCameraFOV(void) { return Fov; }
	float GetCameraNear(void) { return NearZ; }
	float GetCameraFar(void) { return FarZ; }

	// Methods for accessing scene graph stacks
	void PushModelTransform(const matrix &m);
	void PushModelTransform();  // Push a duplicate of current modelTransform onto the stack
	void SetModelTransform(const matrix &m);   // Overwrites the element on the top of the stack with m 
	matrix PopModelTransform();
	matrix GetCurrentModelTransform();

	void PushSurfaceDiffuse(const rgb &diffuse);
	void PushSurfaceDiffuse();  // Push a duplicate
	void SetSurfaceDiffuse(const rgb &diffuse);  // Set the top element of the stack
	rgb PopSurfaceDiffuse();
	rgb GetCurrentSurfaceDiffuse();

	void PushSurfaceSpecular(const rgb &specular);
	void PushSurfaceSpecular();  // Push a duplicate
	void SetSurfaceSpecular(const rgb &specualr);  // Set the top element of the stack
	rgb PopSurfaceSpecular();
	rgb GetCurrentSurfaceSpecular();

	void PushSurfaceShininess(float shininess);
	void PushSurfaceShininess();  // Push a duplicate
	void SetSurfaceShininess(float shininess);  // Set the top element of the stack
	float PopSurfaceShininess();
	float GetCurrentSurfaceShininess();

	void PushAlphaTransparency(bool transparency);
	void PushAlphaTransparency();  // Push a duplicate
	void SetAlphaTransparency(bool transparency);  // Set the top element of the stack
	bool PopAlphaTransparency();
	bool GetCurrentAlphaTransparency();

	void PushShader(shader *s);
	shader *PopShader();
	shader *GetCurrentShader();

	shape *GetSceneShape(void) { return sceneShape; }

	void RegisterUniformParameters(int glProgramObj);

	OpenGLWin *GetTargetWin(void) { return TargetWin; }

private:
	
	enum { MAX_FILENAME_LENGTH = 256, MAX_INCLUDE_DEPTH = 10 };

	void SetupDefaults(void);
	Loader input[MAX_INCLUDE_DEPTH];
	int curStream;

	char filename[MAX_FILENAME_LENGTH];

	shape *sceneShape; // Every scene consists of a single root shape (usually a component)

	// Data for the scene
	point  Camera;
	point  Interest;
	vector Up;
	point  ViewRef;
	float Eye;
	float Viewport[4];
	float Window[4];
	float Fov;
	float AspectRatio;
	float NearZ, FarZ;
	int   Resolution;

	// These boolean flags indicate which of the members above was specified
	// in the input file.  The problem is that there are several redundant 
	// methods for specifying a camera supported in RenderGL and these flags
	// indicate which one was used.  See the scene setup code to see how they
	// are used
	bool hasCamera;
	bool hasInterest;
	bool hasUp;
	bool hasViewRef;
	bool hasEye;
	bool hasViewport;
	bool hasWindow;
	bool hasFov;
	bool hasAspect;
	bool hasNear, hasFar;
	bool hasResolution;

	// Data for lights.  RenderGL supports a maximum of 8 lights
	rgb BackgroundColor;
	rgb GlobalAmbient;
	LightData lights[8];
	int nLights;

	// Transform data.  For model transforms see the stacks below
	matrix view;
	matrix viewInverse;
	matrix viewRotInverse;
	matrix projection;
	matrix viewProjection;

	// Timing data
	float time;

	// These stacks track data for the scene graph as we are traversing it
	Stack<matrix> ModelTransformStack;
	Stack<rgb> SurfaceDiffuseStack;
	Stack<rgb> SurfaceSpecularStack;
	Stack<float> SurfaceShininessStack;
	Stack<bool> AlphaTransparencyStack;
	Stack<bool> AlphaTestStack;
	Stack<shader *> ShaderStack;

	// A flag to indicate if we are resizing the screen or not
	bool resizing;

	// We cache the pointer to the window that the scene is being drawn in so that we can access its
	// characteristics
	OpenGLWin *TargetWin;
	Application *App;
};

#endif
