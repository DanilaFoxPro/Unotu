#include <utility\widget.h>

#include <utility\shortcuts.h>

        //:: Shortcuts.

/// @brief Given position and size, calculate second position.
point SecondPosition( const point& Position, const point& Size )
{
        return
        point
        (
                Position.x + Size.x,
                Position.y - Size.y
        );
}

/// @brief Given position and size, calculate second position.
fpoint SecondPosition( const fpoint Position, const fpoint Size )
{
        return
        fpoint
        (
                Position.x + Size.x,
                Position.y - Size.y
        );
}


point AreaSize( const point& begin, const point& end )
{
// FIXME: Seems broken.
        return
        point
        (
                end.x - begin.x,
                begin.y - end.y
        );
}

/**
*  @brief  Return what the content area would be depending on outline thickness.
*  
*  Returns position and position2 as a pair.
*  
* */
std::pair< point, point > OutlineAdjustedArea
(
        const point& Position,
        const point& Position2,
        const int& OutlineThickness
)
{
        
        const point PointOutlineThickness = point( pixel( OutlineThickness ) );
        return OutlineAdjustedArea( Position, Position2, PointOutlineThickness );
        
}

/**
*  @brief  Return what the content area would be depending on outline thickness.
*  
*  Returns position and position2 as a pair.
*  
* */
std::pair< point, point > OutlineAdjustedArea
(
        const point& Position,
        const point& Position2,
        const point& OutlineThickness
)
{
        
        const point Offset = SecondPosition( point(0), OutlineThickness );
        std::pair< point, point > oArea = std::make_pair( Position, Position2 );
        
        if( OutlineThickness.x.xratio() > pixel(0).xratio() )
        {
                oArea.first.x += Offset.x;
                oArea.second.x -= Offset.x;
        }
        
        if( OutlineThickness.y.yratio() > pixel(0).yratio() )
        {
                oArea.first.y += Offset.y;
                oArea.second.y -= Offset.y;
        }
        
        return oArea;
        
}

/**
*  @brief  Apply padding to an area, supports negative values.
*  
*  Returns position and position2 as a pair.
*  
* */
std::pair< point, point > ApplyPadding( const point& Position, const point& Position2, const point& Padding )
{
        return std::make_pair
                        (
                                SecondPosition( Position, Padding ),
                                SecondPosition( Position2, -Padding )
                        );
        
}

fpoint Localize( const fpoint& Origin, const fpoint& Position )
{
        return Position-Origin;
}


        //:: Appearance.

void DrawWidgets( const std::vector<std::shared_ptr<widget>>& Widgets )
{
        for( std::size_t i = 0; i < Widgets.size(); i++ ) {
                widget* current = Widgets[i].get();
                if(current) {
                        current->OnDraw();
                        if( current->bDrawRecursive ) {
                                DrawWidgets( current->Children );
                        }
                }
                
        }// For each widget.
}

void SynchronizeButtons( std::vector<std::shared_ptr<w_button>> Buttons )
{
        int MinimumFontSize = INT_MAX;
        
        for( std::size_t i = 0; i < Buttons.size(); i++ ) {
                if( Buttons[i]->FontSize < MinimumFontSize )
                {
                        MinimumFontSize = Buttons[i]->FontSize;
                }
        }
        
        for( std::size_t i = 0; i < Buttons.size(); i++ ) {
                Buttons[i]->FontSize = MinimumFontSize;
                Buttons[i]->bOverrideSize        = true;
                Buttons[i]->bOverrideSizeFragile = true;
                Buttons[i]->Invalidate( ValidityState::ParametersUpdated );
        }
                
}

        //:: Search.

/**
*  @brief  Search for widget pointer in shared_ptr vector.
*  
*  Recursively.
* 
* */
std::weak_ptr<widget> WidgetByPointer( const std::vector< std::shared_ptr<widget> >& Widgets, widget* const Widget )
{
        
        for( std::size_t i = 0; i < Widgets.size(); i++ )
        {
                widget* const Current = Widgets[i].get();  
                if( Current == Widget )
                {
                        return std::weak_ptr<widget>( Widgets[i] );
                }
                else
                {
                        // Recursively check children.
                        std::weak_ptr<widget> MatchingChild = WidgetByPointer( Current->Children, Widget );
                        if( !MatchingChild.expired() )
                        {
                                return MatchingChild;
                        }
                }
        }
        return std::weak_ptr<widget>();
}

bool ContainsPointer( const std::vector<std::weak_ptr<widget> >& Widgets, const std::shared_ptr<widget>& Widget )
{
        for( std::weak_ptr<widget> Cur : Widgets ) {
                const std::shared_ptr<widget> Locked = Cur.lock();
                if( Locked == Widget ) {
                        return true;
                }
        }
        return false;
}


/**
*  @brief  Search for widget pointer in widget pointer vector.
* 
*  @note Non-recursive, for recursive version see ContainsPointerRecursive().
* 
* */
bool ContainsPointer( const std::vector< widget* >& Widgets, widget* const Widget )
{
        
        for( std::size_t i = 0; i < Widgets.size(); i++ )
        {
                widget* const Current = Widgets[i];  
                if( Current == Widget ) {
                        return true;
                }
        }
        return false;
}

/**
*  @brief  Search for widget pointer in widget pointer vector recursively.
* 
* */
bool ContainsPointerRecursive( const std::vector< widget* >& Widgets, widget* const Widget )
{
        for( std::size_t i = 0; i < Widgets.size(); i++ )
        {
                widget* const Current = Widgets[i];  
                if( Current == Widget ) {
                        return true;
                } else if ( Current
                                && ContainsPointerRecursive( ResolveShared( Current->Children ), Widget ) ) {
                        return true;
                }
        }
        return false;
}

        //:: Collision and input.

std::vector< std::weak_ptr<widget> > CollisionTrace( point Position, const std::vector< std::shared_ptr<widget> >& Widgets )
{
        std::vector< std::weak_ptr<widget> > Output;
        
        for( std::size_t i = Widgets.size()-1; i != std::size_t(-1); i-- ) {
                std::shared_ptr<widget> const Current = Widgets[i];
                
                if( !Current ) {
                        printf( "%s()::Null pointer at '%i'.\n", __func__, (int)i );
                        continue;
                }
                
                const bool bCollides = Current->DoesCollide( Position );
                if( bCollides ) {
                        Output.push_back( std::weak_ptr<widget>(Current) );
                        
                        Output += CollisionTrace( Position, Current->Children );
                        
                        if( Current->bSolid )
                                break;
                        
                }
                
        }
        
        return Output;
}

std::shared_ptr<widget> HintCollisionTrace( point Position, const std::vector<std::shared_ptr<widget> >& Widgets )
{
        for( std::size_t i = Widgets.size()-1; i != std::size_t(-1); i-- ) {
                const std::shared_ptr<widget> Current = Widgets[i];
                
                if( !Current ) {
                        printf( "%s()::Null pointer at '%i'.\n", __func__, (int)i );
                        continue;
                }
                
                const std::shared_ptr<widget> Result = HintCollisionTrace( Position, Current->Children );
                if( Result ) {
                        return Result;
                } else if( Current->DoesHintCollide( Position ) ) {
                        return Current;
                }
        }
        return std::shared_ptr<widget>();
}

bool DoesCollideRectangle( point Position, point XY1, point XY2 )
{
        return
        (
                XY1.x.xratio() < Position.x.xratio() &&
                XY1.y.yratio() > Position.y.yratio() &&
                XY2.x.xratio() > Position.x.xratio() &&
                XY2.y.yratio() < Position.y.yratio()
        );
}


/**
* @brief Update widget state and fire events related to mouse-over.
* 
* @param Position Where the mouse is currently.
* @param Widgets Widgets to operate upon.
* @param Colliding Which widgets collide with the mouse.
* 
* @see MousePosition()
* @see mouseMoveTrace()
*/
void UpdateMouseOverState
(
        const point& Position,
        const std::vector<std::shared_ptr<widget>>& Widgets,
        const std::vector<std::weak_ptr<widget>>& Colliding
)
{

        for( std::size_t i = 0; i < Widgets.size(); i++ )
        {
                std::shared_ptr<widget> const Current = Widgets[i];
                
                if( !Current )
                        continue;
                
                // Collides.
                if( ContainsPointer( Colliding, Current ) ) {
                        // Wasn't colliding before.
                        if( !Current->bMouseOver ) {
                                Current->bMouseOver = true;
                                Current->OnMouseEnter();
                                if( Current->bInvalidateOnMousePass )
                                {
                                        Current->Invalidate( ValidityState::Mouse );
                                }
                        }
                        if( Current->bInvalidateOnMouseOver ) {
                                Current->Invalidate( ValidityState::Mouse );
                        }
                        Current->OnMouseOver( Position );
                // Doesn't collide, but was colliding before.
                } else if( Current->bMouseOver ) {
                        Current->bMouseOver = false;
                        Current->OnMouseLeave();
                        if( Current->bInvalidateOnMousePass )
                        {
                                Current->Invalidate( ValidityState::Mouse );
                        }
                }
                // Recursively do the same to children.
                UpdateMouseOverState( Position, Current->Children, Colliding );
        }
}

        //:: Validation.

void InvalidateWidgets( const std::vector< std::shared_ptr<widget> >& Widgets, ValidityState_t Reason )
{
for( std::size_t i = 0; i < Widgets.size(); i++ )
{
        const std::shared_ptr<widget> Current = Widgets[i];
        
        if( Current )
        {
        Current->Invalidate( Reason );
        
        if( Current->bInvalidateChildren && Current->Children.size() != 0 )
        {
                InvalidateWidgets( Current->Children, Reason );
        }
        }
        
}
}

/**
* @brief Validates supplied widgets recursively.
* 
* @note Sorts children if any of them were invalid.
*       Same with initially supplied batch.
*/
bool RefreshWidgets( std::vector<std::shared_ptr<widget>>& Widgets )
{

        bool AnyInvalid = false;

        for( std::size_t i = 0; i < Widgets.size(); i++ ) {
                widget* Current = Widgets[i].get();
                
                if( Current == nullptr ) {
                        printf( "%s(): Null widget at %i!\n", __func__, (int)i );
                        continue;
                }
                
                const ValidityState_t CurValidity = Current->ValidityState;
                
                if( CurValidity != ValidityState::Valid ) {
                        Current->OnRefresh( CurValidity );
                        
                        if( Current->bValidateOnRefresh ) {
                                Current->ValidityState = ValidityState::Valid;
                        }
                        
                        AnyInvalid = true;
                        
                }
                
                bool ChildrenRefreshed = false;
                if( Current->bRefreshRecurisve
                && Current->Children.size() != 0 ) {
                        ChildrenRefreshed = RefreshWidgets( Current->Children );
                        AnyInvalid |= ChildrenRefreshed;
                }
                
                Current->OnPostRefresh( CurValidity, ChildrenRefreshed );
                
        }

        if( AnyInvalid ) {
                SortWidgets( Widgets );
        }

        return AnyInvalid;
        
}

        //:: Tick.

void TickWidgets( const std::vector< std::shared_ptr<widget> >& Widgets )
{
        for( std::size_t i = 0; i < Widgets.size(); i++ ) {
                widget* Current = Widgets[i].get();
                
                if( Current == nullptr ) {
                        continue;
                }
                
                Current->OnTick();
                if( Current->bTickRecursive ) {
                        TickWidgets( Current->Children );
                }
                
        }
}

        //:: Debug.

void PrintWidgetHierarchy( const std::vector<std::shared_ptr<widget> >& Widgets, unsigned int Indent )
{
        for( std::size_t i = 0; i < Widgets.size(); i++ ) {
                std::shared_ptr<widget> Current = Widgets[i];
                if( Current ) {
                        const std::string String = std::string( Indent, ' ' ) + ClassName( *Current );
                        printf( "%s\n", String.c_str() );
                        PrintWidgetHierarchy( Current->Children, Indent+2 ); 
                } else {
                        const std::string String = std::string( Indent, ' ' ) + "null";
                        printf( "%s\n", String.c_str() );
                }
        }
}

        //:: Other.

void SortWidgets( std::vector<std::shared_ptr<widget> >& Widgets )
{
        
        for( std::size_t x = 0; x < Widgets.size(); x++ )
        {
                std::shared_ptr<widget>& curx = Widgets[x];
                
                for( std::size_t y = x+1; y < Widgets.size(); y++ )
                {
                        std::shared_ptr<widget>& cury = Widgets[y];
                        
                        if( *curx > *cury && curx && cury )
                                curx.swap( cury );
                        
                }
                
        }
}

void RecursivePostConstruct( const std::vector< std::shared_ptr<widget> >& Widgets )
{
        for( std::size_t i = 0; i < Widgets.size(); i++ )
        {
                const std::shared_ptr<widget> Current = Widgets[i];
                if( !Current  )
                {
                        continue;
                }
                
                if( !Current->bConstructed )
                {
                        Current->PostConstruct();
                        Current->bConstructed = true;
                }
                
                if( Current->Children.size() != 0 )
                {
                        RecursivePostConstruct( Current->Children );
                }
        }
}
