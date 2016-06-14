//
//  skyBox.hpp
//  HW2
//
//  Created by Junxi Li on 5/2/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#ifndef skyBox_hpp
#define skyBox_hpp

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "Window.h"
class skyBox{
private:
    

public:
    skyBox();
    ~skyBox();
    
    glm::mat4 toWorld;
    GLuint cubemapTexture;
    GLuint textureID;
    GLuint skyboxVAO, skyboxVBO;
    
    void pre_process();
    std::vector<unsigned char*> convert_img_ppm();
    GLuint loadCubemap();
    unsigned char* loadPPM(const char* filename, int& width, int& height);
    void draw(GLuint);
};


#endif /* skyBox_hpp */
