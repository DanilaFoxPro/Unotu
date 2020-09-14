#ifndef __UNOTU_UTILITY_WIDGET_H_
#define __UNOTU_UTILITY_WIDGET_H_

#include <vector>
#include <memory>

#include <entities\widgets\widget.h>
#include <entities\widgets\w_button.h>

        //:: Shortcuts

point SecondPosition( const point& position, const point& size );
fpoint SecondPosition( const fpoint, const fpoint );
point AreaSize( const point& begin, const point& end );
std::pair< point, point > OutlineAdjustedArea( const point&, const point&, const int& );
std::pair< point, point > OutlineAdjustedArea( const point&, const point&, const point& );
std::pair< point, point > ApplyPadding( const point&, const point&, const point& );
fpoint Localize( const fpoint&, const fpoint& );

        //:: Appearance.

void DrawWidgets( const std::vector<std::shared_ptr<widget>>& Widgets );
/// Most likely won't be useful. TODO.
void SynchronizeButtons( std::vector<std::shared_ptr<w_button>> );

        //:: Search.

std::weak_ptr<widget> WidgetByPointer( const std::vector< std::shared_ptr<widget> >&, widget* const );
bool ContainsPointer( const std::vector<std::weak_ptr<widget>>&, const std::shared_ptr<widget>& );
bool ContainsPointer( const std::vector< widget* >&, widget* const );
bool ContainsPointerRecursive( const std::vector< widget* >&, widget* const );

        //:: Collision and input.

std::vector< std::weak_ptr<widget> > CollisionTrace( point Position, const std::vector< std::shared_ptr<widget> >& Widgets );
std::shared_ptr<widget> HintCollisionTrace( point Position, const std::vector< std::shared_ptr<widget> >& Widgets );

bool DoesCollideRectangle( point Position, point XY1, point XY2 );

void UpdateMouseOverState(
	const point&,
	const std::vector<std::shared_ptr<widget>>&,
	const std::vector<std::weak_ptr<widget>>&
);

        //:: Validation.

void InvalidateWidgets( const std::vector< std::shared_ptr<widget> >&, ValidityState_t = ValidityState::Unspecified );
bool RefreshWidgets( std::vector<std::shared_ptr<widget>>& Widgets );

        //:: Tick.

void TickWidgets( const std::vector< std::shared_ptr<widget> >& Widgets );

        //:: Debug

void PrintWidgetHierarchy( const std::vector< std::shared_ptr<widget> >& Widgets, unsigned int Indent = 0 );

        //:: Other.

void SortWidgets( std::vector<std::shared_ptr<widget>>& );
void RecursivePostConstruct( const std::vector< std::shared_ptr<widget> >& );

#endif
