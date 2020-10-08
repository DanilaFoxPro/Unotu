#include <entities\widgets\geometry\color.h>

#include <entities\ent_window.h>
#include <entities\ent_opengl.h>

//:: Geometry
color_geometry::color_geometry( GLenum usage_ )
{
	this->Usage = usage_;
        this->EnableBlend = false;
}

void color_geometry::Update()
{
	
	if( !this->ValidBuffers )
		this->CreateBuffers();
	
	//
	
	glBindVertexArray( this->VAO );
		
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size()*sizeof(colored_vertex), (void*)&Vertices[0], this->Usage);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size()*sizeof(unsigned int), (void*)&Indices[0], this->Usage);
		
		//Position stuff.
	    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	    glEnableVertexAttribArray(0);
	    
	    //Color stuff.
	    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2*sizeof(float)));
	    glEnableVertexAttribArray(1);
	
	glBindBuffer( GL_ARRAY_BUFFER, 0 ); // Just to be safe.
		
}

void color_geometry::Draw()
{
	
	if( this->EnableBlend )
	{
		glEnable( GL_BLEND );
	}
	
	glUseProgram( the_opengl.ColorShader );
	glBindVertexArray( this->VAO );
	glDrawElements(GL_TRIANGLES, this->Indices.size(), GL_UNSIGNED_INT, 0);
	
	if( this->EnableBlend )
	{
		glDisable( GL_BLEND );
	}
	
}

//--Shapes.

void color_geometry::Clear()
{
	Vertices.clear();
	Indices.clear();
	this->EnableBlend = false;
}

void color_geometry::AddRectangle( const colored_rectangle& rect )
{
	
	std::size_t vertex_offset = this->Vertices.size();
	std::size_t index_offset = this->Indices.size();
	
	Vertices.resize( Vertices.size()+4 );
	Indices.resize( Indices.size()+6 );
	
	Indices[ index_offset++ ] = vertex_offset+0;
	Indices[ index_offset++ ] = vertex_offset+1;
	Indices[ index_offset++ ] = vertex_offset+2;
	
	Indices[ index_offset++ ] = vertex_offset+2;
	Indices[ index_offset++ ] = vertex_offset+1;
	Indices[ index_offset++ ] = vertex_offset+3;
	
	Vertices[ vertex_offset++ ] = { rect.x1, rect.y1, rect.color }; // Top-left.
	Vertices[ vertex_offset++ ] = { rect.x2, rect.y1, rect.color }; // Top-right.
	Vertices[ vertex_offset++ ] = { rect.x1, rect.y2, rect.color }; // Bottom-left.
	Vertices[ vertex_offset++ ] = { rect.x2, rect.y2, rect.color }; // Bottom-right.
	
// 	if( rect.color.alpha < 1.0f )
// 	{
// 		this->EnableBlend = true;
// 	}

        this->EnableBlend = true;
	
}

/** @brief Adds an outline with specified thickness and lets you adjust each bar's color.
  * @note  Allows for negative thickness. */
void color_geometry::AddOutline( const rectangle& Bounds, int Thickness, rgba Left, rgba Right, rgba Top, rgba Bottom )
{
	
	const float XThickness = pixel( Thickness ).xratio();
	const float YThickness = pixel( Thickness ).yratio();
        
        //
        
        if( Thickness < 0 ) {
                const rectangle NewBounds = rectangle(
                        Bounds.x1 + XThickness,
                        Bounds.y1 - YThickness,
                        Bounds.x2 - XThickness,
                        Bounds.y2 + YThickness
                );
                return this->AddOutline( NewBounds, -Thickness, Left, Right, Top, Bottom );
        }
        
        //
        
        std::size_t VertexOffset = this->Vertices.size();
	std::size_t IndexOffset  = this->Indices.size();
        
        this->Vertices.resize( VertexOffset+4*4 );
        this->Indices.resize ( IndexOffset +6*4 );
        
        // Corners.
        const vertex TopLeft     = { Bounds.x1, Bounds.y1 };
	const vertex TopRight    = { Bounds.x2, Bounds.y1 };
	const vertex BottomLeft  = { Bounds.x1, Bounds.y2 };
	const vertex BottomRight = { Bounds.x2, Bounds.y2 };
        
        // Inner corners.
        const vertex InTopLeft     = TopLeft     + vertex( XThickness,  -YThickness );
	const vertex InTopRight    = TopRight    + vertex( -XThickness, -YThickness );
	const vertex InBottomLeft  = BottomLeft  + vertex( XThickness,   YThickness );
	const vertex InBottomRight = BottomRight + vertex( -XThickness,  YThickness );
        
                //:: Left.
        
        Indices[ IndexOffset++ ] = VertexOffset;
        Indices[ IndexOffset++ ] = VertexOffset+1;
        Indices[ IndexOffset++ ] = VertexOffset+2;
        
        Indices[ IndexOffset++ ] = VertexOffset;
        Indices[ IndexOffset++ ] = VertexOffset+2;
        Indices[ IndexOffset++ ] = VertexOffset+3;
        
        Vertices[ VertexOffset++ ] = colored_vertex( TopLeft,      Left );
        Vertices[ VertexOffset++ ] = colored_vertex( InTopLeft,    Left );
        Vertices[ VertexOffset++ ] = colored_vertex( InBottomLeft, Left );
        Vertices[ VertexOffset++ ] = colored_vertex( BottomLeft,   Left );
        
                //:: Right.
        
        Indices[ IndexOffset++ ] = VertexOffset;
        Indices[ IndexOffset++ ] = VertexOffset+1;
        Indices[ IndexOffset++ ] = VertexOffset+2;
        
        Indices[ IndexOffset++ ] = VertexOffset;
        Indices[ IndexOffset++ ] = VertexOffset+2;
        Indices[ IndexOffset++ ] = VertexOffset+3;
        
        Vertices[ VertexOffset++ ] = colored_vertex( InTopRight,    Right );
        Vertices[ VertexOffset++ ] = colored_vertex( TopRight,      Right );
        Vertices[ VertexOffset++ ] = colored_vertex( BottomRight,   Right );
        Vertices[ VertexOffset++ ] = colored_vertex( InBottomRight,    Right );
        
                //:: Top.
        
        Indices[ IndexOffset++ ] = VertexOffset;
        Indices[ IndexOffset++ ] = VertexOffset+1;
        Indices[ IndexOffset++ ] = VertexOffset+2;
        
        Indices[ IndexOffset++ ] = VertexOffset;
        Indices[ IndexOffset++ ] = VertexOffset+2;
        Indices[ IndexOffset++ ] = VertexOffset+3;
        
        Vertices[ VertexOffset++ ] = colored_vertex( TopLeft,    Top );
        Vertices[ VertexOffset++ ] = colored_vertex( TopRight,   Top );
        Vertices[ VertexOffset++ ] = colored_vertex( InTopRight, Top );
        Vertices[ VertexOffset++ ] = colored_vertex( InTopLeft,  Top );
        
                //:: Bottom.
        
        Indices[ IndexOffset++ ] = VertexOffset;
        Indices[ IndexOffset++ ] = VertexOffset+1;
        Indices[ IndexOffset++ ] = VertexOffset+2;
        
        Indices[ IndexOffset++ ] = VertexOffset;
        Indices[ IndexOffset++ ] = VertexOffset+2;
        Indices[ IndexOffset++ ] = VertexOffset+3;
        
        Vertices[ VertexOffset++ ] = colored_vertex( InBottomLeft,  Bottom );
        Vertices[ VertexOffset++ ] = colored_vertex( InBottomRight, Bottom );
        Vertices[ VertexOffset++ ] = colored_vertex( BottomRight,   Bottom );
        Vertices[ VertexOffset++ ] = colored_vertex( BottomLeft,    Bottom );
        
                //:: Blend.
        
        if(
                   Left.alpha < 1.0f
                || Right.alpha < 1.0f
                || Top.alpha < 1.0f
                || Bottom.alpha < 1.0f
        ) {
                this->EnableBlend = true;
        }
        
}


void color_geometry::AddOutline( const colored_rectangle& Area, int Thickness )
{
	
	ent_window& the_window = TheWindowManager.Cur();
	
	const float XThickness = pixel( Thickness ).ratio( the_window.x );
	const float YThickness = pixel( Thickness ).ratio( the_window.y );
	
	if( Thickness < 0 )
	{
		// Makes sure negative thickness is handled properly.
		// Keep in mind that x_thickness and y_thickness are negative at this point.
		const colored_rectangle new_area =
		          colored_rectangle
		(
			             Area.x1+XThickness,
			             Area.y1-YThickness,
			             Area.x2-XThickness,
			             Area.y2+YThickness,
			             Area.color
		);
		return this->AddOutline( new_area, -Thickness );
	}
	
	//Top bar.
	this->AddRectangle
	(
		      colored_rectangle
		(
			         Area.x1, Area.y1,
			         Area.x2, Area.y1-YThickness,
			         Area.color
		)
	);
	
	//Bottom bar.
	this->AddRectangle
	(
		      colored_rectangle
		(
			         Area.x1, Area.y2+YThickness,
			         Area.x2, Area.y2,
			         Area.color
		)
	);
	
	//Left bar.
	this->AddRectangle
	(
		      colored_rectangle
		(
			         Area.x1, Area.y1-YThickness,
			         Area.x1+XThickness, Area.y2+YThickness,
			         Area.color
		)
	);
	
	//Right bar.
	this->AddRectangle
	(
		      colored_rectangle
		(
			         Area.x2-XThickness, Area.y1-YThickness,
			         Area.x2, Area.y2+YThickness,
			         Area.color
		)
	);
	
	if( Area.color.alpha < 1.0f )
	{
		this->EnableBlend = true;
	}
	
}
