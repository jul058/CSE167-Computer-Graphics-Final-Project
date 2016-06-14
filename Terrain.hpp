//
//  Terrain.hpp
//  HW2
//
//  Created by Junxi Li on 5/25/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#ifndef Terrain_hpp
#define Terrain_hpp

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "Window.h"
#include <cstdlib>
#include <algorithm>
#include <climits>

class Terrain{
public:
    Terrain();
    void draw(GLuint shaderProgram);
    unsigned char* loadPPM(const char* filename, int& width, int& height);
    GLuint loadCubemap();
    void create();
    void change();
    
    bool prev_state;
    bool use_bump;
    float x_offset, y_offset, z_offset;
    GLfloat max_height, min_height;
    GLfloat max_x, max_z;
    std::vector<glm::vec3> v;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> normals2;
    std::vector<unsigned int> indicies;
    glm::mat4 toWorld;
    GLfloat **arr;
    
    
    GLuint cubemapTexture;
    GLuint textureID;
    GLuint terrainVAO, terrainVBO, terrainEBO, terrainNBO;
};

#endif /* Terrain_hpp */
