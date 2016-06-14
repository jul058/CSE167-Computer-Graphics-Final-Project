//
//  Controller.cpp
//  HW2
//
//  Created by Junxi Li on 5/30/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#include "Controller.hpp"
#define N 1024
#define UP 0
#define LEFT 1
#define RIGHT 2
#define DOWN 3


Controller::Controller(OBJObject* obj_in, Terrain* ter, int x_in, int z_in, GLfloat SIZE, GLfloat XD, GLfloat YD, GLfloat ZD, GLfloat y_offset, glm::vec3 board_translation, glm::vec3 board_scale){
    this->first = true;
    this->board_scale = board_scale;
    this->board_translation = board_translation;
    this->y_offset = y_offset;
    this->SIZE = SIZE;
    this->ter = ter;
    this->obj = obj_in;
    this->VAO = obj->VAO;
    this->EBO = obj->EBO;
    this->NBO = obj->NBO;
    this->VBO = obj->VBO;
    
    this->obj->toWorld = glm::scale(glm::mat4(1.0f), this->board_scale)*this->obj->toWorld;
    this->obj->toWorld = glm::translate(glm::mat4(1.0f), this->board_translation)*this->obj->toWorld;
    this->obj->toWorld = obj->toWorld*glm::scale(glm::mat4(1.0f), glm::vec3(SIZE,SIZE,SIZE));
    this->obj->toWorld[3][1] = ter->arr[x_in][z_in]-N/3+y_offset;
    this->obj->toWorld = glm::rotate(this->obj->toWorld, -90.0f/180.0f*glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));

    last_point = curr_point = glm::vec3(0.0f);
    
    box = new Box(this->obj, XD, YD, ZD);
    x = 0;    z = 0;
    prev_height = curr_height = ter->arr[x+N/2][z+N/2]-N/3;
    last_point = curr_point = glm::vec3(x+N/2, curr_height, z+N/2);
    speed = 1.0f;
    orientation = 0.0f;
    box->scale(SIZE, SIZE, SIZE);
    box->init();
    
    
    gravity = -0.00002f;
    

    
}

void Controller::move(int mode){
    if(first){
        this->box->move(glm::vec3(0.0f, -10.0f, 0.0f));
        first = false;
    }
    if(mode == UP){
        /*z += glm::cos(orientation)*speed;
         x += glm::sin(orientation)*speed;
         prev_height = curr_height;
         curr_height = ter->arr[x+N/2][z+N/2]-N/3;
         speed += (-curr_height+prev_height)/1000.0f;
         if(speed <= 1.0f){
         speed += 1.0f;
         }*/
        if(z-speed > -500){
            z -= speed;
            
            
        }
        
    }
    else if(mode == DOWN){
        
        if(z+speed < 500){
            z += speed;
            
//            prev_height = curr_height;
//            curr_height = ter->arr[x+N/2][z+N/2]-N/3;
//            speed += (-curr_height+prev_height)/100.0f;
//            speed -= 0.5f;
//            if(speed <= 1.0f){
//                speed += 1.0f;
//            }
        }
    }
    else if(mode == LEFT){

        if(x-speed > -500){
            x -= speed;
            
//            prev_height = curr_height;
//            curr_height = ter->arr[x+N/2][z+N/2]-N/3;
//            speed += (-curr_height+prev_height)/1000.0f;
//            speed -= 0.5f;
//            if(speed <= 1.0f){
//                speed += 1.0f;
//            }
        }
    }
    else if(mode == RIGHT){
        if(x+speed < 500){
            x += speed;
            
//            prev_height = curr_height;
//            curr_height = ter->arr[x+N/2][z+N/2]-N/3;
//            speed += (-curr_height+prev_height)/1000.0f;
//            speed -= 0.5f;
//            if(speed <= 1.0f){
//                speed += 1.0f;
//            }
        }
    }
    
    prev_height = curr_height;
    curr_height = ter->arr[x+N/2][z+N/2]-N/3;
    speed += (-curr_height+prev_height)/20.0f;
    speed -= 0.5f;
    if(speed <= 1.0f){
        speed += 1.0f;
    }
    
    
    
    y = ter->arr[x+N/2][z+N/2]-N/3;
    
    curr_point = glm::vec3(x+N/2, y+y_offset, z+N/2);
    direction();
    box->move(curr_point-last_point);
    last_point = curr_point;
    
    
    this->obj->toWorld[3][0] = x;
    this->obj->toWorld[3][1] = y+y_offset;
    this->obj->toWorld[3][2] = z;
    this->box->obj = this->obj;
}

void Controller::draw(GLuint shaderProgram, glm::vec3 camera_pos){
    obj->draw(shaderProgram);
}

void Controller::direction(){
    if(curr_point != last_point){
        glm::vec3 zaxis = curr_point-last_point;
        zaxis = glm::normalize(zaxis);
        glm::vec3 xaxis = glm::normalize(glm::cross(glm::vec3(0.0f,1.0f,0.0f), zaxis));
        glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));
        glm::mat4 C = glm::mat4(glm::vec4(xaxis,0.0f),glm::vec4(yaxis,0.0f),glm::vec4(zaxis,0.0f),glm::vec4(curr_point,1.0f));
    
        this->obj->toWorld = C;
        
        this->obj->toWorld = glm::rotate(this->obj->toWorld, -90.0f/180.0f*glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
        this->obj->toWorld = glm::scale(glm::mat4(1.0f), this->board_scale)*this->obj->toWorld;
        this->obj->toWorld = glm::translate(glm::mat4(1.0f), this->board_translation)*this->obj->toWorld;
        this->obj->toWorld = obj->toWorld*glm::scale(glm::mat4(1.0f), glm::vec3(SIZE,SIZE,SIZE));
        
        //box->obj->toWorld[3][1] += y_offset;
//        box->init();
        
        //this->obj->toWorld[3][1] = ter->arr[x_in][z_in]-N/3+y_offset;
        
        

        
        //last_point = curr_point;
        //printf("%f,%f,%f\n", obj->toWorld[3][0], obj->toWorld[3][1], obj->toWorld[3][2]);

    }
}

