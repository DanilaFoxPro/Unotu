#ifndef __UNOTU_ENTITIES_WIDGETS_GEOMETRY_BASE_H_
#define __UNOTU_ENTITIES_WIDGETS_GEOMETRY_BASE_H_

#include <utility\deps\opengl_includes.h>

struct geometry
{
	unsigned int VAO, VBO, EBO;
	GLenum Usage = GL_STATIC_DRAW;
	bool ValidBuffers;
	bool EnableBlend;
	
	   geometry();
	virtual ~geometry();
	
	void CreateBuffers();
	void DestroyBuffers();
	
	virtual void Update(void) {};
	virtual void Draw(void) {};
	virtual void Clear(void) {};
};

#endif
