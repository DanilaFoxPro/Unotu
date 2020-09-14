#ifndef __W_TABWIDGET_H_
#define __W_TABWIDGET_H_

#include <entities\widgets\widget.h>

#include <utility\colors.h>

struct w_tab : widget
{
	w_tab( const std::string& = "" );
        virtual ~w_tab() = default;
        
	std::string Title;
        rgba Background = color::white;
};

#endif
