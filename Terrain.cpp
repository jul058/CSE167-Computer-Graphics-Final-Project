//
//  Terrain.cpp
//  HW2
//
//  Created by Junxi Li on 5/25/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#include "Terrain.hpp"
#define N 1024

unsigned char* image;
int w, h, data_size;
const char* normal_ppm = "/Users/Frankie/CSE167/Final/normal_map.ppm";
const char* sandiego_ppm = "/Users/Frankie/CSE167/Final/SanDiegoTerrain.ppm";

Terrain::Terrain(){
    prev_state = false;
    max_height = INT_MIN;
    min_height = INT_MAX;
    this->toWorld = glm::mat4(1.0f);
    loadCubemap();
    create();
    int i, j;
    size_t len = normals.size();
    for(i = 0; i < len/2; i++){
        normals[i].x = image[3*i];
        normals[i].y = image[3*i+1];
        normals[i].z = image[3*i+2];
    }
    for(i = 0, j = len/2; j < len; i++, j++){
        normals[j].x = image[3*i];
        normals[j].y = image[3*i+1];
        normals[j].z = image[3*i+2];
    }
    
    
    glGenVertexArrays(1, &terrainVAO);
    glGenBuffers(1, &terrainNBO);
    glGenBuffers(1, &terrainVBO);
    glGenBuffers(1, &terrainEBO);
    
    //bind VAO
    glBindVertexArray(terrainVAO);
    
    
    //bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(v[0]), &v[0], GL_STATIC_DRAW);
    
    
    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size()*sizeof(indicies[0]), &indicies[0], GL_STATIC_DRAW);
    
    
    //VAO
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    //binding NBO
    glBindBuffer(GL_ARRAY_BUFFER, terrainNBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    glBindVertexArray(0);
}



void Terrain::draw(GLuint shaderProgram){
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Draw skybox first
    //glDepthMask(GL_FALSE);// Remember to turn depth writing off
    
    glUniform1f(glGetUniformLocation(shaderProgram, "max_height"), max_height);
    glUniform1f(glGetUniformLocation(shaderProgram, "min_height"), min_height);
    
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);
    
    
    glUniform1i(glGetUniformLocation(shaderProgram, "light_mode"), Window::light_mode);
    glUniform1i(glGetUniformLocation(shaderProgram, "prev_light_mode"), Window::prev_light_mode);
    //glUniform3f(glGetUniformLocation(shade))
    
    glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), 0.0f, 0.0f, 20.0f);
    //directional light
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), Window::dir_light_dir.x, Window::dir_light_dir.y, Window::dir_light_dir.z);
    //glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), .3f, .3f, .3f);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), .2f, .2f, .2f);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), 1.0f, 1.0f, 1.0f);
    
    glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 0.2f, .2f, .2f);
    glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), .5f, .5f, .5f);
    glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 1.0f);
    
    srand(10);
    x_offset = ((rand()%100)/100)*2-1;
    y_offset = ((rand()%100)/100)*2-1;
    z_offset = ((rand()%100)/100)*2-1;
    glUniform1f(glGetUniformLocation(shaderProgram, "x_offset"), x_offset);
    glUniform1f(glGetUniformLocation(shaderProgram, "y_offset"), y_offset);
    glUniform1f(glGetUniformLocation(shaderProgram, "z_offset"), z_offset);
    
    glm::vec3 pointLightColors[] = {
        glm::vec3(0.4f, 0.7f, 1.0f),
        glm::vec3(0.4f, 0.7f, 1.0f),
        glm::vec3(0.4f, 0.7f, 1.0f),
        glm::vec3(0.4f, 0.7f, 1.0f)
    };
    
    //glm::vec3 pointLightPositions = glm::vec3(0.7f, 0.2f, 2.0f);
    glUniform1i(glGetUniformLocation(shaderProgram, "is_tree"), 0);
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
    
    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindVertexArray(terrainVAO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
    glDrawElements(GL_TRIANGLES, (int)indicies.size(), GL_UNSIGNED_INT, 0);
    
    
    //glDrawElements(GL_TRIANGLES, (unsigned int)indicies.size(), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_POINTS, 0, (int)v.size());
    //glDrawArrays(GL_TRIANGLES, 0, (int)v.size());
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
}

void Terrain::create(){
    arr = new GLfloat*[N+1];
    for(int i = 0; i < N+1; i++){
        arr[i] = new GLfloat[N+1];
    }
    data_size = N+1;
    double seed = 100.0;
    arr[0][0] = arr[0][data_size-1] = arr[data_size-1][0] = arr[data_size-1][data_size-1]=seed;
    double h = 360.0;
    srand(10);
    for(int side_len = data_size-1; side_len>=2; side_len/=2, h/=2.0){
        int halfSide = side_len/2;
        for(int x = 0; x < data_size-1; x+=side_len){
            for(int y = 0; y < data_size-1; y+=side_len){
                double avg = arr[x][y]+arr[x+side_len][y]+arr[x][y+side_len]+arr[x+side_len][y+side_len];
                avg /= 4.0;
                arr[x+halfSide][y+halfSide] = avg + ((double)((rand()%100)+1)/100.0*2*h)-h;
                
                //h /= 1.1;
                //arr[x+halfSide][y+halfSide] = avg + (2*h)-h;
            }
        }
        
        for(int x=0;x<data_size-1;x+=halfSide){
            for(int y=(x+halfSide)%side_len;y<data_size-1;y+=side_len){
                double avg =
                arr[(x-halfSide+data_size)%data_size][y] +
                arr[(x+halfSide)%data_size][y] +
                arr[x][(y+halfSide)%data_size] +
                arr[x][(y-halfSide+data_size)%data_size];
                avg /= 4.0;
                
                
                avg = avg + ((double)((rand()%100)+1)/100.0*2*h) - h;
                //h/=1.5;
                //avg = avg + (2*h) - h;
                arr[x][y] = avg;
                
                if(x == 0)  arr[data_size-1][y] = avg;
                if(y == 0)  arr[x][data_size-1] = avg;
            }
        }
    }
    int iterations = 15;
    double k = 0.5;
    while(iterations--){
        //row, left to right
        for(int x = 1; x < N+1; x++){
            for(int z = 0; z < N+1; z++){
                arr[x][z] = arr[x-1][z]*(1-k)+arr[x][z]*k;
            }
        }
        
        //row, right to left
        for(int x = N-2+1; x >= 0; x--){
            for(int z = 0; z < N+1; z++){
                arr[x][z] = arr[x+1][z]*(1-k)+arr[x][z]*k;
            }
        }
        
        //col, buttom to top
        for(int x = 0; x < N+1; x++){
            for(int z = 1; z < N+1; z++){
                arr[x][z] = arr[x][z-1]*(1-k)+arr[x][z]*k;
            }
        }
        
        //col, top to buttom
        for(int x = 0; x < N+1; x++){
            for(int z = N; z >= 0; z--){
                arr[x][z] = arr[x][z+1]*(1-k)+arr[x][z]*k;
            }
        }
    }
    
    
    int u,w;
    u = w = 0;
    for(GLfloat i = 0; i < N+1; i+=1.0f, u++){
        w=0;
        for(GLfloat j = 0; j < N+1; j+=1.0f,w++){
            v.push_back(glm::vec3(i-N/2, arr[u][w]-N/3, j-N/2));
            if(arr[u][w]-N/3 > max_height){
                max_x = i;
                max_z = j;
            }
            max_height = std::max(max_height,arr[u][w]-N/3);
            min_height = std::min(min_height, arr[u][w]-N/3);
            
        }
    }
    
    //std::cout << "max height"<<max_height;
    printf("max height: %f\n", max_height);
    printf("min height: %f\n", min_height);
    glm::vec3 tmp1, tmp2, nor;
    int left, right, down, up;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            left = i*(N+1)+j, right = i*(N+1)+j+1, down = (i+1)*(N+1)+j;
            indicies.push_back(left);
            indicies.push_back(right);
            indicies.push_back(down);
            
            tmp1 = v[down]-v[left];
            tmp2 = v[right]-v[left];
            nor = glm::cross(tmp1, tmp2);
            //nor = glm::normalize(nor * 2.0f - 1.0f);
            normals2.push_back(nor);
        }
    }
    for(int i = 0; i < N; i++){
        for(int j = 1; j < N+1; j++){
            up = i*(N+1)+j, left = i*(N+1)+j+N, right = (i+1)*(N+1)+j;
            
            indicies.push_back(up);
            indicies.push_back(right);
            indicies.push_back(left);
            
            tmp1 = v[up]-v[right];
            tmp2 = v[left]-v[right];
            nor = glm::cross(tmp1, tmp2);
            //nor = glm::normalize(nor * 2.0f - 1.0f);
            normals.push_back(nor);
            normals2.push_back(nor);
        }
    }
}

void Terrain::change(){
    if(prev_state != use_bump){
        if(use_bump){
            //normals.clear();
            size_t len = normals.size();
            int i, j;
            for(i = 0; i < len/2; i++){
                normals[i].x = image[3*i];
                normals[i].y = image[3*i+1];
                normals[i].z = image[3*i+2];
            }
            for(i = 0, j = len/2; j < len; i++, j++){
                normals[j].x = image[3*i];
                normals[j].y = image[3*i+1];
                normals[j].z = image[3*i+2];
            }
        }
        else{
            normals.clear();
            normals.assign(normals2.begin(), normals2.end());
        }
        prev_state = use_bump;
        
        
        //binding NBO
        glBindBuffer(GL_ARRAY_BUFFER, terrainNBO);
        glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        
        glBindVertexArray(0);

    }
}

unsigned char* Terrain::loadPPM(const char* filename, int& width, int& height)
{
    const int BUFSIZE = 128;
    FILE* fp;
    unsigned int read;
    unsigned char* rawData;
    char buf[3][BUFSIZE];
    char* retval_fgets;
    size_t retval_sscanf;
    
    if ( (fp=fopen(filename, "rb")) == NULL)
    {
        std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
        width = 0;
        height = 0;
        return NULL;
    }
    
    // Read magic number:
    retval_fgets = fgets(buf[0], BUFSIZE, fp);
    
    // Read width and height:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
    width  = atoi(buf[1]);
    height = atoi(buf[2]);
    
    // Read maxval:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    
    // Read image data:
    rawData = new unsigned char[width * height * 3];
    read = fread(rawData, width * height * 3, 1, fp);
    fclose(fp);
    if (read != 1)
    {
        std::cerr << "error parsing ppm file, incomplete data" << std::endl;
        delete[] rawData;
        width = 0;
        height = 0;
        return NULL;
    }
    w = width;
    h = height;
    return rawData;
}

GLuint Terrain:: loadCubemap(){
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width, height;
    
    image = loadPPM(normal_ppm, width, height); //raw data
    
    
    
    cubemapTexture = textureID;
    return textureID;
}
