#include "OBJObject.h"
#include "light.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <limits>
#define P_INF std::numeric_limits<double>::max()
#define N_INF std::numeric_limits<double>::min()
//int lightMode = Window::light_mode;

OBJObject::~OBJObject(){
}

OBJObject::OBJObject(const char *filepath, bool original_parser)
{
    is_tree = false;
    toWorld = glm::mat4(1.0f);
    if(original_parser)
        parse(filepath);
    else
        parse2(filepath);
    printf("finish\n");
    // Create buffers/arrays
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
OBJObject::OBJObject(){}

glm::mat4 model = glm::mat4(1.0f);

void OBJObject::parse(const char *filepath)
{
    std::ifstream in(filepath);
    std::string s;
    char buf1,buf2,store;
    float v1,v2,v3,culX=0.0,culY=0.0,culZ=0.0;
    maxX=N_INF,maxY=N_INF,maxZ=N_INF,minX=P_INF,minY=P_INF,minZ=P_INF;
    float avgX,avgY,avgZ;
    unsigned int f1,f2;
    while(1){
        buf1 = in.get();
        if(in.eof())    break;
        if(buf1 == 'v' || buf1 == 'f'){
            buf2 = in.get();
            if(buf1 == 'v' && buf2 == ' '){
                in>>v1>>v2>>v3;
                culX+=v1;
                culY+=v2;
                culZ+=v3;
                maxX = std::max(maxX, v1);
                minX = std::min(minX, v1);
                maxY = std::max(maxY, v2);
                minY = std::min(minY, v2);
                maxZ = std::max(maxZ, v3);
                minZ = std::min(minZ, v3);
                
                vertices.push_back(glm::vec3(v1,v2,v3));
            }
            else if(buf1 == 'v' && buf2 == 'n'){
                in>>v1>>v2>>v3;
                normals.push_back(glm::vec3(v1,v2,v3));
            }
            else if(buf1 == 'f' && buf2 == ' '){
                for(int j = 0; j < 3; j++){
                    in >> f1 >> store >> store >> f2;
                    indices.push_back(f1-1);
                }
            }
        }
        //printf("")
        getline(in, s);
    }
    avgX = culX/vertices.size();
    avgY = culY/vertices.size();
    avgZ = culZ/vertices.size();
    for(int i = 0; i < vertices.size(); i++){
        vertices[i] = glm::vec3((vertices[i].x-avgX)/(maxX-minX),
                                (vertices[i].y-avgY)/(maxY-minY),(vertices[i].z-avgZ)/(maxZ-minZ));
    }
    // Populate the face indices, vertices, and normals vectors with the OBJ Object data
}

void OBJObject::parse2(const char *filepath)
{
    std::ifstream in(filepath);
    std::string s;
    char buf1,buf2,store;
    float v1,v2,v3,culX=0.0,culY=0.0,culZ=0.0;
    maxX=N_INF,maxY=N_INF,maxZ=N_INF,minX=P_INF,minY=P_INF,minZ=P_INF;
    float avgX,avgY,avgZ;
    unsigned int f1,f2,f3,f4,f5,f6,f7,f8,f9;
    while(1){
        buf1 = in.get();
        if(in.eof())    break;
        if(buf1 == 'v' || buf1 == 'f'){
            buf2 = in.get();
            if(buf1 == 'v' && buf2 == ' '){
                in>>v1>>v2>>v3;
                culX+=v1;
                culY+=v2;
                culZ+=v3;
                maxX = std::max(maxX, v1);
                minX = std::min(minX, v1);
                maxY = std::max(maxY, v2);
                minY = std::min(minY, v2);
                maxZ = std::max(maxZ, v3);
                minZ = std::min(minZ, v3);
                
                vertices.push_back(glm::vec3(v1,v2,v3));
            }
            else if(buf1 == 'v' && buf2 == 'n'){
                in>>v1>>v2>>v3;
                normals.push_back(glm::vec3(v1,v2,v3));
            }
            else if(buf1 == 'f' && buf2 == ' '){
//                for(int j = 0; j < 3; j++){
//                    in >> f1 >> store  >> f2 >> store >> f3;
//                    indices.push_back(f1);
//                    indices.push_back(f2);
//                    indices.push_back(f3);
//                }
//                in >>f1>>store>>f2>>store>>f3>>f4>>store>>f5>>store>>f6>>f7>>store>>f8>>store>>f9;
//                indices.push_back(f1-1);
//                indices.push_back(f4-1);
//                indices.push_back(f7-1);
//                indices.push_back(f2-1);
//                indices.push_back(f5-1);
//                indices.push_back(f8-1);
//                indices.push_back(f3-1);
//                indices.push_back(f6-1);
//                indices.push_back(f9-1);
                for(int j = 0; j < 3; j++){
                    in >> f1>>store>>f2>>store>>f3;
                    indices.push_back(f1-1);
                    //indices.push_back(f3-1);
                }

            }
        }
        //printf("")
        getline(in, s);
    }
    avgX = culX/vertices.size();
    avgY = culY/vertices.size();
    avgZ = culZ/vertices.size();
    for(int i = 0; i < vertices.size(); i++){
        vertices[i] = glm::vec3((vertices[i].x-avgX)/(maxX-minX),
                                (vertices[i].y-avgY)/(maxY-minY),(vertices[i].z-avgZ)/(maxZ-minZ));
    }
    // Populate the face indices, vertices, and normals vectors with the OBJ Object data
}

void OBJObject::draw(GLuint shaderProgram)
{
    /*glMatrixMode(GL_MODELVIEW);
    
    // Push a save state onto the matrix stack, and multiply in the toWorld matrix
    glPushMatrix();
    glMultMatrixf(&(toWorld[0][0]));
    */
    /*glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i < indices.size(); i++)
    {
        glColor3f(normals[indices[i]].x, normals[indices[i]].y, normals[indices[i]].z);
        glNormal3f(normals[indices[i]].x, normals[indices[i]].y, normals[indices[i]].z);
        glVertex3f(vertices[indices[i]].x, vertices[indices[i]].y, vertices[indices[i]].z);
    }
    glEnd();
    */
    // Calculate combination of the model (toWorld), view (camera inverse), and perspective matrices
    // We need to calculate this because as of GLSL version 1.40 (OpenGL 3.1, released March 2009), gl_ModelViewProjectionMatrix has been
    // removed from the language. The user is expected to supply this matrix to the shader when using modern OpenGL.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    GLuint MatrixID1 = glGetUniformLocation(shaderProgram, "model");
    GLuint MatrixID2 = glGetUniformLocation(shaderProgram, "view");
    GLuint MatrixID3 = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(MatrixID1, 1, GL_FALSE, &toWorld[0][0]);
    glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &Window::V[0][0]);
    glUniformMatrix4fv(MatrixID3, 1, GL_FALSE, &Window::P[0][0]);
    
    
    
    glUniform1i(glGetUniformLocation(shaderProgram, "light_mode"), Window::light_mode);
    glUniform1i(glGetUniformLocation(shaderProgram, "prev_light_mode"), Window::prev_light_mode);
    //glUniform3f(glGetUniformLocation(shade))
    
    glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), 0.0f, 0.0f, 20.0f);
    //directional light
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), Window::dir_light_dir.x, Window::dir_light_dir.y, Window::dir_light_dir.z);
    //glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), 1.0f, 1.0f, 1.0f);
    
    if(!(this->is_tree)){
        glUniform1i(glGetUniformLocation(shaderProgram, "is_tree"), 0);
        glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), .3f, .3f, .25f);
        glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), .4f, .4f, 0.4f);
        glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), .3f);
    }
    
    else{
        glUniform1i(glGetUniformLocation(shaderProgram, "is_tree"), 1);
        glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 0.0f, 0.05f, 0.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 0.4f, 0.5f, 0.4f);
        glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), .004f, .7f, 0.04f);
        glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), .78125f);
        
    }
    
    glm::vec3 pointLightColors[] = {
        glm::vec3(0.4f, 0.7f, 0.1f),
        glm::vec3(0.4f, 0.7f, 0.1f),
        glm::vec3(0.4f, 0.7f, 0.1f),
        glm::vec3(0.4f, 0.7f, 0.1f)
    };
    
    //glm::vec3 pointLightPositions = glm::vec3(0.7f, 0.2f, 2.0f);
    
    //point light 1
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].position"), Window::point_light_pos.x, Window::point_light_pos.y, Window::point_light_pos.z);
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].ambient"), pointLightColors[0].x * 0.1,  pointLightColors[0].y * 0.1,  pointLightColors[0].z * 0.1);
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].diffuse"), pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].specular"), pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
    glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].linear"), 0.09);
    glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].quadratic"), 0.032);

    
    //point light 2
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].position"), Window::point_light_pos.x, Window::point_light_pos.y, Window::point_light_pos.z);
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].ambient"), pointLightColors[1].x * 0.1,  pointLightColors[1].y * 0.1,  pointLightColors[1].z * 0.1);
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].diffuse"), pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[1].specular"), pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);
    glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].linear"), 0.09);
    glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].quadratic"), 0.032);

    //point light 3
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].position"), Window::point_light_pos.x, Window::point_light_pos.y, Window::point_light_pos.z);
    
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].ambient"), pointLightColors[2].x * 0.1,  pointLightColors[2].y * 0.1,  pointLightColors[2].z * 0.1);
    
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].diffuse"), pointLightColors[2].x,  pointLightColors[2].y,  pointLightColors[2].z);
    
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[2].specular") ,pointLightColors[2].x,  pointLightColors[2].y,  pointLightColors[2].z);
    
    glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].constant"), 1.0f);
    
    glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].linear"), 0.09);
    
    glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].quadratic"), 0.032);

    
    //point light 4
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].position"), Window::point_light_pos.x, Window::point_light_pos.y, Window::point_light_pos.z);
    
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].ambient"), pointLightColors[3].x * 0.1,  pointLightColors[3].y * 0.1,  pointLightColors[3].z * 0.1);
    
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].diffuse"), pointLightColors[3].x,  pointLightColors[3].y,  pointLightColors[3].z);
    
    glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[3].specular"), pointLightColors[3].x,  pointLightColors[3].y,  pointLightColors[3].z);
    
    glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].constant"), 1.0f);
    
    glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].linear"), 0.09);
    
    glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].quadratic"), 0.032);
    
    
    //spot light
    glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.position"), Window::spot_light_pos.x,Window::spot_light_pos.y, Window::spot_light_pos.z);
    
    glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.direction"), -1.0f*Window::spot_light_pos.x, -1.0f*Window::spot_light_pos.y, -1.0f*Window::spot_light_pos.z);
    
    glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.ambient"), .5f, .5f, .5f);
    
    glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.diffuse"), .3f, .3f, .3f);
    
    glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
    
    glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.constant"), 1.0f);
    
    glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.linear"), 0.07);
    
    glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.quadratic"), 0.017);
    
    glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.cutOff"), Window::spot_radius);
    
    glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.outerCutOff"), Window::other_radius);
    
    //////////
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
    

    glBindVertexArray(0);
    
    
    // Pop the save state off the matrix stack
    // This will undo the multiply we did earlier
    //glPopMatrix();
}
void OBJObject::spin(float deg){
    this->angle += deg;
    if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
    // This creates the matrix to rotate the cube
    this->toWorld = glm::rotate(toWorld, this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}
void OBJObject::update(){
    //spin(0.001f);
}
void OBJObject::move(int mode){
    if(mode == 1){
        this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f)) * toWorld;
    }
    else if(mode == 2){
        this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f)) * toWorld;
    }
    else if(mode == 3){
        this->toWorld =glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f)) * toWorld;
    }
    else if(mode == 4){
        this->toWorld =glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f)) * toWorld;
    }
    else if(mode == 5){
        this->toWorld =glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f)) * toWorld;
    }
    else if(mode == 6){
        this->toWorld =glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f)) * toWorld;
    }
    else if(mode == 7){
        this->toWorld =glm::scale(toWorld, glm::vec3(2.0f, 2.0f, 2.0f));
    }
    else if(mode == 8){
        this->toWorld =glm::scale(toWorld, glm::vec3(0.5f, 0.5, 0.5f));
    }
    else if(mode == 9){
        model[0][0] = cosf(25.0f);
        model[0][1] = sinf(25.0f);
        model[1][0] = -sinf(25.0f);
        model[1][1] = cosf(25.0f);
        this->toWorld = model * toWorld;
    }
    else if(mode == 10){
        model[0][0] = cosf(25.0f);
        model[0][1] = -sinf(25.0f);
        model[1][0] = sinf(25.0f);
        model[1][1] = cosf(25.0f);
        this->toWorld = model * toWorld;
    }
}
void OBJObject::set_toWorld(glm::mat4 m){
    this->toWorld = m;
}
glm::mat4 OBJObject::get_toWorld(){
    glm::mat4 copy = glm::mat4(this->toWorld);
    return copy;
}
std::vector<glm::vec3> OBJObject::get_colors(){
    std::vector<glm::vec3> copy = std::vector<glm::vec3>(colors);
    return copy;
}
glm::vec3 OBJObject::normal_color(glm::vec3 v){
    v = glm::normalize(v);
    if(v.x < 0) v.x = (float)(v.x+1)/(float)2;
    if(v.y < 0) v.y = (float)(v.y+1)/(float)2;
    if(v.z < 0) v.z = (float)(v.z+1)/(float)2;
    return v;
}
std::vector<glm::vec3> OBJObject::get_vertices(){
    std::vector<glm::vec3> copy = std::vector<glm::vec3>(vertices);
    return copy;
}
void OBJObject::reset_angle(){
    this->angle = 0.0f;
}