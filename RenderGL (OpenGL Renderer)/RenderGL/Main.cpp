//
// File:	Main.cpp
// Author:	John McDonald
//

#include <io.h> // For hooking up the console to stdio
#include <fcntl.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
using namespace std;

#include "Application.h"

void CreateConsole(void)
{
	int hCrt;
	FILE *hf;

	//////////////////////////////////////////////////////////////////////////
	// Create the console window
	//////////////////////////////////////////////////////////////////////////
	AllocConsole(); 

	//////////////////////////////////////////////////////////////////////////
	// Get the console's standard output handle and 
	// Connect stdout to that handle so that printf
	// Prints to the console
	//////////////////////////////////////////////////////////////////////////
	SetConsoleTitle("OORenderGL Console");	
	hCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	hf = _fdopen(hCrt, "w");
	*stdout = *hf;
	setvbuf(stdout, NULL, _IONBF, 0 );

	//////////////////////////////////////////////////////////////////////////
	// Get the console's standard input handle and 
	// Connect stdout to that handle so that scanf
	// Reads from the console
	//////////////////////////////////////////////////////////////////////////
	hCrt = _open_osfhandle((long)GetStdHandle(STD_INPUT_HANDLE), _O_TEXT);
	hf = _fdopen(hCrt, "r");
	*stdin = *hf;
	setvbuf(stdin, NULL, _IONBF, 0 );

	//////////////////////////////////////////////////////////////////////////
	// Get the console's standard error handle and 
	// Connect stdout to that handle so that fprintf(stderr, ...)
	// Prints to the console
	//////////////////////////////////////////////////////////////////////////
	hCrt = _open_osfhandle((long)GetStdHandle(STD_ERROR_HANDLE), _O_TEXT);
	hf = _fdopen(hCrt, "w");
	*stderr = *hf;
	setvbuf(stderr, NULL, _IONBF, 0 );

	//////////////////////////////////////////////////////////////////////////
	// Set cin, cout, cerr to all work with the console                     
	// Note that is suffices to call sync_with_stdio                        
	// Just with cout.  It is not necessary to repeat with cin and cerr     
	//////////////////////////////////////////////////////////////////////////
	cout.sync_with_stdio();
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
					  LPSTR lpCmdLine, int nShowCmd)
{
	//////////////////////////////////////////////////////////////////////////
	// Create the application's console first so that it will appear behind 
	//////////////////////////////////////////////////////////////////////////
	CreateConsole();
	cout << "RenderGL Object Oriented Rendering System\n";
	cout << "Copyright 2004 by John McDonald and Rosalee Wolfe\n";
	cout << "For use in DePaul University Computer Graphics Classes Only\n\n";

	//////////////////////////////////////////////////////////////////////////
	// Record the application's instance handle                             
	//////////////////////////////////////////////////////////////////////////
	Window::SetAppInstance(hInstance);

	//const char *files[] = { "gouraud.frag", "gouraud.vert", "phong.frag", "phong.vert", "simpleTex.frag", "simpleTex.vert" };

	//for (int i = 0; i < 6; i++)
	//{
	//	int size;
	//	unsigned char *buffer;
	//	FILE *shaderFile = fopen(files[i], "rb");
	//	if (shaderFile == NULL)
	//	{
	//		cout << "Unable to open shader file phong.vert" << endl;
	//		return NULL;
	//	}

	//	fseek(shaderFile, 0, SEEK_END);
	//	size = ftell(shaderFile);
	//	rewind(shaderFile);

	//	buffer = new unsigned char[size + 1];
	//	int num = fread(buffer, 1, size, shaderFile);
	//	buffer[num] = '\0';

	//	fclose(shaderFile);

	//	for (int j = 0; j < num; j++)
	//	{
	//		if (buffer[j] < 128)
	//			buffer[j] = 127 - buffer[j];
	//		else 
	//			buffer[j] = 127 + (128 - (buffer[j] - 128));
	//	}

	//	char newName[64];
	//	sprintf(newName, "%sE", files[i]);
	//	shaderFile = fopen(newName, "wb");
	//	fwrite(buffer, 1, size, shaderFile);
	//	fclose(shaderFile);
	//}

	//////////////////////////////////////////////////////////////////////////
	// Create the application (including the application window)            
	//////////////////////////////////////////////////////////////////////////
	Application app;

	//////////////////////////////////////////////////////////////////////////
	// Enter the event loop and wait for user events                        
	//////////////////////////////////////////////////////////////////////////
	EventLoop();

	return 0;
}

 