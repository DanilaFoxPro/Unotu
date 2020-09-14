#ifndef __CALLBACKS_H_
#define __CALLBACKS_H_

#include <utility\deps\opengl_includes.h>

void CallbackKey(GLFWwindow* WindowReference, int Key, int Scancode, int Action, int Modifiers );
void CallbackCharacter(GLFWwindow* WindowReference, unsigned int Codepoint);
void CallbackScroll( GLFWwindow* WindowReference, double X, double Y );

void CallbackExit();

#endif
