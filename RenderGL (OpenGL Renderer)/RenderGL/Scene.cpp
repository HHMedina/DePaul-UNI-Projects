#include <time.h>
#include "OpenGLWin.h"
#include "jmisc.h"
#include "Scene.h"
#include "Component.h"
#include "Mesh.h"
#include "Texture.h"
#include "Application.h"
#include "OpenGLWin.h"

void LightData::RegisterUniformParameters(GLuint curProgram, matrix view, int lightID)
{
	char paramName[64];
	sprintf(paramName, "lightPosition[%d]", lightID);
	point lightInView = view * position;
	glUniform4fv(glGetUniformLocation(curProgram, paramName), 1, lightInView.AsArray());
	sprintf(paramName, "lightColor[%d]", lightID);
	glUniform4fv(glGetUniformLocation(curProgram, paramName), 1, color.AsArray());
	sprintf(paramName, "lightSpecular[%d]", lightID);
	glUniform4fv(glGetUniformLocation(curProgram, paramName), 1, specularColor.AsArray());
	sprintf(paramName, "lightAttenuation[%d]", lightID);
	glUniform3fv(glGetUniformLocation(curProgram, paramName), 1, attenuation.AsArray());
	sprintf(paramName, "lightSpotDirection[%d]", lightID);
	vector v = view * spotDirection;
	v.Normalize();
	glUniform3fv(glGetUniformLocation(curProgram, paramName), 1, v.AsArray());
	sprintf(paramName, "lightSpotCutoff[%d]", lightID);
	glUniform1f(glGetUniformLocation(curProgram, paramName), spotCutoff * PIf / 180.0f);
	sprintf(paramName, "lightSpotFalloff[%d]", lightID);
	glUniform1f(glGetUniformLocation(curProgram, paramName), spotFalloff * PIf / 180.0f);



}

scene::scene()
{
	strcpy(filename, "");
	SetupDefaults();
	TargetWin = NULL;
	curStream = -1;
	time = 0;

	ModelTransformStack.Push(matrix::Identity(4));
	SurfaceDiffuseStack.Push(rgb::white);
	SurfaceSpecularStack.Push(rgb::black);
	SurfaceShininessStack.Push(20.0f);
	AlphaTransparencyStack.Push(false);
	AlphaTestStack.Push(false);

	shader *defaultShader = new gouraudShader();
	defaultShader->SetScene(this);
	ShaderStack.Push(defaultShader);
}

scene::scene(Application &app, OpenGLWin &targetWin)
{
	setup(app, targetWin);
}

void scene::setup(Application &app, OpenGLWin &targetWin)
{
	strcpy(filename, "");
	SetupDefaults();
	curStream = -1;
	App = &app;
	TargetWin = &targetWin;
	TargetWin->Clear();
	time = 0;

	ModelTransformStack.Clear();
	SurfaceDiffuseStack.Clear();
	SurfaceSpecularStack.Clear();
	SurfaceShininessStack.Clear();
	AlphaTransparencyStack.Clear();
	AlphaTestStack.Clear();

	ModelTransformStack.Push(matrix::Identity(4));
	SurfaceDiffuseStack.Push(rgb::white);
	SurfaceSpecularStack.Push(rgb::black);
	SurfaceShininessStack.Push(20.0f);
	AlphaTransparencyStack.Push(false);
	AlphaTestStack.Push(false);

	shader *defaultShader = new constantShader();
	defaultShader->SetScene(this);
	ShaderStack.Push(defaultShader);
}

void scene::SetupDefaults(void)
{
	Camera = point(0, 0, 0);
	Interest = point(0, 0, -1);
	Up = vector(0, 1, 0);
	NearZ = -10;
	FarZ = 100;
	Resolution = 400;
	Window[0] = -1; Window[1] = -1;
	Window[2] = 1; Window[3] = 1;
	Fov = 0.0f;
	AspectRatio = 1.0f;
	nLights = 0;
	hasFov = false;
	hasViewRef = false;
	sceneShape = NULL;
	resizing = false;

	time = 0;

	GlobalAmbient = rgb::gray;
	BackgroundColor = rgb::black;

	// Setup a default light
	lights[0].color = rgb::white;	
	lights[0].specularColor = rgb::white;
	lights[0].position[0] = 50.0f;					// Essentially a distant light high above
	lights[0].position[1] = 100.0f;
	lights[0].position[2] = 50.0f;
	lights[0].attenuation[0] = 1.0f;				// No Attenuation
	lights[0].attenuation[1] = 0.0f;				// No Attenuation
	lights[0].attenuation[2] = 0.0f;				// No Attenuation
	lights[0].spotCutoff = 180;						// No spotlight cutoff
	lights[0].shadowmap = NULL;
	nLights = 1;
}
 
void scene::SetupScene(bool show, char *fileName)
{
	int Width, Height;
	float windowW, windowH;

	if (hasFov)
	{
		if (!hasWindow)
		{
			if (AspectRatio > 1)
			{
				Window[0] = -1.0f; Window[1] = -1.0f / AspectRatio;
				Window[2] =  1.0f; Window[3] =  1.0f / AspectRatio;
			}
			else
			{
				Window[0] = -AspectRatio; Window[1] = -1.0f;
				Window[2] =  AspectRatio; Window[3] =  1.0f;
			}			
		}
		else
			AspectRatio = (Window[2] - Window[0]) / (Window[3] - Window[1]);
	}
	windowW = Window[2] - Window[0];
	windowH = Window[3] - Window[1];

	if (windowW > windowH)
	{
		Width = int(Resolution);
		Height = int(windowH / windowW * Resolution);
	}
	else
	{
		Width = int(windowW / windowH * Resolution);
		Height = int(Resolution);
	}

	if (!hasFov)
	{
		Fov = 2 * atan(windowH / (2*NearZ)) * 180.0f / PIf;
		AspectRatio = windowW / windowH;

		// We don't set 'hasFov' here because we rely on the rgl file to choose whether it specifies
		// field of view with an angle or with the window/near values, and we want to preserve 
		// this setting over several "frames" in the rgl file.
	}

	// We turn on the "resizing" flag so that we don't cause an infinite loop of redraws.
	resizing = true;
	App->Resize(Width + 4, Height + 4); // adjust for borders
	resizing = false;

	Display(show, fileName);
}

void scene::Display(bool show, char *fileName)
{
	if ((TargetWin == NULL) || (sceneShape == NULL) || resizing)
		return;

	__int64 res, t1, t2;
	QueryPerformanceFrequency((LARGE_INTEGER *)&res);
	QueryPerformanceCounter((LARGE_INTEGER *)&t1);

	TargetWin->MakeCurrent();

	RenderScene(sceneShape, TargetWin->Width(), TargetWin->Height(), Camera, Interest, Up, Fov, NearZ, FarZ);

	if (show)
		TargetWin->SwapBuffers();
	if (strlen(fileName) > 0)
		TargetWin->Save(fileName);

	QueryPerformanceCounter((LARGE_INTEGER *)&t2);
	cout << "Draw took " << float(t2 - t1) / res << endl;

	GLERR;
}

void scene::RenderScene(shape *scene, int Width, int Height, 
						   point cameraPos, point cameraTarget, vector cameraUp, float cameraFov,
						   float cameraNear, float cameraFar, const vector &nearNormal,
						   const matrix &worldTrans)  
{
	matrix I(4, 4, 1.0); 
	Stack<matrix> tmpMatrixStack = ModelTransformStack;
	ModelTransformStack.Clear();
	ModelTransformStack.Push(I);

	shape::SetCurrentScene(this);
	texture::SetCurrentScene(this);

	TargetWin->MakeCurrent();

	if ((scene == NULL) || resizing)
	{
		scene = sceneShape;   

		TargetWin->Clear();
		TargetWin->SwapBuffers();
	}

	float cameraAspect = float(Width) / Height;

	// We assume that there will be always one shader in the stack.  See the constructor
	GetCurrentShader()->BeginShading();
	glClearColor(BackgroundColor[0], BackgroundColor[1], BackgroundColor[2], BackgroundColor[3]);

	// Setup the viewport and the projection matrix.
	glViewport(0, 0, Width, Height);

	// To Do
	// 
	// Once you have implmenented the full versions of the "PreComp" functions 
	// below, replace these calls with your own!
	// 
	// For the projection, if the cameraFov is 0, then we use an orthographic projection
	if (IsNotZero(cameraFov))
		projection = Perspective(cameraFov, cameraAspect, cameraNear, cameraFar);
	else
		projection = Ortho((Window[3] - Window[1]) / 2, cameraAspect, cameraNear, cameraFar);

	view = LookAt(cameraPos, cameraTarget, cameraUp);
	viewInverse = view.Inverse();
	
	// To Do
	//
	// Compute the product of the view and projection matrices.  Remember though
	// that you want the view transform to affect the vertex first, so make sure
	// that you multiply them in the correct order.
	viewProjection = projection  *  view;

	TargetWin->Clear();
	scene->Draw();
	GetCurrentShader()->EndShading();

	ModelTransformStack = tmpMatrixStack;
}

matrix scene::Perspective(float Fov, float AspectRatio, float NearZ, float FarZ)
{
	// To Do
	// 
	// Compute the perspective matrix with the given data and return it
	

	float toRad = (float)PI/180.0f;
	//height = 2n * tan(Theta/2)
	float height = 2.0f * NearZ * tanf(Fov * toRad*.5f); 
	float width = height * AspectRatio;
	matrix temp(4, 4,0.0f);

	temp(0 ,0) = 2.0*NearZ/width;
	temp(1, 1) = 2.0f*NearZ/ height;
	temp(2, 2) = -(FarZ + NearZ) / (FarZ - NearZ);
	temp(2, 3) = (-2.0f*FarZ * NearZ) / (FarZ - NearZ);
	temp(3, 2) = -1.0f;

	return temp;
}

matrix scene::Ortho(float halfH, float AspectRatio, float NearZ, float FarZ)
{
	matrix m(4, 4, 0.0);

	float halfW = halfH * AspectRatio;

	m(0, 0) = 1 / halfW;
	m(1, 1) = 1 / halfH;
	m(2, 2) = -2 / (FarZ - NearZ);
	m(2, 3) = -(FarZ + NearZ) / (FarZ - NearZ);
	m(3, 3) = 1;

	return m;
}

matrix scene::LookAt(point camera, point interest, vector up)
{
	// To Do
	// 
	// Calculate the rotation and translation parts of the "look-at" transformation
	// and return the product (i.e. the matrix that orients the camera for our scene.
	//
	// Note that you can access element (row i, col j) in the matrix class with the
	// () operator, i.e. m(i, j)

	vector camPosV = vector(camera[0], camera[1], camera[2]); /*Can only use DOT on vectors*/

	//V = targ - campos;
	vector view = interest - camera;
	view.Normalize();

	//R = V X Up
	vector right = Cross(view, up);
	right.Normalize();

	//perpUp = right X view
	vector perpUp = Cross(right, view);
	perpUp.Normalize();

	matrix temp(4, 4); // identity 4x4 Matrix

	//1st row
	temp(0, 0) = right[0];
	temp(0, 1) = right[1];
	temp(0, 2) = right[2];
	temp(0, 3) = Dot(right, -camPosV);

	//2nd row
	temp(1, 0) = perpUp[0];
	temp(1, 1) = perpUp[1];
	temp(1, 2) = perpUp[2];
	temp(1, 3) = Dot(perpUp, -camPosV);

	//3rd row
	temp(2, 0) = -view[0];
	temp(2, 1) = -view[1];
	temp(2, 2) = -view[2];
	temp(2, 3) = Dot(-view, -camPosV);

	return temp;
}

void scene::Rotate(float dx, float dy)
{
	// To Do
	//
	// Use the values of dx and dy to rotate the scene.
	// Do this by moving the camera along the vector 
	// to the right of the camera (you need to calculate
	// that) according to dx and along the vector towards
	// the top of the camera (the "up" vector) according
	// to dy


	float Sensitivity = 16.0f;

	float initialDist = 0.0f;

	vector view = Interest - Camera;
	initialDist = view.Length();
	if (IsNotZero(view)){
		view.Normalize();
	}

	vector rightOfCam = Cross(view, Up);
	if (IsNotZero(rightOfCam)){
		rightOfCam.Normalize();
	}

	vector UpPerp = Cross(rightOfCam, view);
	if (IsNotZero(UpPerp)){
		UpPerp.Normalize();
	}


	Camera -= rightOfCam * dx * Sensitivity;
	Camera += UpPerp * dy * Sensitivity;

	/*Fix the distance of result*/
	vector targToCam = Camera - Interest;
	if (IsNotZero(targToCam)){
		targToCam.Normalize();
	}

	Camera = Interest + (initialDist * targToCam);

	/*Fix the up vector of camera*/
	Up = UpPerp;

}

void scene::InOut(float dx, float dy)
{
	// To Do
	//
	// Use the values of dx and dy to move the camera
	// in and out towards the interest point.  Note that
	// you do not necessarily have to use both dx and dy.
	//
	// Note also that this should not move the interest point.
	// It is not a fly through, but a movement towards the 
	// interest point.

	float Sensitivity = 2.0f;

	vector dirToTar = (Interest - Camera); // not normalized to have more of an effect when further away

	Camera -= (dirToTar * dx * Sensitivity);

}

void scene::Translate(float dx, float dy)
{
	// To Do
	//
	// Use the values of dx and dy to translate the scene
	// Here you have to move both the camera and the interest
	// point

	float Sensitivity = 4.0f;

	vector view = Interest - Camera;
	if (IsNotZero(view)){
		view.Normalize();
	}

	vector rightOfCam = Cross(view, Up);
	if (IsNotZero(rightOfCam)){
		rightOfCam.Normalize();
	}

	vector UpPerp = Cross(-view, rightOfCam);
	if (IsNotZero(UpPerp)){
		UpPerp.Normalize();
	}

	Camera -= rightOfCam * dx * Sensitivity;
	Camera += UpPerp * dy * Sensitivity;

	Interest -= rightOfCam * dx* Sensitivity;
	Interest += UpPerp * dy* Sensitivity;


}

void scene::RotateViewer(float dx, float dy)
{
	// To Do
	//
	// This camera rotates the view from the perspective of the viewer, it does 
	// so in the same way as the Rotate method above, but it affects the target
	// instead of the camera position because the direction of the view must
	// change
}

void scene::FlyViewer(float dx, float dy)
{
	// To Do
	// 
	// This is almost exactly the same as the InOut method above, but instead
	// of just affecting the camera and moving it towards the target, it moves 
	// both the camera and the target along the view vector.
}

void scene::BankViewer(float dx, float dy)
{
	// To Do
	//
	// This method banks the camera around the view vector.  You can do this by 
	// adding a small multiple of the vector to the right to the up vector and 
	// then renormalizing.
}

void scene::PushModelTransform(const matrix &m)
{
	ModelTransformStack.Push(m);
}

void scene::PushModelTransform()
{
	ModelTransformStack.Push(ModelTransformStack.Peek());
}

void scene::SetModelTransform(const matrix &m)
{
	ModelTransformStack.Pop();
	ModelTransformStack.Push(m);
}

matrix scene::PopModelTransform()
{
	return ModelTransformStack.Pop();
}

matrix scene::GetCurrentModelTransform()
{
	return ModelTransformStack.Peek();
}

void scene::PushSurfaceDiffuse(const rgb &diffuse)
{
	SurfaceDiffuseStack.Push(diffuse);
}

void scene::PushSurfaceDiffuse()
{
	SurfaceDiffuseStack.Push(SurfaceDiffuseStack.Peek());
}

void scene::SetSurfaceDiffuse(const rgb &diffuse)
{
	SurfaceDiffuseStack.Pop();
	SurfaceDiffuseStack.Push(diffuse);
}

rgb scene::PopSurfaceDiffuse()
{
	return SurfaceDiffuseStack.Pop();
}

rgb scene::GetCurrentSurfaceDiffuse()
{
	return SurfaceDiffuseStack.Peek();
}

void scene::PushSurfaceSpecular(const rgb &specular)
{
	SurfaceSpecularStack.Push(specular);
}

void scene::PushSurfaceSpecular()
{
	SurfaceSpecularStack.Push(SurfaceSpecularStack.Peek());
}

void scene::SetSurfaceSpecular(const rgb &specular)
{
	SurfaceSpecularStack.Pop();
	SurfaceSpecularStack.Push(specular);
}

rgb scene::PopSurfaceSpecular()
{
	return SurfaceSpecularStack.Pop();
}

rgb scene::GetCurrentSurfaceSpecular()
{
	return SurfaceSpecularStack.Peek();
}

void scene::PushSurfaceShininess(float shininess)
{
	SurfaceShininessStack.Push(shininess);
}

void scene::PushSurfaceShininess()
{
	SurfaceShininessStack.Push(SurfaceShininessStack.Peek());
}

void scene::SetSurfaceShininess(float shininess)
{
	SurfaceShininessStack.Pop();
	SurfaceShininessStack.Push(shininess);
}

float scene::PopSurfaceShininess()
{
	return SurfaceShininessStack.Pop();
}

float scene::GetCurrentSurfaceShininess()
{
	return SurfaceShininessStack.Peek();
}

void scene::PushAlphaTransparency(bool transparency)
{
	AlphaTransparencyStack.Push(transparency);
}

void scene::PushAlphaTransparency()
{
	AlphaTransparencyStack.Push(AlphaTransparencyStack.Peek());
}

void scene::SetAlphaTransparency(bool transparency)
{
	AlphaTransparencyStack.Pop();
	AlphaTransparencyStack.Push(transparency);
}

bool scene::PopAlphaTransparency()
{
	return AlphaTransparencyStack.Pop();
}

bool scene::GetCurrentAlphaTransparency()
{
	return AlphaTransparencyStack.Peek();
}

void scene::PushShader(shader *newShader)
{
	newShader->SetScene(this);
	ShaderStack.Push(newShader);
}

shader *scene::PopShader()
{
	return ShaderStack.Pop();
}

shader *scene::GetCurrentShader()
{
	return ShaderStack.Peek();
}

void scene::RegisterUniformParameters(int curProgram)
{
	int loc;


	/*Register the position of the Camera for PhongIllumination model*/

	// To Do 
	// 
	// You will need to "peek" the current values of the model transform from the stack, 
	// and then use the "view" and "projection" matrices calcualted in RenderScene to calculate 
	// the combined "ModelView" and "ModelViewProjection" transforms (remember we are in an M*p 
	// world, so be careful of the order you multiply them in!  Remember also that you want the 
	// MODEL transform to hit the object first, then VIEW, etc.)
	//
	// Use the model transform to compute the normal transform ... how is the normal transform computed?
	
	matrix modelTransform = ModelTransformStack.Peek();
	matrix modelViewTransform = view * modelTransform;
	matrix modelViewProjection = projection *  modelViewTransform;
	matrix normalTransform = (modelViewTransform.Inverse()).Transpose();
	
	// To Do 
	// 
	// Then you need to register each transform with OpenGL.  The uniform transforms supported by RenderGL are  
	//
	//     projection
	//     viewTransform
	//     modelTransform
	//     normalTransform
	//     modelViewTransform
	//     modelViewProjection   -- This is the one you will use in GLSL to project to the screen and it has been sent for you    
	// 
	// We will set ALL of them as 4x4 matrices for simplicity (even though the normalTransform could be a 
	// 3x3.  It would be quite a bit more complicated to send.

	loc = glGetUniformLocation(curProgram, "projection");
	glUniformMatrix4fv(loc, 1, false, projection.AsArray());

	loc = glGetUniformLocation(curProgram, "viewTransform");
	glUniformMatrix4fv(loc, 1, false, view.AsArray());

	loc = glGetUniformLocation(curProgram, "modelTransform");
	glUniformMatrix4fv(loc, 1, false, modelTransform.AsArray());

	loc = glGetUniformLocation(curProgram, "normalTransform");
	glUniformMatrix4fv(loc, 1, false, normalTransform.AsArray());

	loc = glGetUniformLocation(curProgram, "modelViewTransform");
	glUniformMatrix4fv(loc, 1, false, modelViewTransform.AsArray());

	loc = glGetUniformLocation(curProgram, "modelViewProjection");
	glUniformMatrix4fv(loc, 1, false, modelViewProjection.AsArray());

	// Next register the light parameters
	glUniform4fv(glGetUniformLocation(curProgram, "globalAmbient"), 1, GlobalAmbient.AsArray());
	glUniform1i(glGetUniformLocation(curProgram, "nLights"), nLights);

	for (int i = 0; i < nLights; i++)
		lights[i].RegisterUniformParameters(curProgram, view, i);

	rgb diffuse = SurfaceDiffuseStack.Peek();
	loc = glGetUniformLocation(curProgram, "surfaceDiffuse");
	glUniform4fv(loc, 1, diffuse.AsArray());

	// To Do
	//
	// You also need to peek the specular and shininess values from the repsective stacks and
	// set the corresponding uniform in GLSL
	loc = glGetUniformLocation(curProgram, "surfaceSpecular");
	glUniform4fv(loc, 1, SurfaceSpecularStack.Peek().AsArray());

	loc = glGetUniformLocation(curProgram, "surfaceShininess");
	glUniform1f(loc, SurfaceShininessStack.Peek());


	// Next register the surface parameters
	
	// Finally register the current time
	glUniform1f(glGetUniformLocation(curProgram, "time"), (float)time);

	GLERR;
}
