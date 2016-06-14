//
//  Environment.cpp
//  HW2
//
//  Created by Junxi Li on 5/16/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#include "Environment.hpp"

int combination[4] = {1, 3, 3, 1};

Environment::Environment(OBJObject *obj_in){
    obj = obj_in;
    this->indices = obj->indices;
    this->vertices = obj->vertices;
    this->normals = obj->normals;
    this->VAO = obj->VAO;
    this->EBO = obj->EBO;
    this->NBO = obj->NBO;
    this->VBO = obj->VBO;
    this->toWorld = obj->toWorld*glm::scale(glm::mat4(1.0f), glm::vec3(0.5f,0.5f,0.5f));
    gravity = -0.00002f;
    
    glGenVertexArrays(1, &VAO);
    
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive vertex attributes. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    
    
    
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    ////////
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    //    glBindVertexArray(1);
    
}

void Environment::draw(GLuint shaderProgram, glm::vec3 camera_pos){
    GLuint MatrixID1 = glGetUniformLocation(shaderProgram, "model");
    GLuint MatrixID2 = glGetUniformLocation(shaderProgram, "view");
    GLuint MatrixID3 = glGetUniformLocation(shaderProgram, "projection");
    GLuint cam_pos_id = glGetUniformLocation(shaderProgram, "cameraPos");
    glUniformMatrix4fv(MatrixID1, 1, GL_FALSE, &toWorld[0][0]);
    glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &Window::V[0][0]);
    glUniformMatrix4fv(MatrixID3, 1, GL_FALSE, &Window::P[0][0]);
    glUniform3f(cam_pos_id, camera_pos.x, camera_pos.y, camera_pos.z);
    glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

GLfloat Environment::cal_C(GLfloat t, int i){
    return combination[i]*glm::pow(t, i)*glm::pow((1.0f-t),(3-i));
}

glm::vec3 Environment::B(GLfloat t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3){
    return cal_C(t, 0)*p0+cal_C(t, 1)*p1+cal_C(t, 2)*p2+cal_C(t, 3)*p3;
}

void Environment::update(glm::vec3 curr_pos){
    this->toWorld[3][0] = curr_pos.x;
    this->toWorld[3][1] = curr_pos.y;
    this->toWorld[3][2] = curr_pos.z;
}

void Environment::orientation(glm::vec3 curr_point, glm::vec3 last_point){
    glm::vec3 zaxis = curr_point-last_point;
    zaxis = glm::normalize(zaxis);
    glm::vec3 xaxis = glm::normalize(glm::cross(glm::vec3(0.0f,1.0f,0.0f), zaxis));
    glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));
    glm::mat4 C = glm::mat4(glm::vec4(xaxis,0.0f),glm::vec4(yaxis,0.0f),glm::vec4(zaxis,0.0f),glm::vec4(last_point,1.0f));
    this->toWorld = C;
}

