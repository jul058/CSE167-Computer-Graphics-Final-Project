//
//  Particle.cpp
//  HW2
//
//  Created by Junxi Li on 5/27/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//
#include "Particle.hpp"
#define MAX_PARTICLES 6000
#define WIDTH 1024

struct small_part{
    // Position/direction
    GLfloat xpos;
    GLfloat ypos;
    GLfloat zpos;
    // Velocity/Direction, only goes down in y dir
    GLfloat vel;
    // Gravity
    GLfloat gravity;
}particles[MAX_PARTICLES];

Particle::Particle(){
    init();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v[0])*v.size(), &v[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Particle::~Particle(){
    
}

void Particle::init(){
    for(int i = 0; i < MAX_PARTICLES; i++){
        particles[i].xpos = ((GLfloat)(rand()%(WIDTH/2))*2-(WIDTH/2));
        particles[i].zpos = ((GLfloat)(rand()%(WIDTH/2))*2-(WIDTH/2));
        particles[i].ypos = (GLfloat)(WIDTH/2);
        particles[i].vel = (GLfloat)(rand()%3)+((GLfloat)(rand()%100)/100.0);
        particles[i].gravity = -0.001f;
        v.push_back(glm::vec3(particles[i].xpos, particles[i].ypos, particles[i].zpos));
    }
}
void Particle::redraw(int i){
    particles[i].xpos = ((GLfloat)(rand()%(WIDTH/2))*2-(WIDTH/2));
    particles[i].zpos = ((GLfloat)(rand()%(WIDTH/2))*2-(WIDTH/2));
    particles[i].ypos = (GLfloat)(WIDTH/2);
    particles[i].vel = (GLfloat)(rand()%3)+((GLfloat)(rand()%100)/100.0);
    particles[i].gravity = -0.001f;
    v[i] = glm::vec3(particles[i].xpos, particles[i].ypos, particles[i].zpos);
}

void Particle::draw(GLuint shaderProgram){
    GLfloat x, y, z;
    for(int i = 0; i < MAX_PARTICLES; i++){
            x = particles[i].xpos;
            y = particles[i].ypos;
            z = particles[i].zpos;
            y += particles[i].vel - (float)((rand()%3)/300.0);
            x += (float)((rand()%3)/3.0*2.0-1.0);
            z += (float)((rand()%3)/3.0*2.0-1.0);
            
            if(x <= -(GLfloat)(WIDTH/2)){
                x += (GLfloat)WIDTH;
            }
            else if(x >= (GLfloat)(WIDTH/2)){
                x -= (GLfloat)WIDTH;
            }
            if(z <= -(GLfloat)(WIDTH/2)){
                z += (GLfloat)WIDTH;
            }
            else if(z >= (GLfloat)(WIDTH/2)){
                z -= (GLfloat)WIDTH;
            }
            /*if(y >= (GLfloat)(WIDTH/2)){
                y -= 128.0f;
            }*/
            
            particles[i].ypos = y;
            particles[i].xpos = x;
            particles[i].zpos = z;
            
            v[i] = glm::vec3(particles[i].xpos, particles[i].ypos, particles[i].zpos);
            if(particles[i].ypos>400.0f){
                particles[i].vel += particles[i].gravity;
            }
            if(particles[i].ypos < -128.0f){
                redraw(i);
            }
            /*else if(particles[i].ypos >= (GLfloat)(WIDTH/2)+50.0){
                redraw(i);
            }*/
                
    }

    GLuint MatrixID1 = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(MatrixID1, 1, GL_FALSE, &Window::V[0][0]);
    GLuint MatrixID2 = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &Window::P[0][0]);
    GLuint colorID = glGetUniformLocation(shaderProgram, "in_color");
    glUniform3f(colorID, 1.0,1.0,1.0);
    
    
    
    glPointSize(3.0f);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(v[0]), &v[0], GL_STATIC_DRAW);
    glDrawArrays(GL_POINTS, 0, (int)v.size());
    glBindVertexArray(0);
}
