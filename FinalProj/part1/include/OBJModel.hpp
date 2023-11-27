#ifndef OBJ_HPP
#define OBJ_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Vertex.hpp"
#include "Triangle.hpp"

#include <glad/glad.h>
#include "util.hpp"
#include "globals.hpp"

class OBJModel {
    public:
        // Constructor loads a filename with .obj extension
        OBJModel(std::string fileName);
    
        
        ~OBJModel();
    
        void LoadModel(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<Triangle>& triangles);
        
        std::vector<Vertex> getVertices();
        std::vector<Triangle> getTriangles();

        // void Initialize();
        // void PreDraw();
        // void Draw();
 
 
    private:
        std::vector<Vertex> vertices;
        std::vector<Triangle> triangles;
        std::vector<GLuint> indices;
        //GLuint mShaderID	= 0;

        // OpenGL Objects
        // Vertex Array Object (VAO)
        // Vertex array objects encapsulate all of the items needed to render an object.
        // For example, we may have multiple vertex buffer objects (VBO) related to rendering one
        // object. The VAO allows us to setup the OpenGL state to render that object using the
        // correct layout and correct buffers with one call after being setup.
        //GLuint gVertexArrayObjectFloor              = 0;
        // Vertex Buffer Object (VBO)
        // Vertex Buffer Objects store information relating to vertices (e.g. positions, normals, textures)
        // VBOs are our mechanism for arranging geometry on the GPU.
        //GLuint  gVertexBufferObjectFloor            = 0;
        //GLuint gEBO = 0;
    
        
};
 




#endif