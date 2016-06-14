#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <glm/gtx/transform.hpp>
#include <algorithm>
#include "Window.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


class OBJObject
{
public:
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;
    float angle;
    

    GLuint VBO, VAO, EBO, NBO;
    
    OBJObject();
    ~OBJObject();
    OBJObject(const char* filepath, bool original_parser);
    void parse(const char* filepath);
    void parse2(const char* filepath);
    void draw(GLuint shaderProgram);
    void move(int);
    void spin(float);
    void update();
    glm::mat4 toWorld;
    glm::mat4 get_toWorld();
    void set_toWorld(glm::mat4);
    std::vector<glm::vec3> get_colors();
    glm::vec3 normal_color(glm::vec3);
    std::vector<glm::vec3> get_vertices();
    void reset_angle();
    void set_pos(glm::vec3);
    float maxX, minX, maxY, minY, maxZ, minZ;
    bool bunny;
    bool is_tree;
};

#endif