#include "SDLGraphicsProgram.hpp"
#include "Camera.hpp"
//#include "Terrain.hpp"
// #include "Sphere.hpp"
#include "ObjectManager.hpp"
#include "SceneNode.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h, int argc, char* argv[]){

    for(int i=1; i<argc; i++) {
        myOBJPath.push_back(argv[i]);
    }

	// Initialization flag
	bool success = true;
	// String to hold any errors that occur.
	std::stringstream errorStream;
	// The window we'll be rendering to
	m_window = NULL;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// We want to request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		m_window = SDL_CreateWindow( "Leigh-Riane Amsterdam Final Project v2",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                w,
                                h,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		// Check if Window did not create.
		if( m_window == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		m_openGLContext = SDL_GL_CreateContext( m_window );
		if( m_openGLContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		// Initialize GLAD Library
		if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
			errorStream << "Failed to iniitalize GLAD\n";
			success = false;
		}

		//Initialize OpenGL
		if(!InitGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}
  	}

    // If initialization did not work, then print out a list of errors in the constructor.
    if(!success){
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors=errorStream.str();
        SDL_Log("%s\n",errors.c_str());
    }else{
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
	GetOpenGLVersionInfo();


    // Setup our Renderer
    m_renderer = new Renderer(w,h); 


}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    if(m_renderer!=nullptr){
        delete m_renderer;
    }


    //Destroy window
	SDL_DestroyWindow( m_window );
	// Point m_window to NULL to ensure it points to nothing.
	m_window = nullptr;
	//Quit SDL subsystems
	SDL_Quit();
}


// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::InitGL(){
	//Success flag
	bool success = true;

	return success;
}


// ====================== Create the planets =============
Object* my_obj;
SceneNode* sn_obj;

// Object* my_objLeaves;
// SceneNode* sn_objLeaves;

//Loops forever!
void SDLGraphicsProgram::Loop(){

    // ================== Initialize the planets ===============

    my_obj = new Object(myOBJPath[0]);
    my_obj->MakeTexture();
    sn_obj = new SceneNode(my_obj);


    
    m_renderer->setRoot(sn_obj);

    //sn_obj->AddChild(sn_objLeaves);
    
    // Set a default position for our camera
    m_renderer->GetCamera(0)->SetCameraEyePosition(-21.85f,0.0f,9.080f);
    //m_renderer->GetCamera(0)->SetCameraEyePosition(125.0f,50.0f,500.0f);

    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

    // Set the camera speed for how fast we move.
    float cameraSpeed = 0.5f;

    // While application is running
    while(!quit){

        //Handle events on queue
        while(SDL_PollEvent( &e ) != 0){
            // User posts an event to quit
            // An example is hitting the "x" in the corner of the window.
            if(e.type == SDL_QUIT){
                quit = true;
            }
            // Handle keyboad input for the camera class
            if(e.type==SDL_MOUSEMOTION && mouseToggle){
                // Handle mouse movements
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
                m_renderer->GetCamera(0)->MouseLook(mouseX, mouseY);
            }
            switch(e.type){
                // Handle keyboard presses
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case SDLK_LEFT:
                            m_renderer->GetCamera(0)->MoveLeft(cameraSpeed);
                            break;
                        // case SDLK_RIGHT:
                        //     m_renderer->GetCamera(0)->MoveRight(cameraSpeed);
                        //     break;
                        case SDLK_UP:
                            m_renderer->GetCamera(0)->MoveForward(cameraSpeed);
                            break;
                        case SDLK_DOWN:
                            m_renderer->GetCamera(0)->MoveBackward(cameraSpeed);
                            break;
                        case SDLK_RSHIFT:
                            m_renderer->GetCamera(0)->MoveUp(cameraSpeed);
                            break;
                        case SDLK_LSHIFT:
                            m_renderer->GetCamera(0)->MoveDown(cameraSpeed);
                            break;
                        case SDLK_q:
                            quit = true;
                            break;
                        case SDLK_m:
                            mouseToggle = !mouseToggle;
                            break;
                        case SDLK_RIGHT:
                            SceneNode::IncrementCellShading(0.1f); // Increment cellShading by 0.1
                            if (sn_obj != nullptr) {
                                sn_obj->ToggleCellShading(); // Apply the updated cellShading value
                            }
                    }
                break;
            }
        } // End SDL_PollEvent loop.

        sn_obj->GetLocalTransform().LoadIdentity();
        sn_obj->GetLocalTransform().Translate(0.0f, 0.0f, -6.0f);
        //sn_obj->GetLocalTransform().Rotate(rotate_scale, 0.0f, 1.0f, 0.0f);

        //sn_objLeaves->GetLocalTransform().LoadIdentity();
        //sn_objLeaves->GetLocalTransform().Translate(0.0f, 0.0f, -6.0f);

        rotate_scale += 0.01;
        

        // Update our scene through our renderer
        m_renderer->Update();
        // Render our scene using our selected renderer
        m_renderer->Render();
        // Delay to slow things down just a bit!
        SDL_Delay(25);  // TODO: You can change this or implement a frame
                        // independent movement method if you like.
      	//Update screen of our specified window
      	SDL_GL_SwapWindow(GetSDLWindow());
	}
    //Disable text input
    SDL_StopTextInput();
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
