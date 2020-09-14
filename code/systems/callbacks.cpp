#include "callbacks.h"

#include <stdio.h> //DEBUG

#include <entities/ent_window.h>


void CallbackKey(GLFWwindow* WindowReference, int Key, int /*Scancode*/, int Action, int Modifiers )
{
        // NOTE: Only handles 'OnKeyInput()' for now. No 'OnPressed()' and 'OnReleased()' here.
        
        // Possible actions: GLFW_PRESS, GLFW_RELEASE and GLFW_REPEAT.
        if( Action != GLFW_RELEASE ){
                ent_window& TheWindow = TheWindowManager[WindowReference];
                TheWindow.PendingKeys.WriteBuffer().push_back( key_item( Key, Modifiers ) );
        }
}


void CallbackCharacter(GLFWwindow* WindowReference, unsigned int Codepoint)
{
        ent_window& the_window = TheWindowManager[WindowReference];
        if( Codepoint < 128 ) {
                the_window.PendingInput.WriteBuffer() += (unsigned char)Codepoint;
        } else {
                printf( "%s() - Unicode: '%u'.\n", __func__, Codepoint );
        }
}

void CallbackScroll( GLFWwindow* WindowReference, double, double Y )
{
        ent_window& TheWindow = TheWindowManager[WindowReference];
        TheWindow.PendingScroll.WriteBuffer() += Y;
}


void CallbackExit()
{
        printf( "Terminate\n{\n" );
		
        printf( "  GLFW..\n" );
        glfwTerminate();
        
        // TODO: Saving should occur here.
        
        printf( "}\n" );
}
