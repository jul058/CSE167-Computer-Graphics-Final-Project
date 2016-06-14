//
//  Environment.hpp
//  HW2
//
//  Created by Junxi Li on 5/16/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#ifndef Environment_hpp
#define Environment_hpp

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "Window.h"
#include "OBJObject.h"
class Environment{
public:
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;
  
    glm::mat4 toWorld;
    GLfloat gravity;
    GLfloat velocity;
    GLuint VAO, VBO, NBO, EBO;
    //std::vector<> v;
    Environment(OBJObject* obj);
    void draw(GLuint shaderProgram, glm::vec3 camera_pos);
    void update(glm::vec3 curr_pos);
    GLfloat cal_C(GLfloat t, int i);
    glm::vec3 B(GLfloat t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
    void orientation(glm::vec3 curr_point, glm::vec3 last_point);
    
    OBJObject* obj;
};


#endif /* Environment_hpp */
