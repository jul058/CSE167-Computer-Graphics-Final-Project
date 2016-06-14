//
//  Controller.hpp
//  HW2
//
//  Created by Junxi Li on 5/30/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#ifndef Controller_hpp
#define Controller_hpp

#include <stdio.h>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "Window.h"
#include "OBJObject.h"
#include "Terrain.hpp"
#include "Box.hpp"

class Controller{
public:
    Controller(OBJObject*, Terrain*, int, int, GLfloat,  GLfloat XD, GLfloat YD, GLfloat ZD, GLfloat y_offset, glm::vec3 board_translation, glm::vec3 board_scale);
    void move(int mode);
    void draw(GLuint, glm::vec3);
    void direction();

    
    GLfloat SIZE;

    
    bool first;
    glm::vec3 board_translation;
    glm::vec3 board_scale;
    glm::vec3 baord_rotation;
    GLfloat y_offset;
    Box* box;
    glm::vec3 curr_point;
    glm::vec3 last_point;
    GLfloat orientation;
    GLfloat speed;
    GLfloat prev_height;
    GLfloat curr_height;
    int x, z;
    GLfloat y;
    Terrain * ter;
    OBJObject* obj;
    glm::mat4 toWorld;
    GLfloat gravity;
    GLfloat velocity;
    GLuint VAO, VBO, NBO, EBO;
};

#endif /* Controller_hpp */
