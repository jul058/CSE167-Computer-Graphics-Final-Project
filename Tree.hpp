//
//  Tree.hpp
//  HW2
//
//  Created by Junxi Li on 5/26/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#ifndef Tree_hpp
#define Tree_hpp

#include <string>
#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
//#include "Cylinder.hpp"
#include "OBJObject.h"
#include "MatrixTransform.hpp"
#include "Geode.hpp"
#include "Node.hpp"
#include "Box.hpp"
#include <cmath>
#include <stack>


using namespace std;
class Tree
{
public:
    Tree(int iteration, OBJObject* obj, float, std::string, std::string, glm::mat4);
    ~Tree();
    string create(GLuint i, string curr, int start);
    void draw(GLuint);
    int generate(MatrixTransform *curr, int start, int);
    void generate2(MatrixTransform *curr);
    void deleteAll(Node* curr);
    
    Box *box;
    MatrixTransform *root;
    int iteration;
    string result;
    OBJObject* obj;
    //vector<Cylinder*> v;
    //Cylinder* cylinder;
    string s = "X";
    string rule1;
    string rule2;
    //string rule = "FF-[-F+F+F]+[+F-F-F]";
    //std::stack<glm::mat4> store;
    std::stack<MatrixTransform*> store;
    glm::mat4 plus_sign;
    glm::mat4 minus_sign;
    
    glm::mat4 curr_mat;
    glm::vec3 curr_pos;
    glm::vec3 prev_pos;
    glm::vec3 prev;
    glm::mat4 toWorld;
    GLuint shaderprogram;
};

#endif /* Tree_hpp */
