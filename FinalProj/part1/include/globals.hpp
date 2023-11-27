#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif


#include "Camera.hpp"
#include "Light.hpp"
#include "OBJModel.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Vertex.hpp"
#include "Triangle.hpp"

#include <glad/glad.h>
#include "util.hpp"

// Forward Declaration

class OBJModel;

struct Global{
		// Screen Dimensions
		int gScreenWidth 						= 640;
		int gScreenHeight 						= 480;
		SDL_Window* gGraphicsApplicationWindow 	= nullptr;
		SDL_GLContext gOpenGLContext			= nullptr;

		// Main loop flag
		bool gQuit = false; // If this is quit = 'true' then the program terminates.

		// Camera
		Camera gCamera;

		// Draw wireframe mode
		GLenum gPolygonMode = GL_FILL;

		// Light object
		Light gLight;

		//Obj object
		OBJModel* gBunny;

		 //Global() : gBunny(nullptr) { }


		
};

// External linkage such that the
// global variable is available
// everywhere.
extern Global g;

#endif