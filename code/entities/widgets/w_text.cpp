#include "w_text.h"

#include <utility\text.h>
#include <entities\ent_opengl.h>
#include <entities\ent_window.h>

void w_text::OnRefresh( ValidityState_t )
{

        Size = point( (int)text.size()*font_size/2, font_size );

        gText.Clear();
        
        gText.AddText(
                this->text,
                this->font_size,
                this->Position,
                this->color
        );

        gText.Update();

}

void w_text::OnDraw()
{
	
	this->gText.Draw();
	
}
