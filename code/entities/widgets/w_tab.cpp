#include <entities\widgets\w_tab.h>

w_tab::w_tab( const std::string& Title )
{
        this->Position	= point( 0.0f, 1.0f );
        this->Size	= point( 1.0f, 1.0f );

        this->Title = Title;
}
