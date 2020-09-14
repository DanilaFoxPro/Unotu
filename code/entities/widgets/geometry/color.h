#ifndef __UNOTU_ENTITIES_WIDGETS_GEOMETRY_COLOR_H_
#define __UNOTU_ENTITIES_WIDGETS_GEOMETRY_COLOR_H_

#include <entities\widgets\geometry\base.h>

#include <vector>

#include <types.h>


struct color_geometry : geometry
{
        std::vector<colored_vertex> Vertices;
        std::vector<unsigned int> Indices;
        
        color_geometry( GLenum usage_ = GL_STATIC_DRAW );
        
        virtual void Update();
        virtual void Draw();
        
        virtual void Clear();
        void AddRectangle( const colored_rectangle& );
        void AddOutline( const rectangle& Bounds, int Thickness, rgba Left, rgba Right, rgba Top, rgba Bottom );
        void AddOutline( const colored_rectangle&, int = 5);
};

#endif
