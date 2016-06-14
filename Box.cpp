//
//  Box.cpp
//  HW2
//
//  Created by Junxi Li on 5/27/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#include "Box.hpp"

Box::Box(OBJObject* obj, GLfloat XD, GLfloat YD, GLfloat ZD){
    this->obj = obj;
    this->collideID = 0;
    this->XD = XD, this->YD = YD, this->ZD = ZD;
}

void Box::init(){
    x_width = obj->maxX - obj->minX;
    y_width = obj->maxY - obj->minY;
    z_width = obj->maxZ - obj->minZ;
    x_width *= x_scale, y_width *= y_scale, z_width *= z_scale;
    GLfloat x, y, z;
    glm::vec3 tmp;
    x = obj->toWorld[3][0], y = obj->toWorld[3][1], z = obj->toWorld[3][2];
    //x *= x_scale, y *= y_scale, z *= z_scale;
    
//    v.clear();
//    w.clear();
    
    //0
    tmp = glm::vec3(x-x_width/XD, y+y_width/YD, z+z_width/ZD);
    v.push_back(tmp);
    
    //1
    tmp.x += x_width/XD*2;
    v.push_back(tmp);
    
    //2
    tmp.y -= y_width/YD*2;
    v.push_back(tmp);
    
    //3
    tmp.x -= x_width/XD*2;
    v.push_back(tmp);
    
    //4
    tmp.z -= z_width/ZD*2;
    tmp.y += y_width/YD*2;
    v.push_back(tmp);
    
    //5
    tmp.x += x_width/XD*2;
    v.push_back(tmp);
    
    //6
    tmp.y -= y_width/YD*2;
    v.push_back(tmp);
    
    //7
    tmp.x -= x_width/XD*2;
    v.push_back(tmp);
    
    //front face
    w.push_back(v[0]);
    w.push_back(v[1]);
    w.push_back(v[1]);
    w.push_back(v[2]);
    w.push_back(v[2]);
    w.push_back(v[3]);
    w.push_back(v[3]);
    w.push_back(v[0]);
    
    //back facec
    w.push_back(v[4]);
    w.push_back(v[5]);
    w.push_back(v[5]);
    w.push_back(v[6]);
    w.push_back(v[6]);
    w.push_back(v[7]);
    w.push_back(v[7]);
    w.push_back(v[4]);
    
    
    //top face
    w.push_back(v[0]);
    w.push_back(v[4]);
    w.push_back(v[1]);
    w.push_back(v[5]);
    
    //buttom face
    w.push_back(v[3]);
    w.push_back(v[7]);
    w.push_back(v[2]);
    w.push_back(v[6]);
    
    
    //calculate widths
    box_x_width = v[1].x-v[0].x;
    box_y_width = v[1].y-v[2].y;
    box_z_width = v[1].z-v[5].z;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, w.size()*sizeof(w[0]), &w[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Box::draw(GLuint shaderProgram){
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &obj->toWorld[0][0]);
    GLuint MatrixID1 = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(MatrixID1, 1, GL_FALSE, &Window::V[0][0]);
    GLuint MatrixID2 = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &Window::P[0][0]);
    glUniform1i(glGetUniformLocation(shaderProgram, "ID"), collideID);
    
    glLineWidth(5.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, (int)w.size());
    
    glBindVertexArray(0);
}

bool Box::collide(Box *other){
    if(v[7].x+box_x_width >= other->v[7].x
       && v[7].x <= other->v[7].x+other->box_x_width
       && v[7].y+box_y_width >= other->v[7].y
       && v[7].y <= other->v[7].y+other->box_y_width
       && v[7].z+box_z_width >= other->v[7].z
       && v[7].z <= other->v[7].z+other->box_z_width){
        this->collideID = 1;
        other->collideID = 1;
        return true;
    }
    this->collideID = 0;
    other->collideID = 0;
    return false;
}


void Box::move(glm::vec3 offset){
    for(int i = 0; i < v.size(); i++){
        v[i] += offset;
    }
    for(int i = 0; i < w.size(); i++){
        w[i] += offset;
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, w.size()*sizeof(w[0]), &w[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Box::scale(GLfloat a, GLfloat b, GLfloat c){
    x_scale=a;
    y_scale=b;
    z_scale=c;
}



