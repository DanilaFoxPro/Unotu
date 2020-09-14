#ifndef __WIDGET_WORKER_H_
#define __WIDGET_WORKER_H_

#include <vector>

#include <entities\widgets\widget.h>
#include <entities\ent_window.h>

//:: Interface actions.

void ProcessWidgetInput();
void UpdateWidgetHints();
void UpdateWidgetHint( ent_window& );
void TickWidgets();
void RefreshWidgets();
void InvalidateAllWidgets( const std::size_t Window, const ValidityState_t Reason = ValidityState::Unspecified );

void SwitchTabs();

void DrawWidgets();
void DrawWidgets( const std::size_t& WindowIndex );

void SetKeyboardFocus( std::weak_ptr<widget>, ent_window& = TheWindowManager.Cur() );
void ResetKeyboardFocus( ent_window& = TheWindowManager.Cur() );
std::weak_ptr<widget> GetKeyboardFocus( ent_window& = TheWindowManager.Cur() );

//:: Search.

std::weak_ptr<widget> WidgetByPointer( widget* const );

//:: Collision and input.

std::vector< std::weak_ptr<widget> > CollisionTrace( point );
void mouseMoveTrace( point, const std::vector< std::weak_ptr<widget> >& );
void mouseButtonTrace( int Button, const std::vector< std::weak_ptr<widget> >&  );
void mouseButtonRelease( int Button );

void ScrollTrace( const std::vector< std::weak_ptr<widget> >& );

#endif
