//
//  Handle.hpp
//  HW2
//
//  Created by Junxi Li on 5/16/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#ifndef Handle_hpp
#define Handle_hpp

#include <stdio.h>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "Window.h"

class Handle{
public:
    glm::vec3 p2, p1;
    GLuint VAO, VBO;
    std::vector<glm::vec3> v;
    glm::mat4 toWorld;
    
    Handle(glm::vec3, glm::vec3);
    void draw(GLuint);
    void update(glm::vec3 p2, glm::vec3 p1);
};

#endif /* Handle_hpp */
