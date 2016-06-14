//
//  Particle.hpp
//  HW2
//
//  Created by Junxi Li on 5/27/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#ifndef Particle_hpp
#define Particle_hpp

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

class Particle{
public:
    Particle();
    ~Particle();
    void draw(GLuint shaderProgram);
    void init();
    void redraw(int);
    
    std::vector<glm::vec3> v;
    GLuint VAO, VBO;
    
};

#endif /* Particle_hpp */
