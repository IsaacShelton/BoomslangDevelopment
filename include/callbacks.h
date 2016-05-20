
#ifndef CALLBACKS_H_INCLUDED
#define CALLBACKS_H_INCLUDED

#include <GLFW/glfw3.h>

void error_callback(int, const char*);
void key_callback(GLFWwindow*, int, int, int, int);
void character_callback(GLFWwindow*, unsigned int);
void mouse_button_callback(GLFWwindow*, int, int, int);
void mouse_scroll_callback(GLFWwindow*, double, double);

#endif // CALLBACKS_H_INCLUDED
