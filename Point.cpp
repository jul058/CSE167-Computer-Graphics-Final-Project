//
//  Point.cpp
//  HW2
//
//  Created by Junxi Li on 5/13/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#include "Point.hpp"
#include <iostream>

Point::Point(glm::vec3 a, GLint ID){
    this->ID = ID;
    //toWorld = glm::mat4(1.0f);
    coor = a;
    this->v.push_back(a);
    
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

void Point::draw(GLuint shaderProgram){
    
    //std::cout << "ID od this point is "<< this->ID << std::endl;
    
    /*GLuint MatrixID = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &toWorld[0][0]);*/
    GLuint MatrixID1 = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(MatrixID1, 1, GL_FALSE, &Window::V[0][0]);
    GLuint MatrixID2 = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &Window::P[0][0]);
    GLuint point_ID = glGetUniformLocation(shaderProgram, "ID");
    glUniform1i(point_ID, this->ID);
    
    glBindVertexArray(VAO);
    glPointSize(20.0f);
    glDrawArrays(GL_POINTS, 0, (int)v.size());
    
    glBindVertexArray(0);
}

void Point::update(double diffx, double diffy, glm::vec3 xaxis, glm::vec3 yaxis, glm::vec3 zaxis){
    v[0] += xaxis * glm::vec3(diffx) + yaxis * glm::vec3(diffy);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(v[0]), &v[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



