#ifndef CODE_ENTITIES_WIDGETS_EDITABLETEXTBOX_H_
#define CODE_ENTITIES_WIDGETS_EDITABLETEXTBOX_H_

#include <entities\widgets\widget.h>
#include <entities\widgets\w_textscrollbox.h>
#include <entities\modules\m_caret.h>

struct w_editabletextbox : public w_textscrollbox, public m_caret
{
//:: Constructors.
	w_editabletextbox
	(
		const point& Position	= point( 0.0f, 1.0f ),
		const point& Size		= point( 1.0f, 1.0f ),
		
		const std::string&	Text		= "",
		const int&			FontSize	= 16,
		const rgba&			TextColor	= rgba( color::black, 1.0f ),
		
		const rgba& BackgroundColor   = rgba( color::gray*1.4f ), 
		const rgba& OutlineColor      = rgba( color::gray*0.5f ),
                const int&  OutlineThickness  = 2
	);
	virtual ~w_editabletextbox() = default; // Just to be safe.
	
//:: Overrides.
	virtual void OnTick();
	virtual void OnRefresh( ValidityState_t );
	virtual void OnDraw();
	
	virtual void OnKeyboardFocused();
	
	virtual void OnCharacterInput( const std::string& ) final;
	virtual void OnKeyInput( const int&, const int& ) final;
	
//:: Geometry.
	text_geometry gText; // Only for a caret. What a waste.
};

#endif
