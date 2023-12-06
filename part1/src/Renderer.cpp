#include "Renderer.hpp"


// Sets the height and width of our renderer
Renderer::Renderer(unsigned int w, unsigned int h){
    m_screenWidth = w;
    m_screenHeight = h;

    // By default create one camera per render
    // TODO: You could abstract out further functions to create
    //       a camera as a scene node and attach them at various levels.
    Camera* defaultCamera = new Camera();
    // Add our single camera
    m_cameras.push_back(defaultCamera);

    m_root = nullptr;

    // By derfaflt create one framebuffer within the renderer.
    //Framebuffer
    Framebuffer* newFramebuffer = new Framebuffer();
    newFramebuffer->Create(w,h);
    m_framebuffers.push_back(newFramebuffer);
}

// Sets the height and width of our renderer
Renderer::~Renderer(){
    // Delete all of our camera pointers
    for(int i=0; i < m_cameras.size(); i++){
        delete m_cameras[i];
    }
}

void Renderer::Update(){
    // Here we apply the projection matrix which creates perspective.
    // The first argument is 'field of view'
    // Then perspective
    // Then the near and far clipping plane.
    // Note I cannot see anything closer than 0.1f units from the screen.
    m_projectionMatrix = glm::perspective(45.0f,((float)m_screenWidth)/((float)m_screenHeight),0.1f,512.0f);

    // Perform the update
    if(m_root!=nullptr){
        // TODO: By default, we will only have one camera
        //       You may otherwise not want to hardcode
        //       a value of '0' here.
        m_root->Update(m_projectionMatrix, m_cameras[0]);
    }
}

// Initialize clear color
// Setup our OpenGL State machine
// Then render the scene
void Renderer::Render(){
     // Setup our uniforms
    // In reality, only need to do this once for this
    // particular fbo because the texture data is 
    // not going to change.
    // NOTE:
    //       Assume for this implementation we only have at most
    //       One framebuffer
    // m_framebuffers[0]->Update();
    // // Bind to our farmebuffer
    // m_framebuffers[0]->Bind();

    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor( 0.2f, 0.2f, 0.2f, 1.f );
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    m_root->DrawOutline();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    m_root->Draw();

    // Finish with our framebuffer
    //m_framebuffers[0]->Unbind();

    // Clear everything away
    // Clear the screen color, and typically I do this
    // to something 'different' than our original as an
    // indication that I am in a FBO. But you may choose
    // to match the glClearColor
    // glClearColor(1.0f,1.0f,1.0f,1.0f);
    // // We only have 'color' in our buffer that is stored
    // glClear(GL_COLOR_BUFFER_BIT); 
    // // Use our new 'simple screen shader'
    // m_framebuffers[0]->m_fboShader->Bind();
    // // Overlay our 'quad' over the screen
    // m_framebuffers[0]->DrawFBO();    
    // // Unselect our shader and continue
    // m_framebuffers[0]->m_fboShader->Unbind();
}

// Determines what the root is of the renderer, so the
// scene can be drawn.
void Renderer::setRoot(SceneNode* startingNode){
    m_root = startingNode;
}


