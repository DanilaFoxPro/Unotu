#ifndef CODE_ENTITIES_WIDGETS_W_MINICARD_H_
#define CODE_ENTITIES_WIDGETS_W_MINICARD_H_

#include <entities\widgets\widget.h>
#include <entities\widgets\w_text.h>
#include <entities\widgets\w_textbox.h>
#include <utility\classes\card.h>

struct w_minicard: public widget {
	
	//:: Constructors. / Destructor.
	
	w_minicard
	(
		const point& = point( 0, 0 ),
		const std::string& = "BLANK",
		const std::string& = "BLANK",
		const point& = point( 160, 80 )
	);
        w_minicard
        (
                const card& Card,
                const point& = point( 0.0f, 1.0f ),
                const point& = point( 160, 80 )
        );
        w_minicard
        (
                const indexed_card& Card,
                const point& = point( 0.0f, 1.0f ),
                const point& = point( 160, 80 )
        );
	void PostConstruct();
	virtual ~w_minicard();
        void CommonSetup();
	
	//:: Data.
	
        int CardID = -1;
        
	std::shared_ptr<w_text> TitleText;
	std::shared_ptr<w_textbox> ContentText;
	
	color_geometry ColorGeometry;
	
	//:: Overrides.
	
	virtual void OnTick();
	virtual void OnRefresh( ValidityState_t );
	virtual void OnDraw();
        
        virtual void OnMousePressed( int );
        
        //:: Helpers.
        
        void SetTitle( const std::string& );
        void SetContent( const std::string& );
	
};

#endif /* CODE_ENTITIES_WIDGETS_W_MINICARD_H_ */
