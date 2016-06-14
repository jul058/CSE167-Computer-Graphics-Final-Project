//
//  Box.hpp
//  HW2
//
//  Created by Junxi Li on 5/27/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#ifndef Box_hpp
#define Box_hpp

#include <cstdio>
#include <string>
#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cylinder.hpp"
#include "OBJObject.h"
#include "Cube.h"

class Box{
public:
    OBJObject* obj;
    std::vector<glm::vec3> v;
    std::vector<glm::vec3> w;
    GLuint VAO, VBO;
    GLint collideID;
    GLfloat XD, YD, ZD;
    GLfloat x_scale, y_scale, z_scale;

    
    
    Box(OBJObject*, GLfloat, GLfloat, GLfloat);
    void init();
    void draw(GLuint);
    void move(glm::vec3);
    bool collide(Box* other);
    bool collide2(Box *other);
    void scale(GLfloat, GLfloat, GLfloat);
    GLfloat x_width, y_width, z_width;
    GLfloat box_x_width, box_y_width, box_z_width;
};

#endif /* Box_hpp */
