#include <entities\widgets\geometry\texture.h>

#include <entities\ent_window.h>
#include <entities\ent_opengl.h>

//:: Textured Geometry

texture_geometry::texture_geometry( GLenum usage_ )
{
	this->Usage = usage_;
	this->EnableBlend = true; // Because currently it's only used with font texture, which has alpha.
}

void texture_geometry::Update(void)
{
        
        if( !this->ValidBuffers )
                this->CreateBuffers();

        //

        glBindVertexArray( this->VAO );
                
                glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
                glBufferData(GL_ARRAY_BUFFER, Vertices.size()*sizeof(textured_vertex), (void*)&Vertices[0], this->Usage);
                
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size()*sizeof(unsigned int), (void*)&Indices[0], this->Usage);
                
        //Position stuff.
        glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0 );
        glEnableVertexAttribArray( 0 );

        //Texture coordinates stuff.
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)) );
        glEnableVertexAttribArray( 1 );
        
        glBindBuffer( GL_ARRAY_BUFFER, 0 ); // Just to be safe.
                
}

void texture_geometry::Draw( GLuint Texture )
{
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_2D, Texture );
        
        this->Draw();
        
}

void texture_geometry::Draw()
{
        if( this->EnableBlend ) {
                glEnable( GL_BLEND );
        }

        glUseProgram( the_opengl.TextureShader );
        glBindVertexArray( this->VAO );
        glDrawElements( GL_TRIANGLES, this->Indices.size(), GL_UNSIGNED_INT, 0 );

        if( this->EnableBlend ) {
                glDisable( GL_BLEND );
        }
}

//--Shapes.

void texture_geometry::Clear(void)
{
	Vertices.clear();
	Indices.clear();
}

void texture_geometry::AddRectangle( const textured_rectangle& rect )
{
	int vertex_offset = this->Vertices.size();
	int index_offset = this->Indices.size();
	
	Vertices.resize( Vertices.size()+4 );
	Indices.resize( Indices.size()+6 );
	
	Indices[index_offset++] = vertex_offset  ;
	Indices[index_offset++] = vertex_offset+1;
	Indices[index_offset++] = vertex_offset+2;
	
	Indices[index_offset++] = vertex_offset+2;
	Indices[index_offset++] = vertex_offset+1;
	Indices[index_offset++] = vertex_offset+3;
	
	Vertices[ vertex_offset++ ] = { rect.x1, rect.y1, rect.tc.x1, rect.tc.y1 }; // Top-left.
	Vertices[ vertex_offset++ ] = { rect.x2, rect.y1, rect.tc.x2, rect.tc.y1 }; // Top-right.
	Vertices[ vertex_offset++ ] = { rect.x1, rect.y2, rect.tc.x1, rect.tc.y2 }; // Bottom-left.
	Vertices[ vertex_offset++ ] = { rect.x2, rect.y2, rect.tc.x2, rect.tc.y2 }; // Bottom-right.
	
}
