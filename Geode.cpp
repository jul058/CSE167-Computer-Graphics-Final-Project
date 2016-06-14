//
//  Geode.cpp
//  cse167_hw2
//
//  Created by Simon on 5/3/16.
//  Copyright © 2016 Simon. All rights reserved.
//

#include "Geode.hpp"

Geode::Geode(OBJObject * obj)
{
    M = glm::mat4(1.0f);
    toDraw = obj;
    
}

Geode::~Geode(){
    delete toDraw;
}

void Geode::update(glm::mat4 C)
{
    M = C;
    //M = C*M;
}


void Geode::draw(GLuint skyboxShader)
{
    toDraw->toWorld = M;
    toDraw->draw(skyboxShader);
}

