#include <entities\widgets\w_rectangle.h>

#include <entities\ent_window.h>
#include <entities\ent_opengl.h>

#include <workers\widget_worker.h>
#include <utility\widget.h>

#include <stdio.h>//TODO: DEBUG
void w_rectangle::OnRefresh( ValidityState_t )
{
	ent_window& the_window = TheWindowManager.Cur();
	
	Geometry.Clear();
	
	point position2 = SecondPosition( Position, Size );
	
	   colored_rectangle geometry;
	geometry.x1 = this->Position.x.ratio( the_window.x );
	geometry.y1 = this->Position.y.ratio( the_window.y );
	geometry.x2 = position2.x.ratio( the_window.x );
	geometry.y2 = position2.y.ratio( the_window.y );
	
	geometry.color = this->color;
	
	Geometry.AddRectangle( geometry );
	Geometry.Update();
		
}

void w_rectangle::OnDraw()
{
	
    this->Geometry.Draw();
	
}
