//
//  Point.hpp
//  HW2
//
//  Created by Junxi Li on 5/13/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#ifndef Point_hpp
#define Point_hpp

#include <stdio.h>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "Window.h"

class Point{
public:
    GLuint VAO, VBO;
    glm::vec3 coor;
    GLint ID;
    glm::mat4 toWorld;
    glm::vec3 color;
    std::vector<glm::vec3> v;

    
    
    Point(glm::vec3 a, GLint);
    void draw(GLuint);
    void update(double, double, glm::vec3, glm::vec3, glm::vec3);
};

#endif /* Point_hpp */
