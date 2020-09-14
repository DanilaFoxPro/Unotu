#ifndef __ENT_OPENGL_H_
#define __ENT_OPENGL_H_

#include <utility\deps\opengl_includes.h> // GLuint.
#include <vector>
#include <chrono>

#include "../types.h"

namespace chrono = std::chrono;

struct ent_opengl
{
	// Shaders.
	GLuint ColorShader;
	GLuint TextureShader;
	GLuint TextShader;
	
	// Textures.
	unsigned int FontTexture;
        
        // Other.
	chrono::microseconds IdealFramePeriod = chrono::microseconds(16666);
        
};

extern ent_opengl the_opengl;

#endif
