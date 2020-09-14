#ifndef __WIDGET_EVENT_H_
#define __WIDGET_EVENT_H_

#include <string>
#include <memory>

struct widget;
struct widget_event;

#include <entities\chat\ent_chatconnection.h>

struct widget_event
{
//:: Data.
        bool bHandled = false;
        
        std::weak_ptr<widget> Thrower;
//:: Constructors.
        widget_event() = default;
        virtual ~widget_event() = default;
//:: Functions.
        void Handle()
        {
                bHandled = true;
        }
};

struct we_click : public widget_event
{
        int ActionID;
        std::string ActionName;
        
        int Button;
        
        we_click( int, const std::string&, int );
};

struct we_scroll : public widget_event {};

struct we_scrollsetratio : public we_scroll
{
	float Ratio;
	
        we_scrollsetratio( float );
};

struct we_scrolllines : public we_scroll
{
        float Lines;
        
        we_scrolllines( float );
};

struct we_scrollpages : public we_scroll
{
        float Pages;
        
        we_scrollpages( float );
};

struct we_externalscroll : public we_scrollsetratio
{
        we_externalscroll( float Ratio ) : we_scrollsetratio( Ratio ) {};
};

struct we_textupdated : public widget_event
{
        we_textupdated() = default;
        virtual ~we_textupdated() = default;
};

struct we_logupdated : public we_textupdated
{
        // I'll probably extract log stuff from ENTChatConnection to some log module later.
        ent_chatconnection* Log;
        we_logupdated( const ent_chatconnection* );
        virtual ~we_logupdated() = default;
};

struct we_textcommit : public we_textupdated
{
	std::string* Text;
	
	we_textcommit( std::string* const Text );
};

#endif

