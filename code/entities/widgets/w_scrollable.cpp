#include <entities\widgets\w_scrollable.h>

        //:: Constructors.

void w_scrollable::PostConstruct()
{
        this->Buffer = this->AddChild( new w_buffer( this->Position, this->Size ) );
}

        //:: Helper functions.
        
void w_scrollable::AddScrollableChild( std::shared_ptr<widget> Child )
{
        this->Buffer->AddChild( Child );
        this->Buffer->Invalidate( ValidityState::ParametersUpdated );
}
