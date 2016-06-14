//
//  Tree.cpp
//  HW2
//
//  Created by Junxi Li on 5/26/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#include "Tree.hpp"
#include <iostream>
#define N 8
#define T_H 15.0f
#define T_D 1.5f
//#define THETA 25.7f
int i;
using namespace std;
//glm::mat4 plus_sign = glm::rotate(glm::mat4(1.0f), 30.0f/180.0f*glm::pi<float>(), glm::vec3(0.0f,1.0f,1.0f))
//*glm::rotate(glm::mat4(1.0f), 20.0f/180.0f*glm::pi<float>(), glm::vec3(1.0f, 1.0f, 0.0f));
//glm::mat4 minus_sign = glm::rotate(glm::mat4(1.0f), -30.0f/180.0f*glm::pi<float>(), glm::vec3(0.0f,1.0f,1.0f))
//*glm::rotate(glm::mat4(1.0f), -20.0f/180.0f*glm::pi<float>(), glm::vec3(1.0f, 1.0f, 0.0f));
glm::mat4 equal_sign = glm::rotate(glm::mat4(1.0f), 30.0f/180.0f*glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f))
*glm::rotate(glm::mat4(1.0f), 20.0f/180.0f, glm::vec3(1.0f, 0.0f, 0.0f));




Tree::Tree(int iteration, OBJObject* obj, float THETA, std::string rule1, std::string rule2, glm::mat4 offset_matrix){
    this->rule1 = rule1;
    this->rule2 = rule2;
    this->iteration = iteration;
    create(iteration, s, 0);
    this->obj = obj;
    //box = new Box();
    root = new MatrixTransform();
    i = 0;
    printf("result string: %s\n", result.c_str());
    
    plus_sign = glm::rotate(glm::mat4(1.0f), -THETA/180.0f*glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)) *
    glm::rotate(glm::mat4(1.0f), THETA/180.0f*glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) *
    glm::rotate(glm::mat4(1.0f), -THETA/180.0f*glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
    minus_sign = glm::rotate(glm::mat4(1.0f), THETA/180.0f*glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)) *
    glm::rotate(glm::mat4(1.0f), THETA/180.0f*glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) *
    glm::rotate(glm::mat4(1.0f), THETA/180.0f*glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
    
    generate2(root);
    
    //root->update(glm::mat4(1.0f));
    root->update(offset_matrix);
}

void Tree::generate2(MatrixTransform* curr){
//    MatrixTransform *tmp1;
//    MatrixTransform *tmp2;
    
    for(int i = 0; i < result.size(); i++){
        if(result[i] == 'F' || result[i] == 'X'){
            MatrixTransform *tmp1 = new MatrixTransform();
            MatrixTransform *tmp2 = new MatrixTransform();
            Geode *child = new Geode(obj);
            curr->addChild(tmp1);
            tmp1->addChild(tmp2);
            tmp2->addChild(child);
            
            tmp2->M = glm::scale(glm::mat4(1.0f), glm::vec3(T_D, T_H, T_D)) * tmp2->M;
//            tmp1->M = glm::scale(glm::mat4(1.0f), glm::vec3(0.9f, 0.9f, 0.9f)) * tmp1->M;
            tmp1->M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 0.0f))*tmp1->M;
            
            curr = tmp1;
        }
        else if(result[i] == '['){
//            tmp = new MatrixTransform();
//            tmp->M = glm::scale(glm::mat4(1.0f), glm::vec3(0.9f, 0.9f, 0.9f));
//            curr->addChild(tmp);
            store.push(curr);
//            curr = tmp;
        }
        else if(result[i] == ']'){
            curr = store.top();
            store.pop();
        }
        else if(result[i] == '+'){
            MatrixTransform *tmp = new MatrixTransform();
            curr->addChild(tmp);
            tmp->M = plus_sign * tmp->M;
            curr = tmp;
        }
        else if(result[i] == '-'){
            MatrixTransform *tmp = new MatrixTransform();
            curr->addChild(tmp);
            tmp->M = minus_sign * tmp->M;
            curr = tmp;
        }
        else if(result[i] == '='){
            curr->M = equal_sign * curr->M;
        }
    }
}
int Tree::generate(MatrixTransform *curr, int start, int level){
    //Node *curr = root;
    printf("before: %d\n", start);
    int index_jump;
    i=start;
    while(i < result.size()){
        printf("level is: %d\n", level);
        if(result[i] == '['){
            MatrixTransform *tmp = new MatrixTransform();
            //tmp->M = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
            curr->addChild(tmp);
            generate(tmp, i+1, level+1);
            i++;
            //i += index_jump;
        }
        else if(result[i] == ']'){
            i++;
            return i-start+1;
        }
        else if(result[i] == 'F'){
            MatrixTransform *tmp = new MatrixTransform();
            tmp->M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f))*tmp->M;
            curr->addChild(tmp);
            Geode *child = new Geode(obj);
            tmp->addChild(child);
            index_jump = generate(tmp, i+1, level+1);
            i++;
            //i += index_jump;
        }
        else if(result[i] == '+'){
            i++;
            curr->M = glm::rotate(glm::mat4(1.0f), 20.0f/180.0f*glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f))*curr->M;
        }
        else if(result[i] == '-'){
            i++;
            curr->M = glm::rotate(glm::mat4(1.0f), -20.0f/180.0f*glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f))*curr->M;
        }
    }
    printf("after: %d\n", i);
    return 0;
}

string Tree::create(GLuint i, string curr, int start) {
    if (i == 0) {
        result = curr;
        return curr;
    }
    for (int j = start; j < curr.size(); j++) {
        if (curr[j] == 'F') {
            curr.replace(j, 1, rule2);
            j += rule2.size();
        }
        if(curr[j] == 'X')
        {
            curr.replace(j, 1, rule1);
            j += rule1.size();
        }
    }
    return create(i - 1, curr, 0);
}

void Tree::draw(GLuint shaderProgram) {
    root->draw(shaderProgram);
}


void Tree::deleteAll(Node *curr){
    MatrixTransform* cmp = dynamic_cast<MatrixTransform*>(curr);
    if(cmp != 0){
        for(int i = 0; i < ((MatrixTransform*)curr)->children.size(); i++){
            deleteAll( ((MatrixTransform*)curr) -> children[i] );
        }
    }
    else{
        delete curr;
    }
}

Tree::~Tree(){
    deleteAll(root);
}

