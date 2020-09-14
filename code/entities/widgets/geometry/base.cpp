#include <entities\widgets\geometry\base.h>

geometry::geometry()
{
        this->ValidBuffers = false;
        this->VAO = this->VBO = this->EBO = -1;
        
        this->EnableBlend = false;
}

void geometry::CreateBuffers()
{
        
        this->DestroyBuffers();
                
        glGenVertexArrays       ( 1, &this->VAO );
        glGenBuffers            ( 1, &this->VBO );
        glGenBuffers            ( 1, &this->EBO );
        
        //
        
        glBindVertexArray( this->VAO );
        
        glBindBuffer( GL_ARRAY_BUFFER, this->VBO );
        glBufferData( GL_ARRAY_BUFFER, 0, NULL, this->Usage );
        
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->EBO );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, 0, NULL, this->Usage );
        
        this->ValidBuffers = true;
        
}

void geometry::DestroyBuffers()
{
        if( !this->ValidBuffers ) {
                return void();
        }
        
        //
        
        glBindVertexArray	(  this->VAO );
        
                glDeleteBuffers	( 1, &this->VBO );
                glDeleteBuffers	( 1, &this->EBO );
        
        glDeleteVertexArrays( 1, &this->VAO );
        
}

geometry::~geometry()
{
        this->DestroyBuffers();
}
