#include "window.h"
#include "OBJObject.h"
#include "skyBox.hpp"
#include "Bezier.hpp"
#include "Point.hpp"
#include "Handle.hpp"
#include "Environment.hpp"
#include "Terrain.hpp"
#include "Tree.hpp"
#include "Particle.hpp"
#include "Box.hpp"
#include "Controller.hpp"
#include "Sound.hpp"
#include <iostream>

#include <glm/gtx/rotate_vector.hpp>
#include <iostream>
using namespace std;
const char* window_title = "GLFW Starter Project";
const char* sound_path = "/Users/Frankie/CSE167/Final/Wind.mp3";
const char* pain_path = "/Users/Frankie/CSE167/Final/Pain.mp3";

Cube * cube;
OBJObject bunny, *bear, dragon;
OBJObject *bunny_ptr, *bunny1, *bunny2;
OBJObject *curr, *cylinder, *pod;
skyBox *skybox_ptr;
Bezier *curve;
Point* selected, *p1_tmp, *p2_tmp;
Environment* e_map;
Terrain *ter;
Tree *tree1, *tree2, *tree3;
Particle *par;
Box *box, *box2;
Controller* control, *control2, *snowboarder, *snowboard_controller;
Sound *bgm, *pain;
OBJObject* person;
OBJObject* snowboard;


bool direction[4];

GLfloat pod_speed;
GLfloat prev_speed;
GLfloat pod_velocity;
glm::vec3 initial_point;

bool draw_box;
bool bump_show;

std::vector<Point*> points;
std::vector<Handle*> handles;
std::vector<Bezier*> group_of_curve;
int max_ind;
int max_cuvre_id;
glm::vec3 prev_point;
glm::vec3 curr_point;
GLfloat delta_h;
GLfloat tmp_speed;
GLfloat highest = 0.0f;

bool entered = false;
bool use_person_cam = false;
GLint shaderProgram;
GLint skybox_shader;
GLint curve_shader;
GLint point_shader;
GLint handle_shader;
GLint environment_shader;
GLint terrain_shader;
GLint box_shader;
GLint particle_shader;
GLint tree_shader;

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 1000.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

glm::mat4 bear_cam;
glm::mat4 store_cam;
glm::mat4 skybox_cam;
glm::mat4 Window::initial_toWorld;
GLuint Window::cubemapTexture_window;
int Window::object_num;
int Window::width;
int Window::height;
int Window::light_mode;
int mode;
bool release;
bool move_handle=false;
bool move_anchor;
int selected_id;

int Window::prev_light_mode;
float Window::spot_radius = glm::cos(glm::radians(12.5f));
float Window::other_radius = glm::cos(glm::radians(13.0f));
unsigned char* Window::image_data;
glm::vec3 static_light_pos;
glm::vec3 Window::spot_light_dir;
glm::vec3 Window::dir_light_dir;
glm::vec3 Window::point_light_pos;
glm::vec3 Window::spot_light_pos;
glm::mat4 Window::P;
glm::mat4 Window::V;
glm::mat4 oldv = Window::V;
glm::vec3 last_point;
glm::mat4 S, T, R, T_inv;
GLfloat up_down_count=0.0f;
GLfloat expand_count=0.0f;
bool vertical_go_up=true;
bool should_expand=true;
bool split = false;

glm::vec3 p0_g[8] = {glm::vec3(-5.0f,-105.0f,-2.5f), glm::vec3(59.0f, -134.0f, 53.0f), glm::vec3(114.45f,-133.33f, 68.32f), glm::vec3(132.14f, -154.20f, 130.15f), glm::vec3(315.73f, -235.25f, 57.88f), glm::vec3(382.86f, -225.19f, 77.81f), glm::vec3(97.0f, -197.25f, 173.38f), glm::vec3(-53.37f, -154.34f, 101.15f) };

glm::vec3 p2_g[8] = {glm::vec3(39.864216f, -125.51f, 32.99f), glm::vec3(101.21f, -136.49f,68.71f), glm::vec3(130.24f, -157.08f, 120.51f), glm::vec3(268.0f, -183.20f, 19.66f), glm::vec3(398.5f, -255.2f, 118.15), glm::vec3(107.21, -198.34f, 202.35f), glm::vec3(-24.85f, -135.38f, 106.79f), glm::vec3(-40.75f, -103.28f, 1.99f)  };

glm::vec3 p3_g[8] = {p0_g[1], p0_g[2],  p0_g[3], p0_g[4], p0_g[5], p0_g[6], p0_g[7], p0_g[0]};

glm::vec3 control_points[8][4]=
{   {   p0_g[0], p2_g[7]+(p3_g[7]-p2_g[7])*2.0f, p2_g[0], p3_g[0]},
    
    {   p0_g[1], p2_g[0]+(p3_g[0]-p2_g[0])*2.0f, p2_g[1], p3_g[1]  },
    
    {   p0_g[2], p2_g[1]+(p3_g[1]-p2_g[1])*2.0f, p2_g[2], p3_g[2]  },
    
    {   p0_g[3], p2_g[2]+(p3_g[2]-p2_g[2])*2.0f, p2_g[3], p3_g[3]  },
    
    {   p0_g[4], p2_g[3]+(p3_g[3]-p2_g[3])*2.0f, p2_g[4], p3_g[4]  },
    
    {   p0_g[5], p2_g[4]+(p3_g[4]-p2_g[4])*2.0f, p2_g[5], p3_g[5]  },
    
    {   p0_g[6], p2_g[5]+(p3_g[5]-p2_g[5])*2.0f, p2_g[6], p3_g[6]  },
    
    {   p0_g[7], p2_g[6]+(p3_g[6]-p2_g[6])*2.0f, p2_g[7], p3_g[7]  }
};


double last_x, curr_x, diff_x;
double last_y, curr_y, diff_y;
float x_sign, y_sign;


void Window::initialize_objects()
{
    draw_box = false;
    bump_show = false;
    
    spot_light_pos = glm::vec3(400.0f, 0.0f, 0.0f);
    spot_light_dir = glm::vec3(0.0f, -1.0f, 0.0f);
    dir_light_dir = glm::vec3(0.0f, -1.0f, 0.0f);
    point_light_pos = glm::vec3(300.0f, 0.0f, 0.0f);
    
    
    
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), Window::dir_light_dir.x, Window::dir_light_dir.y, Window::dir_light_dir.z);
    cylinder = new OBJObject("/Users/Frankie/CSE167/HW2/cylinder.obj", true);
    cylinder->is_tree = true;
    //person = new OBJObject("")
    
    //cube = new Cube();
    skybox_ptr = new skyBox();
    
    ter = new Terrain();
    glm::mat4 tree_translate1 = glm::translate(glm::mat4(1.0f), glm::vec3(128.0f, ter->arr[128+512][128+512]-1024.0f/3.0f, 128.0f));
    tree1 = new Tree(4, cylinder, 25.7f, "F[+X][-X]FX", "FF", tree_translate1);
    
    glm::mat4 tree_translate2 = glm::translate(glm::mat4(1.0f), glm::vec3(-256.0f, ter->arr[-256+512][-256+512]-1024.0f/3.0f, -256.0f));
    tree2 = new Tree(4, cylinder, 20.0f, "F[+X]F[-X]+X", "FF", tree_translate2);
    
    glm::mat4 tree_translate3 = glm::translate(glm::mat4(1.0f), glm::vec3(-128.0f, ter->arr[-128+512][-128+512]-1024.0f/3.0f, -128.0f));
    tree3 = new Tree(4, cylinder, 22.5f, "F-[[X]+X]+F[+FX]-X", "FF", tree_translate3);
    

    
    
    par = new Particle();
    
    bunny1 = new OBJObject("/Users/Frankie/CSE167/CSE167 HW1/bunny.obj", true);
    bunny2 = new OBJObject("/Users/Frankie/CSE167/CSE167 HW1/bunny.obj", true);
    bear = new OBJObject("/Users/Frankie/CSE167/CSE167 HW1/bear.obj", true);
    
    
    glm::vec3 board_translation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 board_scale = glm::vec3(1.0f, 1.0f, 1.0f);
//    glm::vec3 test = glm::vec3(512-8, ter->arr[512-8][512-17]-1024/3.0f, 512-17);
//    bear->toWorld[3] = glm::vec4(test, 1.0f);
    snowboarder = new Controller(bear, ter, 512, 512, 15.0f, 20.0f, 25.0f, 20.0f, 10.0f, board_translation, board_scale);
    //snowboarder->box->move(glm::vec3(-10.0f, -115.0f-(ter->arr[512][512]-1024.0f/3.0f) ,-9.0f));
    
    
    
    snowboard = new OBJObject("/Users/Frankie/CSE167/HW2/cylinder.obj", true);
    //snowboard->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(22.5f, 1.0f, 10.0f))*snowboard->toWorld;
    //snowboard->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -105.0f, 0.0f))*snowboard->toWorld;
    
    
    board_translation = glm::vec3(0.0f, -108.0f, 0.0f);
    board_scale = glm::vec3(20.0f, 1.0f, 10.0f);
    //snowboard->toWorld = glm::rotate(snowboard->toWorld, 180.0f/180.0f*glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
    snowboard_controller = new Controller(snowboard, ter, 512, 512, 1.0f, 20.0f, 25.0f, 20.0f, 2.5f, board_translation, board_scale);
    
//    box = new Box(bunny1);
//    box2 = new Box(bunny2);
    
    //control = new Controller(bunny1, ter, 512, 512, 30.0f, 3.0f, 3.0f, 2.5f, 10.0f);
    
    glm::vec3 tmp = glm::vec3(128.0f, ter->arr[512+128][512+128]-1024.0f/3.0f, 128.0f);
    bunny2->toWorld[3] = glm::vec4(tmp, 1.0f);

    control2 = new Controller(bunny2, ter, 512+128, 512+128, 30.0f, 3.0f, 3.0f, 2.5f, 10.0f, glm::vec3(0.0f), glm::vec3(1.0f));
    
    bgm = new Sound(sound_path);
    pain = new Sound(pain_path);
    
    //ter->loadCubemap();

    /*for(int i = 0; i < 8; i++){
        group_of_curve.push_back(new Bezier(control_points[i][0],control_points[i][1],control_points[i][2],control_points[i][3]));
    }
    
    
    for(int i = 0; i < 8; i++){
        if(highest < group_of_curve[i]->highest.y){
            highest = group_of_curve[i]->highest.y;
            max_ind = group_of_curve[i]->max_ind;
            max_cuvre_id = i;
            prev_point = curr_point = initial_point = group_of_curve[i]->highest;
        }
    }
    printf("initilized: %f, %f, %f\n", prev_point.x, prev_point.y, prev_point.z);
    for(int i = 0; i < 24; i++){
        points.push_back(new Point(control_points[i/3][i%3], i));
    }
    
    int p1_ind, p2_ind;
    for(int i = 0; i < 8; i++){
        p1_ind = i*3+1;
        p2_ind = (i*3+23)%24;
        handles.push_back(new Handle(control_points[p2_ind/3][p2_ind%3], control_points[p1_ind/3][p1_ind%3]));
    }*/
    
    /*pod = new OBJObject("/Users/Frankie/CSE167/HW2/pod.obj");
    pod->toWorld[3][0] = prev_point.x;
    pod->toWorld[3][1] = prev_point.y;
    pod->toWorld[3][2] = prev_point.z;
    e_map = new Environment(pod);
    pod_speed = (GLfloat)max_ind/200.0f+0.01f;*/
    
    object_num = 1;
    //handles.push_back(new Handle(glm::vec3(10.0f,10.0f,10.0f), glm::vec3(20.0f,20.0f,20.0f)));
    //bunny = OBJObject("/Users/Frankie/CSE167/CSE167 HW1/bunny.obj");
    //bear = OBJObject("/Users/Frankie/CSE167/CSE167 HW1/bear.obj");
    //dragon = OBJObject("/Users/Frankie/CSE167/CSE167 HW1/dragon.obj");
    curr = &bunny;
    
    
    
    // Load the shader program. Similar to the .obj objects, different platforms expect a different directory for files
#ifdef _WIN32 // Windows (both 32 and 64 bit versions)
    shaderProgram = LoadShaders("../shader.vert", "../shader.frag");
#else // Not windows
    shaderProgram = LoadShaders("/Users/Frankie/CSE167/Final/shader.vert", "/Users/Frankie/CSE167/Final/shader.frag");
    skybox_shader = LoadShaders("/Users/Frankie/CSE167/Final/skybox.vert", "/Users/Frankie/CSE167/Final/skybox.frag");
    curve_shader = LoadShaders("curve.vert", "curve.frag");
    point_shader = LoadShaders("point.vert", "point.frag");
    handle_shader = LoadShaders("/Users/Frankie/CSE167/HW2/handle.vert", "/Users/Frankie/CSE167/HW2/handle.frag");
    environment_shader = LoadShaders("environment.vert", "environment.frag");
    terrain_shader = LoadShaders("terrain.vert", "terrain.frag");
    box_shader = LoadShaders("/Users/Frankie/CSE167/Final/HW2/box_shader.vert", "/Users/Frankie/CSE167/Final/HW2/box_shader.frag");
    particle_shader = LoadShaders("/Users/Frankie/CSE167/Final/HW2/particle_shader.vert", "/Users/Frankie/CSE167/Final/HW2/particle_shader.frag");
    tree_shader = LoadShaders("/Users/Frankie/CSE167/Final/HW2/tree_shader.vert", "/Users/Frankie/CSE167/Final/HW2/tree_shader.frag");
#endif

    cubemapTexture_window = skybox_ptr->loadCubemap();
}



void Window::clean_up()
{
    //delete(cube);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(skybox_shader);
}

GLFWwindow* Window::create_window(int width, int height)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }
    
    // 4x antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // Create the GLFW window
    GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);
    
    // Check if the window could not be created
    if (!window)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return NULL;
    }
    
    // Make the context of the window
    glfwMakeContextCurrent(window);
    
    // Set swap interval to 1
    glfwSwapInterval(1);
    
    // Get the width and height of the framebuffer to properly resize the window
    glfwGetFramebufferSize(window, &width, &height);
    // Call the resize callback to make sure things get drawn immediately
    Window::resize_callback(window, width, height);
    
    return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
    Window::width = width;
    Window::height = height;
    // Set the viewport size
    glViewport(0, 0, width, height);
    
    if (height > 0)
    {
        P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 10000.0f);
        V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        store_cam = V;
    }
}

void Window::idle_callback()
{
    if(use_person_cam){
        V = glm::inverse(snowboarder->obj->toWorld * glm::rotate(glm::mat4(1.0f), glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)));
        V = V*glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 30.0f, 0.0f));
    }
    
    if(snowboarder->box->collide(control2->box)){
        pain->play=true;
        pain->pain_sound();
    }
    else{
        pain->play=false;
        pain->time = 0;
    }
    
    if(bump_show){
        ter->use_bump = true;
        ter->change();
        
    }
    else{
        ter->use_bump = false;
        ter->change();
    }
    

    if(direction[0]){
        snowboarder->move(0);
        snowboard_controller->move(0);
        //control->direction();
    }
    if(direction[1]){
        snowboarder->move(1);
        snowboard_controller->move(1);
        //control->direction();
    }
    if(direction[2]){
        snowboarder->move(2);
        snowboard_controller->move(2);

        //control->direction();

    }
    if(direction[3]){
        snowboarder->move(3);
        snowboard_controller->move(3);

        //control->direction();

    }
    
    
    //if(selected_id!=0)
      //  printf("%f, height: %f, %f\n", points[selected_id]->v[0].x, points[selected_id]->v[0].y, points[selected_id]->v[0].z);
    //if(b)
    //printf("%d\n", box->collide(box2));
    /*if(control->box->collide(control2->box)){
        //pain->pain_sound();
        pain->play=true;
        pain->pain_sound();
    }
    else{
        pain->play=false;
        pain->time = 0;
    }*/
    
    /*glm::vec3 p0, p1, p2, p3;
     p0 = group_of_curve[max_cuvre_id]->p0;
     p1 = group_of_curve[max_cuvre_id]->p1;
     p2 = group_of_curve[max_cuvre_id]->p2;
     p3 = group_of_curve[max_cuvre_id]->p3;
     
     if(pod_velocity < 0.000010f && curr_point.y <= highest){
     printf("!!!!!!!!!!\n");
     //pod_speed = (GLfloat)max_ind/200.0f+0.1f;
     pod_velocity += 0.0005f;
     }
     curr_point = e_map->B(pod_speed, p0, p1, p2, p3);
     e_map->update(curr_point);
     e_map->orientation(curr_point, prev_point);
     //delta_h = glm::abs(curr_point.y-prev_point.y);
     delta_h = -curr_point.y+prev_point.y;
     
     //pod_velocity = glm::sqrt(e_map->gravity*2*(-1)*delta_h);
     pod_velocity += delta_h/1000.0f;
     //printf("velocity: %f\n", pod_velocity);
     
     pod_speed += pod_velocity;
     prev_point = curr_point;
     if(pod_speed>1.0f){
     max_cuvre_id = (max_cuvre_id+1)%8;
     pod_speed -= 1.0f;
     }
     if(pod_speed != pod_speed){
     
     }*/
    //printf("%f\n", pod_speed);
    
}

void Window::display_callback(GLFWwindow* window)
{
    // Clear the color and depth buffers
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    
    glUseProgram(skybox_shader);
    skybox_ptr->draw(skybox_shader);
    
    glUseProgram(shaderProgram);
    ter->draw(shaderProgram);
    //bunny1->draw(shaderProgram);
    
    glUseProgram(particle_shader);
    par->draw(particle_shader);
    
    if(draw_box){
        glUseProgram(box_shader);
        //control->box->draw(box_shader);
        control2->box->draw(box_shader);
        snowboarder->box->draw(box_shader);
        snowboard_controller->box->draw(box_shader);
    }
    
    glUseProgram(environment_shader);
    //control->draw(environment_shader, cam_pos);
    control2->draw(environment_shader, cam_pos);
    snowboarder->draw(environment_shader, cam_pos);
    //snowboard->draw(environment_shader);
    snowboard_controller->draw(environment_shader, cam_pos);
    
//    glUseProgram(tree_shader);
//    tree1->draw(tree_shader);
//    tree2->draw(tree_shader);
//    tree3->draw(tree_shader);
    
    glUseProgram(shaderProgram);
    tree1->draw(shaderProgram);
    tree2->draw(shaderProgram);
    tree3->draw(shaderProgram);

    
//    glUseProgram(shaderProgram);
//    person->draw(shaderProgram);
    
    
//    glUseProgram(curve_shader);
//    for(int i = 0; i < group_of_curve.size(); i++){
//        group_of_curve[i]->draw(curve_shader);
//    }
//    
//    glUseProgram(point_shader);
//    for(int i = 0; i < points.size(); i++){
//        points[i]->draw(point_shader);
//    }
//    
//    glUseProgram(handle_shader);
//    for(int i = 0; i < handles.size(); i++){
//        handles[i]->draw(handle_shader);
//    }
    
    //     glUseProgram(environment_shader);
    //     e_map->draw(environment_shader, cam_pos);
    
    
    // Use the shader of programID
    //glUseProgram(shaderProgram);
    //cube->draw(skybox_shader);
    // Gets events, including input such as keyboard and mouse or window resizing
    glfwPollEvents();
    // Swap buffers
    glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Check for a key press
    if (action == GLFW_PRESS)
    {
        // Check if escape was pressed
        if (key == GLFW_KEY_ESCAPE)
        {
            // Close the window. This causes the program to also terminate.
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        else if(key== GLFW_KEY_D)
        {
            cam_look_at.x += 10.0f;
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        
        else if(key== GLFW_KEY_A)
        {
            cam_look_at.x -= 10.0f;
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        
        else if(key== GLFW_KEY_W)
        {
            cam_look_at.y += 10.0f;
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        
        else if(key== GLFW_KEY_S)
        {
            cam_look_at.y -= 10.0f;
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        else if(key == GLFW_KEY_T){
            draw_box = !draw_box;
        }
        else if(key == GLFW_KEY_B){
            bump_show = !bump_show;
        }
        
        else if(key == GLFW_KEY_R){
            highest = 0.0f;
            for(int i = 0; i < group_of_curve.size(); i++){
                for(int j = 0; j < group_of_curve[i]->arr.size(); j++){
                    if(group_of_curve[i]->arr[j].y > highest){
                        max_cuvre_id = i;
                        highest = group_of_curve[i]->arr[j].y;
                        prev_point = curr_point = initial_point = group_of_curve[i]->arr[j];
                        max_ind = j;
                    }
                }
            }
//            printf("%d, %d %f, %f, %f\n", max_cuvre_id, max_ind, prev_point.x, prev_point.y, prev_point.z);
            pod->toWorld[3][0] = prev_point.x;
            pod->toWorld[3][1] = prev_point.y;
            pod->toWorld[3][2] = prev_point.z;
            pod_speed = (GLfloat)max_ind/200.0f+0.01f;
            pod_velocity = 0.001f;
        }
        else if(key == GLFW_KEY_1){
            prev_light_mode = light_mode;
            light_mode = 1;
        }
        else if(key == GLFW_KEY_2){
            prev_light_mode = light_mode;
            light_mode = 2;
        }
        else if(key == GLFW_KEY_3){
            prev_light_mode = light_mode;
            light_mode = 3;
        }
        else if(key == GLFW_KEY_0){
            prev_light_mode = light_mode;
            light_mode = 0;
        }
        else if(key == GLFW_KEY_C){
            use_person_cam = !use_person_cam;
            cout << use_person_cam << endl;
            if(use_person_cam){
                cout << use_person_cam << endl;
                store_cam = V;
                //Window::V = bunny_node->M;
            }
            else{
                cout << "reset" << endl;
                V = store_cam;
            }
        }
        else if(key == GLFW_KEY_UP){
            //control->move(0);
//            printf("pressing.\n");
            direction[0]=true;
        }
        else if(key == GLFW_KEY_LEFT){
            //control->move(1);
            direction[1]=true;
        }
        else if(key == GLFW_KEY_RIGHT){
//            control->move(2);
            direction[2]=true;
        }
        else if(key == GLFW_KEY_DOWN){
            //            control->move(2);
            direction[3]=true;
        }

        
        
    }
    else if(action != GLFW_REPEAT){
//        printf("not pressing\n");
        for(int i = 0; i < 4; i++){
            direction[i]=false;
        }
    }
}
//on mouse call back
void Window::cursor_position_callback(GLFWwindow* window, double xPos, double yPos){
    glm::vec3 direction, currPoint, temp;
    glm::vec4 tmp;
    currPoint = trackBallMapping(xPos, yPos);
    float root_angle, velocity;
    // rotate the object
    if(mode == 1 && !light_mode){
        direction = currPoint - last_point;
        velocity = glm::length(direction);
        if(velocity > 1e-4){
            glm::vec3 rotAxis;
            rotAxis = glm::cross(last_point, currPoint);
            root_angle = velocity * 5.0f;
            if(!release && last_point != currPoint){
                /////////////////   initial code PLEASE KEEP    //////////
                //curr->set_toWorld(glm::rotate(curr->get_toWorld(), root_angle, rotAxis));
                glfwGetCursorPos(window, &xPos, &yPos);
                if(!move_handle){
                    tmp = glm::rotate(glm::mat4(1.0f), root_angle, rotAxis) * glm::vec4(cam_pos,1.0f);
                    cam_pos = glm::vec3(tmp.x, tmp.y, tmp.z);
                    
                    tmp = glm::rotate(glm::mat4(1.0f), root_angle, rotAxis) * glm::vec4(cam_up,1.0f);
                    cam_up = glm::vec3(tmp.x, tmp.y, tmp.z);
                    
                    //cam_look_at = glm::vec3(0.0f)-cam_pos;
                    V = glm::lookAt(cam_pos, cam_look_at, cam_up);
                    
                }
                else{
                    glm::vec3 zaxis = glm::normalize(cam_pos-cam_look_at);
                    glm::vec3 xaxis = glm::cross(glm::normalize(cam_up), zaxis);
                    glm::vec3 yaxis = -glm::cross(zaxis, xaxis);
                    
                    double diff_x = (xPos-last_x)/10.0;
                    double diff_y = (yPos-last_y)/10.0;
                    int second_handle;
                    if(move_anchor){
                        
                        p1_tmp = points[selected_id+1];
                        p2_tmp = points[(selected_id+23)%24];
                        
                        selected->update(diff_x, diff_y, xaxis, yaxis, zaxis);
                        p1_tmp->update(diff_x, diff_y, xaxis, yaxis, zaxis);
                        p2_tmp->update(diff_x, diff_y, xaxis, yaxis, zaxis);
                        
                        handles[selected_id/3]->update(p2_tmp->v[0], p1_tmp->v[0]);
                        
                        group_of_curve[selected_id/3]->update(points[selected_id]->v[0], points[selected_id+1]->v[0], points[selected_id+2]->v[0], points[(selected_id+3)%24]->v[0]);
                        group_of_curve[(selected_id/3-1+8)%8]->update(points[(selected_id+21)%24]->v[0], points[(selected_id+22)%24]->v[0], points[(selected_id+23)%24]->v[0], points[selected_id]->v[0]);
                    }
                    else{
                        int target, chosen_handle;
                        if(selected_id%3==1){
                            target = (selected_id-1+23)%24;
                            chosen_handle = (selected_id-1)/3;
                        }
                        else{
                            target = (selected_id+3+23)%24;
                            chosen_handle = ((selected_id+1)%24)/3;
                        }
                        p1_tmp = points[selected_id];
                        p2_tmp = points[target];
                        
                        p1_tmp->update(diff_x, diff_y, xaxis, yaxis, zaxis);
                        p2_tmp->update(-diff_x, -diff_y, xaxis, yaxis, zaxis);
                        
                        handles[chosen_handle]->update(p1_tmp->v[0], p2_tmp->v[0]);
                        second_handle = (chosen_handle-1+8)%8;
                        group_of_curve[chosen_handle]->update(points[chosen_handle*3]->v[0], points[chosen_handle*3+1]->v[0], points[chosen_handle*3+2]->v[0], points[(chosen_handle*3+3)%24]->v[0]);
                        group_of_curve[second_handle]->update(points[second_handle*3]->v[0], points[second_handle*3+1]->v[0], points[second_handle*3+2]->v[0], points[(second_handle*3+3)%24]->v[0]);
                    }
                }
            }
        }
    }
    //move the object
    else if(mode == 2 && !light_mode){
        diff_x = (xPos - last_x)/20.0f;
        diff_y = (yPos - last_y)/20.0f;
        if(!release)
            curr->set_toWorld(glm::translate(glm::mat4(1.0f), glm::vec3(diff_x, -diff_y, 0.0f))*curr->get_toWorld());
    }
    //direct light
    else if(mode == 1 && light_mode == 1){
        
        if(!release){
            if(last_point != currPoint){
                glm::vec3 rotAxis;
                rotAxis = glm::cross(last_point, currPoint);
                tmp = glm::rotate(glm::mat4(1.0f), 0.1f, rotAxis) * glm::vec4(dir_light_dir, 1.0f);
                dir_light_dir = glm::vec3(tmp.x, tmp.y, tmp.z);
            }
        }
    }
    //point light
    else if(mode == 1 && light_mode == 2){
        if(!release){
            if(last_point != currPoint){
                glm::vec3 rotAxis;
                rotAxis = glm::cross(last_point, currPoint);
                tmp = glm::rotate(glm::mat4(1.0f), .1f, rotAxis) * glm::vec4(point_light_pos, 1.0f);
                point_light_pos = glm::vec3(tmp.x, tmp.y, tmp.z);
            }
        }
    }
    //spot light
    else if(mode == 1 && light_mode == 3){
        if(!release){
            if(last_point!=currPoint){
                glm::vec3 rotAxis;
                rotAxis = glm::cross(last_point, currPoint);
                tmp = glm::rotate(glm::mat4(1.0f), .03f, rotAxis) * glm::vec4(spot_light_pos, 1.0f);
                spot_light_pos = glm::vec3(tmp.x, tmp.y, tmp.z);
            }
        }
    }
    //spot light zoom effect
    else if(mode == 2 && light_mode == 3){
        diff_x = (xPos - last_x);
        diff_y = (yPos - last_y);
        if(!release){
            if(diff_y > 0)
                spot_radius += .1f;
            else if(diff_y < 0)
                spot_radius = (spot_radius - 0.1f <= other_radius) ? other_radius : spot_radius-.1f;
        }
    }
    last_point = currPoint;
    last_y = yPos;
    last_x = xPos;
}
void Window::cursor_enter_callback(GLFWwindow* window, int enter){
    if(enter){
        entered = true;
    }
    else{
        entered = false;
    }
}
//on button down
void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(action == GLFW_RELEASE){
        release = true;
        move_handle = false;
    }
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        release = false;
        glfwGetCursorPos(window, &last_x, &last_y);
        /*if(last_x <= width/4){
         x_sign = -1.0f;
         }
         else{
         x_sign = 1.0f;
         }
         if(last_y >= height/4){
         y_sign = -1.0f;
         }
         else{
         y_sign = 1.0f;
         }*/
        mode = 1;
        unsigned char pix[4];
        GLint viewport[4];
        last_point = trackBallMapping(last_x, last_y);
        //last_x*=2;
        //last_y*=2;
        glUseProgram(point_shader);
        
        
        glGetIntegerv(GL_VIEWPORT, viewport);
        glReadPixels(last_x*2, Window::height-last_y*2, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pix);
        
        /*if((unsigned int)pix[0] < 24){
            move_handle=true;
            selected = points[(unsigned int)pix[0]];
            selected_id=pix[0];

            if(pix[0]%3==0){
                move_anchor=true;
            }
            else{
                move_anchor=false;
            }
            printf("%d\n", (int)pix[0]);
            printf("%f, height: %f, %f\n", points[selected_id]->v[0].x, points[selected_id]->v[0].y, points[selected_id]->v[0].z);
        }*/
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        release = false;
        glfwGetCursorPos(window, &last_x, &last_y);
        mode = 2;
    }
}
void Window::scroll_callback(GLFWwindow* window, double xOffset, double yOffset){
    glm::vec3 direction;
    if(!light_mode){
        if(!move_handle){
            if(yOffset>0.0){
                    if(glm::abs(cam_pos.x) <= 768.0f && glm::abs(cam_pos.y) <= 768.0f && glm::abs(cam_pos.z) <= 768.0f){
                    cam_pos = cam_pos+(cam_pos-cam_look_at)*0.05f;
                        //cam_pos *= 1.1f;
                    V = glm::lookAt(cam_pos, cam_look_at, cam_up);
                }
            }
            else if(yOffset<0.0){
                //cam_pos /= 1.1f;
                cam_pos = cam_pos-(cam_pos-cam_look_at)*0.05f;
                V = glm::lookAt(cam_pos, cam_look_at, cam_up);
            }
        }
    }
    else if(light_mode == 2){
        direction = point_light_pos;
        if(xOffset > 0.0f){
            direction.x += x_sign * 0.1f;
            
            direction.y += y_sign * 0.1f;
        }
        else{
            if(x_sign < 0.0f){
                direction.x = (direction.x - x_sign * 0.1f > 0.0f) ? 0.0f : direction.x - x_sign*0.1f;
            }
            else{
                direction.x = (direction.x - x_sign * 0.1f < 0.0f) ? 0.0f : direction.x - x_sign*0.1f;
            }
            if(y_sign < 0.0f){
                direction.y = (direction.y - y_sign * 0.1f > 0.0f) ? 0.0f : direction.y - y_sign*0.1f;
            }
            else{
                direction.y = (direction.y - y_sign * 0.1f < 0.0f) ? 0.0f : direction.y - y_sign*0.1f;
            }
        }
        point_light_pos = glm::vec3(direction.x, direction.y, direction.z);
    }
    else if(light_mode == 3){
        direction = spot_light_pos;
        if(xOffset > 0.0f){
            direction.x += x_sign * 0.1f;
            
            direction.y += y_sign * 0.1f;
        }
        else{
            if(x_sign < 0.0f){
                direction.x = (direction.x - x_sign * 0.1f > 0.0f) ? 0.0f : direction.x - x_sign*0.1f;
            }
            else{
                direction.x = (direction.x - x_sign * 0.1f < 0.0f) ? 0.0f : direction.x - x_sign*0.1f;
            }
            if(y_sign < 0.0f){
                direction.y = (direction.y - y_sign * 0.1f > 0.0f) ? 0.0f : direction.y - y_sign*0.1f;
            }
            else{
                direction.y = (direction.y - y_sign * 0.1f < 0.0f) ? 0.0f : direction.y - y_sign*0.1f;
            }
        }
        spot_light_pos = glm::vec3(direction.x, direction.y, direction.z);
    }
    //spot wider or narrower
    
}
glm::vec3 Window::trackBallMapping(double x, double y)
{
    x*=2;
    y*=2;
    glm::vec3 v;
    float d;
    v.x = (2.0*x - (double)width) / (double)width;
    v.y = ((double)height - 2.0*y) / (double)height;
    v.z = 0.0f;
    d = glm::length(v);
    d = (d<1.0) ? d : 1.0;
    v.z = sqrtf(1.001 - d*d);
    v = glm::normalize(v); // Still need to normalize, since we only capped d, not v.
    return v;
}