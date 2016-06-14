#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"

class Window
{
public:
    static glm::mat4 initial_toWorld;
    static GLuint cubemapTexture_window;
    static unsigned char *image_data;
    static int prev_light_mode;
    static int object_num;
    static float other_radius;
    static float spot_radius;
    static glm::vec3 spot_light_dir;
    static glm::vec3 spot_light_pos;
    static glm::vec3 point_light_pos;
    static glm::vec3 dir_light_dir;
    static int light_mode;
	static int width;
	static int height;
	static glm::mat4 P;
	static glm::mat4 V;
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);
    static void cursor_enter_callback(GLFWwindow* window, int enter);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
    static glm::vec3 trackBallMapping(double x, double y);
    static void OnMouseMove(double x, double y, int mode);
    static void init();
};

#endif
