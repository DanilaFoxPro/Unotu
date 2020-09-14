#ifndef __UNOTU_UTILITY_GENERAL_H_
#define __UNOTU_UTILITY_GENERAL_H_

#include <string>
#include <vector>

#include <utility\deps\opengl_includes.h>

#include "..\coordinates.h" // For coordinates. Come on. (Unit, point.)

        //:: Textures.
unsigned int loadTexture( const uint8_t* Data, int Length, GLint internal_format, GLenum format, const std::string& = "NO_NAME", bool mipmaps = true );

GLFWimage LoadGLFWImage( const uint8_t* Data, const std::size_t Length );

        //:: Shaders.

GLchar* loadShaderSource( std::string );
GLuint compileAndLinkShaders( GLchar*, GLchar*, std::string = "" );

        //:: Point

point Localize( const point& origin, const point& position );

#endif
