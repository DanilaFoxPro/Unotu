#ifndef CODE_ENTITIES_WIDGETS_W_MINICARD_H_
#define CODE_ENTITIES_WIDGETS_W_MINICARD_H_

#include <unotu\utility\deps\unotui_includes.h>

#include <unotu\utility\classes\card.h>

namespace unotu {

struct w_minicard: public unotui::widget {
	
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
	virtual ~w_minicard() {};
        void CommonSetup();
	
	//:: Data.
	
        int CardID = -1;
        
	std::shared_ptr<unotui::w_text> TitleText;
	std::shared_ptr<unotui::w_textbox> ContentText;
	
	unotui::color_geometry gColor;
        
        //:: Options.
        
        bool bEnforceMinimumSize = true;
	
	//:: Overrides.
	
	virtual void OnTick();
	virtual void OnRefresh( unotui::ValidityState_t );
	virtual void OnDraw();
        
        virtual void OnMousePressed( const int );
        
        //:: Helpers.
        
        void SetTitle( const std::string& );
        void SetContent( const std::string& );
	
};

} // namespace unotu

#endif /* CODE_ENTITIES_WIDGETS_W_MINICARD_H_ */
