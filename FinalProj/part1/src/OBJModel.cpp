#include "OBJModel.hpp"
#include "Vertex.hpp"
#include "Triangle.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>


#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 

#include "util.hpp"
#include "globals.hpp"
//#include <GL/glew.h>

OBJModel::OBJModel(std::string fileName){
    LoadModel(fileName, vertices, triangles);	
}

OBJModel::~OBJModel(){
    // // Delete our OpenGL Objects
    // glDeleteBuffers(1, &gVertexBufferObjectFloor);
    // glDeleteVertexArrays(1, &gVertexArrayObjectFloor);

	// // Delete our Graphics pipeline
    // glDeleteProgram(mShaderID);
}

std::vector<Vertex> OBJModel::getVertices(){
    return vertices;
}

std::vector<Triangle> OBJModel::getTriangles(){
    return triangles;
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

void OBJModel::LoadModel(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<Triangle>& triangles) {
    std::cout << "loading OBJ " << filePath << "\n";

    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        std::cerr << "Cannot open file " << filePath << std::endl;
        return;
    }

    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec3> tempNormals;

    std::string myLine;
    while (std::getline(inFile, myLine)) {
        myLine = trim(myLine);

        if (myLine.empty() || myLine[0] == '#') {
            continue;
        }

        std::stringstream stream(myLine);
        std::string token;
        stream >> token;

        if (token == "v") {
            Vertex vertex;
            stream >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (token == "vn") {
            glm::vec3 normal;
            stream >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        } else if (token == "f") {
            std::cout << "\tface/triangle data incoming\n";
            Triangle triangle;

            for (int i = 0; i < 3; i++) {
                std::string faceData;
                stream >> faceData;

                std::stringstream faceDataStream(faceData);
                std::string indexString;

                // Parse vertex index
                std::getline(faceDataStream, indexString, '/');
                try {
                    int vIndex = std::stoi(indexString);
                    if (vIndex > 0 && vIndex <= vertices.size()) {
                        triangle.indices[i] = vIndex - 1;
                    } else {
                        std::cerr << "Invalid vertex index: " << vIndex << "\n";
                        continue;
                    }
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error converting string to integer: " << e.what() << "\n";
                    continue;
                }

                // Skip texture coordinate index (if exists)
                std::getline(faceDataStream, indexString, '/');

                // Parse normal index (if exists)
                if (std::getline(faceDataStream, indexString, '/') && !indexString.empty()) {
                    try {
                        int nIndex = std::stoi(indexString);
                        if (nIndex > 0 && nIndex <= tempNormals.size()) {
                            vertices[triangle.indices[i]].nx = tempNormals[nIndex - 1].x;
                            vertices[triangle.indices[i]].ny = tempNormals[nIndex - 1].y;
                            vertices[triangle.indices[i]].nz = tempNormals[nIndex - 1].z;
                        } else {
                            std::cerr << "Invalid normal index: " << nIndex << "\n";
                        }
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Error converting string to integer: " << e.what() << "\n";
                    }
                }
            }

            triangles.push_back(triangle);
            std::cout << "Loaded triangle indices: ";
            for (int i = 0; i < 3; i++) {
                std::cout << triangle.indices[i] << " ";
            }
            std::cout << "\n";
        }
    }

    inFile.close();
}


// void OBJModel::Initialize(){
//     // std::string vertexShaderSource      = LoadShaderAsString("./shaders/vert.glsl");
//     // std::string fragmentShaderSource    = LoadShaderAsString("./shaders/frag.glsl");

//     // mShaderID = CreateShaderProgram(vertexShaderSource,fragmentShaderSource);

//     // Convert your vertex data into a single contiguous std::vector<float>
//     std::vector<float> vertexData;
//     for (const auto& vertex : vertices) {
//         vertexData.push_back(vertex.x);
//         vertexData.push_back(vertex.y);
//         vertexData.push_back(vertex.z);
//         vertexData.push_back(vertex.nx);
//         vertexData.push_back(vertex.ny);
//         vertexData.push_back(vertex.nz);
//     }

//     // Vertex Arrays Object (VAO) Setup
//     glGenVertexArrays(1, &gVertexArrayObjectFloor);
//     // Bind (i.e. select) the Vertex Array Object (VAO) to work with
//     glBindVertexArray(gVertexArrayObjectFloor);
//     // Vertex Buffer Object (VBO) creation
//     glGenBuffers(1, &gVertexBufferObjectFloor);
//     // Bind the buffer
//     glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObjectFloor);
//     // Upload the data to the buffer
//     glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

//     glGenBuffers(1, &gEBO);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

//     // Vertex attributes setup
//     // Stride is now 6 floats since each vertex has position (3 floats) and normals (3 floats)

//     // Position information (x,y,z)
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (void*)0);

//     // Normal information (nx,ny,nz)
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (GLvoid*)(sizeof(GL_FLOAT) * 3));

//     // Unbind the currently bound Vertex Array Object
//     glBindVertexArray(0);

//     //Disable any attributes we enabled
//     glDisableVertexAttribArray(0);
//     glDisableVertexAttribArray(1);


//     std::string vertexShaderSource      = LoadShaderAsString("./shaders/vert.glsl");
//     std::string fragmentShaderSource    = LoadShaderAsString("./shaders/frag.glsl");

//     mShaderID = CreateShaderProgram(vertexShaderSource,fragmentShaderSource);
// }



/**
* PreDraw
* Typically we will use this for setting some sort of 'state'
* Note: some of the calls may take place at different stages (post-processing) of the
* 		 pipeline.
* @return void
*/
// void OBJModel::PreDraw(){
//     // Use our shader
// 	glUseProgram(mShaderID);

//     // Model transformation by translating our object into world space
//     glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f)); 


//     // Retrieve our location of our Model Matrix
//     GLint u_ModelMatrixLocation = glGetUniformLocation( mShaderID,"u_ModelMatrix");
//     if(u_ModelMatrixLocation >=0){
//         glUniformMatrix4fv(u_ModelMatrixLocation,1,GL_FALSE,&model[0][0]);
//     }else{
//         std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
//         exit(EXIT_FAILURE);
//     }


//     // Update the View Matrix
//     GLint u_ViewMatrixLocation = glGetUniformLocation(mShaderID,"u_ViewMatrix");
//     if(u_ViewMatrixLocation>=0){
//         glm::mat4 viewMatrix = g.gCamera.GetViewMatrix();
//         glUniformMatrix4fv(u_ViewMatrixLocation,1,GL_FALSE,&viewMatrix[0][0]);
//     }else{
//         std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
//         exit(EXIT_FAILURE);
//     }


//     // Projection matrix (in perspective) 
//     glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
//                                              (float)g.gScreenWidth/(float)g.gScreenHeight,
//                                              0.1f,
//                                              20.0f);

//     // Retrieve our location of our perspective matrix uniform 
//     GLint u_ProjectionLocation= glGetUniformLocation( mShaderID,"u_Projection");
//     if(u_ProjectionLocation>=0){
//         glUniformMatrix4fv(u_ProjectionLocation,1,GL_FALSE,&perspective[0][0]);
//     }else{
//         std::cout << "Could not find u_Perspective, maybe a mispelling?\n";
//         exit(EXIT_FAILURE);
//     }

     // Setup the Lights 
    // GLint loc = glGetUniformLocation( mShaderID,"u_LightPos");
    // if(loc >=0){
    //     glUniform3fv(loc, 1, &g.gLight.startPosition[0]);
    // }else{
    //     std::cout << "Could not find u_LightPos" << std::endl;
    //     exit(EXIT_FAILURE);
    // }
//}


/**
* Draw
* The render function gets called once per loop.
* Typically this includes 'glDraw' related calls, and the relevant setup of buffers
* for those calls.
*
* @return void
*/
// void OBJModel::Draw(){
//     // Enable our attributes
// 	glBindVertexArray(gVertexArrayObjectFloor);

//     //Render data
//     // glDrawArrays(GL_TRIANGLES,0,vertices.size());
//     glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

//     // std::vector<Vertex> OBJModel::getVertices(){
//     //     return vertices;
//     // }

//     // std::vector<Triangle> OBJModel::getTriangles(){
//     //     return triangles;
//     // }

//     // getVertices();
//     // getTriangles();

//     // for (const auto& tri : triangles) {
//     //     indices.insert(indices.end(), std::begin(tri.indices), std::end(tri.indices));
//     // }

//     //g.gLight.PreDraw();
//     //g.gLight.Draw();

// 	// Stop using our current graphics pipeline
// 	// Note: This is not necessary if we only have one graphics pipeline.
//     glUseProgram(0);
// }







