#ifndef __WIDGET_H_
#define __WIDGET_H_

//Standard.

#include <string>
#include <vector>
#include <memory>

//OpenGL.
#include <utility\deps\opengl_includes.h>

//Mine.

#include <events\widget_event.h>
#include <coordinates.h>
#include <types.h>

#include <entities\widgets\geometry\base.h>
#include <entities\widgets\geometry\color.h>
#include <entities\widgets\geometry\texture.h>
#include <entities\widgets\geometry\text.h>

//::Utility.

typedef uint8_t ValidityState_t;
namespace ValidityState
{
    enum
    {
        Valid              = ValidityState_t(0),
        
        Unspecified        = ValidityState_t(1),
        Resized            = ValidityState_t(2),
        ParametersUpdated  = ValidityState_t(4),
        Mouse              = ValidityState_t(8),
        ChildrenUpdated    = ValidityState_t(16),
        
        RefreshEverything  = ValidityState_t(-1),
    };
}// namespace RefreshReason

//::Base.

struct widget
{
//:: Data.

        // Position stuff.
        point Position = point( pixel(0), pixel(0) );
        point Size = point( pixel(64), pixel(64) );
        int Layer = 0;
        
        // Other stuff.

        std::weak_ptr<widget> Parent;
        std::vector<std::shared_ptr<widget>> Children;
        ValidityState_t ValidityState = ValidityState::RefreshEverything;

//:: State.

        // General.
        bool bConstructed = false; // Becomes true after 'PostConstruct()' is called.

        // Collision.
        bool bMouseOver = false;
        bool bHintFocused = false;

        // Input.

        bool bKeyboardFocused = false;

//:: Options.
                /** Consume input. */
        bool bSolid = true;
                /** Draw all the children after this widget is drawn. */
        bool bDrawRecursive = true;
                /** Refresh the children after this widget is refreshed. */
        bool bRefreshRecurisve = true;
                /** Tick the children after this widget is tick'ed. */
        bool bTickRecursive = true;
                /** Auto-validate widget after updating the geometry. */
        bool bValidateOnRefresh = true;
                /** Invalidate the widget when mouse enters or leaves collision. */
        bool bInvalidateOnMousePass = false;
                /** Invalidate the widget each frame the mouse hovers over it. */
        bool bInvalidateOnMouseOver = false;
                /** Invalidate the widget when it gains or losses keyboard focus. */
        bool bInvalidateOnKeyboardFocus = false;
                /** Invalidate children when invalidating via 'InvalidateWidgets()'. */
        bool bInvalidateChildren = true;
                /** Can accept keyboard focus. ( Via LMB. ) */
        bool bKeyboardFocusable = false;
                /** Would accept mouse scroll wheel scroll. */
        bool bAcceptExternalScroll = false;
                /** Lose keyboard focus when ESC is pressed. */
        bool bLoseFocusOnESC = true;

//:: Functions.

        widget() = default;
        widget( const widget& );
        widget( widget& );
        virtual void PostConstruct() {}; // Called after adding the widget to interface vector.
        virtual ~widget() = default;

                /** @brief Called when in need of a refresh.
                  * @note Parent is refreshed before its children. */
        virtual void OnRefresh( ValidityState_t ) {}
                /** @brief Called after all children have been refreshed.
                 *  @param ValidityState Validity state this widget had before refresh.
                 *  @param ChildrenRefreshed True if any of the widgets waren't valid this frame.
                  * @note Called even if this widget wasn't refreshed. */
        virtual void OnPostRefresh( ValidityState_t, bool ) {}
                /** @brief Called each time the widget is supposed to be drawn.
                  * @note Current window is the widget's window. */
        virtual void OnDraw() {}
                /** @brief Called each frame. */
        virtual void OnTick() {};
                /** @brief Returns true if the supplied point collides with the widget.
                 *         False otherwise. */
        virtual bool DoesCollide( point );
        virtual bool DoesHintCollide( point );

        //

        virtual void OnMousePressed( const int Button ) { (void)Button; };
        virtual void OnMouseReleased( const int Button, const bool bFocusingClick ) {(void)Button;(void)bFocusingClick;};
        virtual void OnMouseEnter() {};
        virtual void OnMouseLeave() {};
        virtual void OnMouseOver( point ) { };
        
        //
        
        virtual void OnHintEnter() {};
        virtual void OnHintLeave() {};
        
        //

        virtual void OnKeyboardFocused() {};
        virtual void OnLostKeyboardFocus() {};
        virtual void OnCharacterInput( const std::string& ) {};
        virtual void OnKeyInput( const int& Key, const int& Modifiers ) {};

        //

        void ThrowEvent( std::shared_ptr<widget_event> Event, bool SelfHandle = false );
        void HandleEvent( std::shared_ptr<widget_event> Event );
        virtual void OnEvent( std::shared_ptr<widget_event> Event ) {};

        // Helpers.

        void SimpleCopy( const widget& );

        void SetSecondPosition( const point& position2 );
        void SetParent( std::weak_ptr<widget> parent_ );
        bool SetParent( widget* parent_ );
        std::shared_ptr<widget> AddChild( std::shared_ptr<widget> );
        std::shared_ptr<widget> AddChild( widget* );
        template< class T >
        inline std::shared_ptr<T> AddChild( T* Widget )
        {
                return
                        std::static_pointer_cast<T>
                                (
                                        this->AddChild( std::shared_ptr<widget>( Widget ) )
                                );
        }
        void RemoveChild( widget* Child );
        void RemoveChild( const std::size_t& Index );
        void ClearChildren();

        //

        inline bool Valid() { return this->ValidityState == ValidityState::Valid; }
        void Invalidate( ValidityState_t = ValidityState::Unspecified );
	
};

//Those are for widget sorting.

bool operator<( const widget&, const widget& );
bool operator<=( const widget&, const widget& );

bool operator>( const widget&, const widget& );
bool operator>=( const widget&, const widget& );

#endif
