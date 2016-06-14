//
//  Handle.cpp
//  HW2
//
//  Created by Junxi Li on 5/16/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#include "Handle.hpp"


Handle::Handle(glm::vec3 a, glm::vec3 b){
    this->p2 = a;
    this->p1 = b;
    v.push_back(a);
    v.push_back(b);
    toWorld = glm::mat4(1.0f);
    toWorld = glm::mat4(1.0f);
    
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(v[0]), &v[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Handle::draw(GLuint shaderProgram){
    
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &toWorld[0][0]);
    GLuint MatrixID1 = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(MatrixID1, 1, GL_FALSE, &Window::V[0][0]);
    GLuint MatrixID2 = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &Window::P[0][0]);
    
    
    glBindVertexArray(VAO);
    glPointSize(20.0f);
    glDrawArrays(GL_LINES, 0, (int)v.size());
    
    glBindVertexArray(0);
}

void Handle::update(glm::vec3 p2, glm::vec3 p1){
    this->p2 = p2;
    this->p1 = p1;
    v.clear();
    v.push_back(p2);
    v.push_back(p1);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(v[0]), &v[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


