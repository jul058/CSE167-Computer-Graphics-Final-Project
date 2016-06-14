//
//  Node.hpp
//  cse167_hw2
//
//  Created by Simon on 5/3/16.
//  Copyright © 2016 Simon. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
//#include "OBJObject.h"


class Node
{
public:
    virtual void draw(GLuint skyboxShader) = 0;
    virtual void update(glm::mat4 C) = 0;
    
    //glm::mat4 M;
    //glm::mat4 end;
};
#endif /* Node_hpp */
