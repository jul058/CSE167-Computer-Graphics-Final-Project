//
//  Geode.hpp
//  cse167_hw2
//
//  Created by Simon on 5/3/16.
//  Copyright © 2016 Simon. All rights reserved.
//

#ifndef Geode_hpp
#define Geode_hpp
#include "Node.hpp"
#include "OBJObject.h"

//class Node;
class OBJObject;
class Geode: public Node
{
public:
    glm::mat4 M;
    Geode(OBJObject * obj);
    ~Geode();
    void update(glm::mat4 C);
    void draw(GLuint);
    OBJObject * toDraw;
    
};
#endif /* Geode_hpp */
