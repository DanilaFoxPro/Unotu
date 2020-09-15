#include <entities\widgets\widget.h>

#include <stdio.h>//TODO: DEBUG
#include <algorithm>// std::find.

#include <utility\widget.h>
#include <utility\shortcuts.h>
#include <workers\widget_worker.h>

////::Base

////::Widget Base

//:: Operators.

bool operator<( const widget& a, const widget& b )
{
	return a.Layer < b.Layer;
}

bool operator>=( const widget& a, const widget& b )
{
	return !( a < b );
}

bool operator>( const widget& a, const widget& b )
{
	return a.Layer > b.Layer;
}
bool operator<=( const widget& a, const widget& b )
{
	return !( a > b );
}

//:: Constructors.

widget::widget( const widget& Widget )
{
	this->SimpleCopy( Widget );
	if( !Widget.Parent.expired() )
	{
		const std::string Message =
				std::string("Const reference copy constructor of ") +
				ClassName(*this) +
				" invoked with a " +
				ClassName(*this) +
				" that has a parent. ";
		throw std::invalid_argument( Message.c_str() );
	}
	if( Widget.Children.size() != 0 )
	{
		const std::string Message =
				std::string("Const reference copy constructor of ") +
				ClassName(*this) +
				" invoked with a " +
				ClassName(*this) +
				" that has a children.";
		throw std::invalid_argument( Message.c_str() );
	}
}

widget::widget( widget& Widget )
{
	this->SimpleCopy( Widget );
	
	if( Widget.Children.size() != 0 )
	{
		this->Children = Widget.Children;
		Widget.Children.clear();
	}
	
}

//:: Functions.

bool widget::DoesCollide( point Pos )
{
	point Position2 = SecondPosition( Position, Size );
	return DoesCollideRectangle( Pos, this->Position, Position2 );
}

bool widget::DoesHintCollide(point)
{
        return false;
}


void widget::ThrowEvent( std::shared_ptr<widget_event> Event, bool bSelfHandle )
{
        
        if( Event->Thrower.expired() ) {
                Event->Thrower = WidgetByPointer( this );
        }
        
        if( bSelfHandle ) {
                this->HandleEvent( Event );
        } else {
                std::shared_ptr<widget> strong_parent = Parent.lock();
                if( strong_parent )
                {
                        strong_parent->HandleEvent( Event );
                }
        }
                
	
}

void widget::HandleEvent( std::shared_ptr<widget_event> Event )
{
	this->OnEvent( Event );
	if( !Event->bHandled )
	{
		this->ThrowEvent( Event );
	}
}

// Helpers.

/**
  * @brief Copies basic parameters from the widget specified.
  * 
  * Used internally for copy constructors.
  * 
  */
void widget::SimpleCopy( const widget& Widget )
{
	// Position stuff.
	this->Position = Widget.Position;
	this->Size = Widget.Size;
	this->Layer = Widget.Layer;
		
	// Collision.
	this->bSolid = Widget.bSolid; // Solid -- consume input.
	this->bMouseOver = Widget.bMouseOver;
	
	// Input.
	
	this->bKeyboardFocusable = Widget.bKeyboardFocusable;
	this->bLoseFocusOnESC = Widget.bLoseFocusOnESC;
	this->bKeyboardFocused = Widget.bKeyboardFocused;
	
	// Other.
	this->bValidateOnRefresh = Widget.bValidateOnRefresh;
	this->bInvalidateOnMousePass = Widget.bInvalidateOnMousePass;
	this->bInvalidateOnKeyboardFocus = Widget.bInvalidateOnKeyboardFocus;
	
	//
	
	this->ValidityState = Widget.ValidityState;
	
}

void widget::SetSecondPosition( const point& position2 )
{
	this->Size = AreaSize( Position, position2 );
}

void widget::SetParent( std::weak_ptr<widget> parent_ )
{
        const std::shared_ptr<widget> Old = this->Parent.lock();
	if( !parent_.expired() )
	{
		this->Parent = parent_;
                
                if( Old ) {
                        Old->Invalidate( ValidityState::ChildrenUpdated );
                }
	}
}

bool widget::SetParent( widget* const parent_ )
{
	std::weak_ptr<widget> pointer = WidgetByPointer( parent_ );
	this->SetParent( pointer );
	return !pointer.expired();
}

std::shared_ptr<widget> widget::AddChild( std::shared_ptr<widget> Child )
{
	if( Child ) {
		widget* Parent = Child->Parent.lock().get();
		if( Parent && Parent != this ) {
                        Parent->RemoveChild( Child.get() );
		}
		
		Child->SetParent( this ); // Also invalidates this widget. (ChildrenUpdated.)
		
		if( std::find( this->Children.begin(), this->Children.end(), Child ) != this->Children.end() ) {
                        return Child;
                }
		this->Children.push_back( Child );
        
                if( this->bConstructed && this->Children.size() != 0 ) {
                        RecursivePostConstruct( this->Children );
                }
                
		return Child;
	} else {
		return std::shared_ptr<widget>();
	}
}
std::shared_ptr<widget> widget::AddChild( widget* Child )
{
	return this->AddChild( std::shared_ptr<widget>( Child ) );
}

void widget::RemoveChild( widget* Child )
{
	for( std::size_t i = 0; i < this->Children.size(); i++ )
	{
		if( this->Children[i].get() == Child )
		{
			this->RemoveChild( i );
                        this->Invalidate( ValidityState::ChildrenUpdated );
			return;
		}
	}
}

void widget::RemoveChild( const std::size_t& Index )
{
	const widget* Child = this->Children[Index].get();
	if( Child )
	{
		Child->Parent.lock() = nullptr;
	}
	this->Children.erase( this->Children.begin()+Index );
        this->Invalidate( ValidityState::ChildrenUpdated );
}

void widget::ClearChildren()
{
        if( this->Children.size() != 0 ) {
                this->Invalidate( ValidityState::ChildrenUpdated );
        }
        this->Children.clear();
}


void widget::Invalidate( ValidityState_t Reason )
{
    this->ValidityState |= Reason;
}
