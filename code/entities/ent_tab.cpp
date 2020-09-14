#include <entities/ent_tab.h>

std::weak_ptr<widget> ent_tab::AddWidget( widget* const widget_ )
{
        const std::shared_ptr<widget> SharedPointer =  std::shared_ptr<widget>( widget_ );
        this->Widgets.push_back( SharedPointer );;
        widget_->bConstructed = true;
        widget_->PostConstruct();
        return std::weak_ptr<widget>( SharedPointer );
}

void ent_tab::SwitchTab( w_tab* Tab )
{
        if( this->PendingTab ){
                delete this->PendingTab;
        }
        
        this->PendingTab = Tab;
        
// 	this->Widgets.clear();
// 	this->AddWidget( Tab );
}

std::shared_ptr<widget>& ent_tab::operator[]( std::size_t index )
{
	return this->Widgets[index];
}
