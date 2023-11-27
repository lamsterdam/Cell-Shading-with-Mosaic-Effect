/* Compilation on Linux: 
 g++ -std=c++17 ./src/*.cpp -o prog -I ./include/ -I./../common/thirdparty/ -lSDL2 -ldl
*/

// Third Party Libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "OBJModel.hpp"
#include "Vertex.hpp"
#include "Triangle.hpp"

// C++ Standard Template Library (STL)
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>
// #include <numbers>

// Our libraries
#include "Camera.hpp"
#include "Light.hpp"
#include "util.hpp"

// vvvvvvvvvvvvvvvvvvvvvvvvvv Globals vvvvvvvvvvvvvvvvvvvvvvvvvv
// Globals generally are prefixed with 'g' in this application.

// Screen Dimensions
// int gScreenWidth 						= 640;
// int gScreenHeight 						= 480;
// SDL_Window* gGraphicsApplicationWindow 	= nullptr;
// SDL_GLContext gOpenGLContext			= nullptr;

// Main loop flag
//bool gQuit = false; // If this is quit = 'true' then the program terminates.

// shader
// The following stores the a unique id for the graphics pipeline
// program object that will be used for our OpenGL draw calls.
GLuint gGraphicsPipelineShaderProgram	= 0;

// OpenGL Objects
// Vertex Array Object (VAO)
// Vertex array objects encapsulate all of the items needed to render an object.
// For example, we may have multiple vertex buffer objects (VBO) related to rendering one
// object. The VAO allows us to setup the OpenGL state to render that object using the
// correct layout and correct buffers with one call after being setup.
GLuint gVertexArrayObjectFloor= 0;
// Vertex Buffer Object (VBO)
// Vertex Buffer Objects store information relating to vertices (e.g. positions, normals, textures)
// VBOs are our mechanism for arranging geometry on the GPU.
GLuint  gVertexBufferObjectFloor            = 0;
GLuint gEBO = 0;

// Camera
//Camera gCamera;

// Draw wireframe mode
//GLenum gPolygonMode = GL_LINE;

// Floor resolution
size_t gFloorResolution = 10;
size_t gFloorTriangles  = 0;

std::vector<Vertex> vertices;
std::vector<Triangle> triangles;
std::vector<GLuint> indices;


#include "globals.hpp"
// // ^^^^^^^^^^^^^^^^^^^^^^^^ Globals ^^^^^^^^^^^^^^^^^^^^^^^^^^^



// vvvvvvvvvvvvvvvvvvv Error Handling Routines vvvvvvvvvvvvvvv
// static void GLClearAllErrors(){
//     while(glGetError() != GL_NO_ERROR){
//     }
// }

// Returns true if we have an error
// static bool GLCheckErrorStatus(const char* function, int line){
//     while(GLenum error = glGetError()){
//         std::cout << "OpenGL Error:" << error 
//                   << "\tLine: " << line 
//                   << "\tfunction: " << function << std::endl;
//         return true;
//     }
//     return false;
// }

//#define GLCheck(x) GLClearAllErrors(); x; GLCheckErrorStatus(#x,__LINE__);
// ^^^^^^^^^^^^^^^^^^^ Error Handling Routines ^^^^^^^^^^^^^^^



// /**
// * LoadShaderAsString takes a filepath as an argument and will read line by line a file and return a string that is meant to be compiled at runtime for a vertex, fragment, geometry, tesselation, or compute shader.
// * e.g.
// *       LoadShaderAsString("./shaders/filepath");
// *
// * @param filename Path to the shader file
// * @return Entire file stored as a single string 
// */
// std::string LoadShaderAsString(const std::string& filename){
//     // Resulting shader program loaded as a single string
//     std::string result = "";

//     std::string line = "";
//     std::ifstream myFile(filename.c_str());

//     if(myFile.is_open()){
//         while(std::getline(myFile, line)){
//             result += line + '\n';
//         }
//         myFile.close();

//     }

//     return result;
// }


// /**
// * CompileShader will compile any valid vertex, fragment, geometry, tesselation, or compute shader.
// * e.g.
// *	    Compile a vertex shader: 	CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
// *       Compile a fragment shader: 	CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
// *
// * @param type We use the 'type' field to determine which shader we are going to compile.
// * @param source : The shader source code.
// * @return id of the shaderObject
// */
// GLuint CompileShader(GLuint type, const std::string& source){
// 	// Compile our shaders
// 	GLuint shaderObject;

// 	// Based on the type passed in, we create a shader object specifically for that
// 	// type.
// 	if(type == GL_VERTEX_SHADER){
// 		shaderObject = glCreateShader(GL_VERTEX_SHADER);
// 	}else if(type == GL_FRAGMENT_SHADER){
// 		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
// 	}

// 	const char* src = source.c_str();
// 	// The source of our shader
// 	glShaderSource(shaderObject, 1, &src, nullptr);
// 	// Now compile our shader
// 	glCompileShader(shaderObject);

// 	// Retrieve the result of our compilation
// 	int result;
// 	// Our goal with glGetShaderiv is to retrieve the compilation status
// 	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

// 	if(result == GL_FALSE){
// 		int length;
// 		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
// 		char* errorMessages = new char[length]; // Could also use alloca here.
// 		glGetShaderInfoLog(shaderObject, length, &length, errorMessages);

// 		if(type == GL_VERTEX_SHADER){
// 			std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n" << errorMessages << "\n";
// 		}else if(type == GL_FRAGMENT_SHADER){
// 			std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n" << errorMessages << "\n";
// 		}
// 		// Reclaim our memory
// 		delete[] errorMessages;

// 		// Delete our broken shader
// 		glDeleteShader(shaderObject);

// 		return 0;
// 	}

//   return shaderObject;
// }



// /**
// * Creates a graphics program object (i.e. graphics pipeline) with a Vertex Shader and a Fragment Shader
// *
// * @param vertexShaderSource Vertex source code as a string
// * @param fragmentShaderSource Fragment shader source code as a string
// * @return id of the program Object
// */
// GLuint CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource){

//     // Create a new program object
//     GLuint programObject = glCreateProgram();

//     // Compile our shaders
//     GLuint myVertexShader   = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
//     GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

//     // Link our two shader programs together.
// 	// Consider this the equivalent of taking two .cpp files, and linking them into
// 	// one executable file.
//     glAttachShader(programObject,myVertexShader);
//     glAttachShader(programObject,myFragmentShader);
//     glLinkProgram(programObject);

//     // Validate our program
//     glValidateProgram(programObject);

//     // Once our final program Object has been created, we can
// 	// detach and then delete our individual shaders.
//     glDetachShader(programObject,myVertexShader);
//     glDetachShader(programObject,myFragmentShader);
// 	// Delete the individual shaders once we are done
//     glDeleteShader(myVertexShader);
//     glDeleteShader(myFragmentShader);

//     return programObject;
// }


/**
* Create the graphics pipeline
*
* @return void
*/
void CreateGraphicsPipeline(){

    std::string vertexShaderSource      = LoadShaderAsString("./shaders/vert.glsl");
    std::string fragmentShaderSource    = LoadShaderAsString("./shaders/frag.glsl");

	gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource,fragmentShaderSource);
}


/**
* Initialization of the graphics application. Typically this will involve setting up a window
* and the OpenGL Context (with the appropriate version)
*
* @return void
*/
void InitializeProgram(){
	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		exit(1);
	}
	
	// Setup the OpenGL Context
	// Use OpenGL 4.1 core or greater
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	// We want to request a double buffer for smooth updating.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create an application window using OpenGL that supports SDL
	g.gGraphicsApplicationWindow = SDL_CreateWindow( "Tesselation",
													SDL_WINDOWPOS_UNDEFINED,
													SDL_WINDOWPOS_UNDEFINED,
													g.gScreenWidth,
													g.gScreenHeight,
													SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

	// Check if Window did not create.
	if( g.gGraphicsApplicationWindow == nullptr ){
		std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
		exit(1);
	}

	// Create an OpenGL Graphics Context
	g.gOpenGLContext = SDL_GL_CreateContext( g.gGraphicsApplicationWindow );
	if( g.gOpenGLContext == nullptr){
		std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
		exit(1);
	}

	// Initialize GLAD Library
	if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
		std::cout << "glad did not initialize" << std::endl;
		exit(1);
	}

    // Setup Light(s)
    //g.gLight.Initialize();

	//g.gBunny->Initialize();
    //g.gBunny->VertexSpecification();

    
	
	
}


// struct Vertex{
//     float x,y,z;    // position
// 	float r,g,b; 	// color
// 	float nx,ny,nz; // normals
// };  

// struct Triangle{
//     Vertex vertices[3]; // 3 vertices per triangle
// };



// Return a value that is a mapping between the current range and a new range.
// Source: https://www.arduino.cc/reference/en/language/functions/math/map/
float map_linear(float x, float in_min, float in_max, float out_min, float out_max){
    return (x-in_min) * (out_max - out_min) / (in_max - in_min) + out_min;;
}

// Pass in an unsigned integer representing the number of
// rows and columns in the plane (e.g. resolution=00)
// The plane is 'flat' so the 'z' position will be 0.0f;
std::vector<Triangle> generatePlane(size_t resolution=0){
 
    // Store the resulting plane 
    std::vector<Triangle> result;

    return result;
}




// Regenerate the flat plane
// void GeneratePlaneBufferData(){
//     // Generate a plane with the resolution 
//     std::vector<Triangle> mesh = generatePlane(gFloorResolution); 

// 		std::vector<GLfloat> vertexDataFloor;


// 		// Store size in a global so you can later determine how many
// 		// vertices to draw in glDrawArrays;
//     gFloorTriangles = vertexDataFloor.size();

// 		glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObjectFloor);
// 		glBufferData(GL_ARRAY_BUFFER, 						// Kind of buffer we are working with 
// 																							// (e.g. GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER)
// 							 vertexDataFloor.size() * sizeof(GL_FLOAT), 	// Size of data in bytes
// 							 vertexDataFloor.data(), 											// Raw array of data
// 							 GL_STATIC_DRAW);															// How we intend to use the data
// }

/**
* Setup your geometry during the vertex specification step
*
* @return void
*/
/**
* Setup your geometry during the vertex specification step
*
* @return void
*/
void VertexSpecification() {
    // Convert your vertex data into a single contiguous std::vector<float>
    std::vector<float> vertexData;
    for (const auto& vertex : vertices) {
        vertexData.push_back(vertex.x);
        vertexData.push_back(vertex.y);
        vertexData.push_back(vertex.z);
        vertexData.push_back(vertex.nx);
        vertexData.push_back(vertex.ny);
        vertexData.push_back(vertex.nz);
    }

    // Vertex Arrays Object (VAO) Setup
    glGenVertexArrays(1, &gVertexArrayObjectFloor);
    // Bind (i.e. select) the Vertex Array Object (VAO) to work with
    glBindVertexArray(gVertexArrayObjectFloor);
    // Vertex Buffer Object (VBO) creation
    glGenBuffers(1, &gVertexBufferObjectFloor);
    // Bind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObjectFloor);
    // Upload the data to the buffer
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &gEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Vertex attributes setup
    // Stride is now 6 floats since each vertex has position (3 floats) and normals (3 floats)

    // Position information (x,y,z)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (void*)0);

    // Normal information (nx,ny,nz)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (GLvoid*)(sizeof(GL_FLOAT) * 3));

    // Unbind the currently bound Vertex Array Object
    glBindVertexArray(0);

    // Disable any attributes we enabled
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}


/**
* PreDraw
* Typically we will use this for setting some sort of 'state'
* Note: some of the calls may take place at different stages (post-processing) of the
* 		 pipeline.
* @return void
*/
void PreDraw(){
 	// Disable depth test and face culling.
       glEnable(GL_DEPTH_TEST);                    // NOTE: Need to enable DEPTH Test
       glDisable(GL_CULL_FACE);

    // Set the polygon fill mode
       glPolygonMode(GL_FRONT_AND_BACK, g.gPolygonMode);

    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, g.gScreenWidth, g.gScreenHeight);
    glClearColor( 0.1f, 4.f, 7.f, 1.f );

    //Clear color buffer and Depth Buffer
  	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    

    // Use our shader
	glUseProgram(gGraphicsPipelineShaderProgram);

    // Model transformation by translating our object into world space
    glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f)); 


    // Retrieve our location of our Model Matrix
    GLint u_ModelMatrixLocation = glGetUniformLocation( gGraphicsPipelineShaderProgram,"u_ModelMatrix");
    if(u_ModelMatrixLocation >=0){
        glUniformMatrix4fv(u_ModelMatrixLocation,1,GL_FALSE,&model[0][0]);
    }else{
        std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }


    // Update the View Matrix
    GLint u_ViewMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram,"u_ViewMatrix");
    if(u_ViewMatrixLocation>=0){
        glm::mat4 viewMatrix = g.gCamera.GetViewMatrix();
        glUniformMatrix4fv(u_ViewMatrixLocation,1,GL_FALSE,&viewMatrix[0][0]);
    }else{
        std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }


    // Projection matrix (in perspective) 
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
                                             (float)g.gScreenWidth/(float)g.gScreenHeight,
                                             0.1f,
                                             20.0f);

    // Retrieve our location of our perspective matrix uniform 
    GLint u_ProjectionLocation= glGetUniformLocation( gGraphicsPipelineShaderProgram,"u_Projection");
    if(u_ProjectionLocation>=0){
        glUniformMatrix4fv(u_ProjectionLocation,1,GL_FALSE,&perspective[0][0]);
    }else{
        std::cout << "Could not find u_Perspective, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

         // Setup the Lights 
    // GLint loc = glGetUniformLocation( gGraphicsPipelineShaderProgram,"u_LightPos");
    // if(loc >=0){
    //     glUniform3fv(loc, 1, &g.gLight.startPosition[0]);
    // }else{
    //     std::cout << "Could not find u_LightPos" << std::endl;
    //     exit(EXIT_FAILURE);
    // }
}


/**
* Draw
* The render function gets called once per loop.
* Typically this includes 'glDraw' related calls, and the relevant setup of buffers
* for those calls.
*
* @return void
*/
void Draw(){
    // Enable our attributes
	glBindVertexArray(gVertexArrayObjectFloor);

    //Render data
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


	// Stop using our current graphics pipeline
	// Note: This is not necessary if we only have one graphics pipeline.
    glUseProgram(0);



    //new draw:
    //g.gBunny->PreDraw();
    //g.gBunny->Draw();

    

    //g.gLight.PreDraw();
    //g.gLight.Draw();

}

// // Function to log OpenGL errors to a file
// void LogOpenGLErrorToFile(const GLenum error, const std::string& logFilePath) {
//     std::ofstream logFile(logFilePath, std::ios::app);
//     if (logFile.is_open()) {
//         logFile << "OpenGL error: " << error << std::endl;
//         logFile.close();
//     }
// }

/**
* Helper Function to get OpenGL Version Information
*
* @return void
*/
void getOpenGLVersionInfo(){
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}


/**
* Function called in the Main application loop to handle user input
*
* @return void
*/
void Input(){
    // Two static variables to hold the mouse position
    static int mouseX=g.gScreenWidth/2;
    static int mouseY=g.gScreenHeight/2; 

	// Event handler that handles various events in SDL
	// that are related to input and output
	SDL_Event e;
	//Handle events on queue
	while(SDL_PollEvent( &e ) != 0){
		// If users posts an event to quit
		// An example is hitting the "x" in the corner of the window.
		if(e.type == SDL_QUIT){
			std::cout << "Goodbye! (Leaving MainApplicationLoop())" << std::endl;
			g.gQuit = true;
		}
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
			std::cout << "ESC: Goodbye! (Leaving MainApplicationLoop())" << std::endl;
            g.gQuit = true;
        }
        if(e.type==SDL_MOUSEMOTION){
            // Capture the change in the mouse position
            mouseX+=e.motion.xrel;
            mouseY+=e.motion.yrel;
            g.gCamera.MouseLook(mouseX,mouseY);
        }
	}

    // Retrieve keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP]) {
        SDL_Delay(250);
        gFloorResolution+=1;
        std::cout << "Resolution:" << gFloorResolution << std::endl;
        //GeneratePlaneBufferData();
    }
    if (state[SDL_SCANCODE_DOWN]) {
        SDL_Delay(250); 
        gFloorResolution-=1;
        if(gFloorResolution<=1){
            gFloorResolution=1;
        }
        std::cout << "Resolution:" << gFloorResolution << std::endl;
        //GeneratePlaneBufferData();
    }

    // Camera
    // Update our position of the camera
    if (state[SDL_SCANCODE_W]) {
        g.gCamera.MoveForward(0.001f);
    }
    if (state[SDL_SCANCODE_S]) {
        g.gCamera.MoveBackward(0.001f);
    }
    if (state[SDL_SCANCODE_A]) {
    }
    if (state[SDL_SCANCODE_D]) {
    }

    if (state[SDL_SCANCODE_TAB]) {
        SDL_Delay(250); // This is hacky in the name of simplicity,
                       // but we just delay the
                       // system by a few milli-seconds to process the 
                       // keyboard input once at a time.
        if(g.gPolygonMode== GL_FILL){
            g.gPolygonMode = GL_LINE;
        }else{
            g.gPolygonMode = GL_FILL;
        }
    }
}


/**
* Main Application Loop
* This is an infinite loop in our graphics application
*
* @return void
*/
void MainLoop(){

    // Little trick to map mouse to center of screen always.
    // Useful for handling 'mouselook'
    // This works because we effectively 're-center' our mouse at the start
    // of every frame prior to detecting any mouse motion.
    SDL_WarpMouseInWindow(g.gGraphicsApplicationWindow,g.gScreenWidth/2,g.gScreenHeight/2);
    SDL_SetRelativeMouseMode(SDL_TRUE);


	// While application is running
	while(!g.gQuit){
		// Handle Input
		Input();
		// Setup anything (i.e. OpenGL State) that needs to take
		// place before draw calls
		PreDraw();
		// Draw Calls in OpenGL
        // When we 'draw' in OpenGL, this activates the graphics pipeline.
        // i.e. when we use glDrawElements or glDrawArrays,
        //      The pipeline that is utilized is whatever 'glUseProgram' is
        //      currently binded.
		Draw();

        

		//Update screen of our specified window
		SDL_GL_SwapWindow(g.gGraphicsApplicationWindow);
	}
}



/**
* The last function called in the program
* This functions responsibility is to destroy any global
* objects in which we have create dmemory.
*
* @return void
*/
void CleanUp(){
	//Destroy our SDL2 Window
	SDL_DestroyWindow(g.gGraphicsApplicationWindow );
	g.gGraphicsApplicationWindow = nullptr;

    // Delete our OpenGL Objects
    // glDeleteBuffers(1, &gVertexBufferObjectFloor);
    // glDeleteVertexArrays(1, &gVertexArrayObjectFloor);

	// // Delete our Graphics pipeline
    // glDeleteProgram(gGraphicsPipelineShaderProgram);

    

	//Quit SDL subsystems
	SDL_Quit();
}


/**
* The entry point into our C++ programs.
*
* @return program status
*/
int main( int argc, char* args[] ){
    std::cout << "Use w and s keys to move forward and back\n";
    std::cout << "Use up and down to change tessellation\n";
    std::cout << "Use 1 to toggle wireframe\n";
    std::cout << "Press ESC to quit\n";


     if(argc < 2){
        std::cout << "Please specific an obj model to load" << std::endl;
        return 0;
    }

    std::cout << "program name is argv[0]: " << args[0] <<"\n";
    std::cout << "file path is: " << args[1] << "\n";
    std::string modelfFilePath(args[1]);

    OBJModel objModel(modelfFilePath);

    
    // vertices = g.gBunny->getVertices();
    // triangles = g.gBunny->getTriangles();

    vertices = objModel.getVertices();
    triangles = objModel.getTriangles();
    for (const auto& tri : triangles) {
        indices.insert(indices.end(), std::begin(tri.indices), std::end(tri.indices));
    }


	// 1. Setup the graphics program
	InitializeProgram();
	
	// 2. Setup our geometry
	VertexSpecification();
    //g.gBunny->VertexSpecification();
	
	// 3. Create our graphics pipeline
	// 	- At a minimum, this means the vertex and fragment shader
	CreateGraphicsPipeline();
    
	
	// 4. Call the main application loop
	MainLoop();	

	// 5. Call the cleanup function when our program terminates
	CleanUp();

	return 0;
}
