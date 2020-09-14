#ifndef __ENT_WINDOW_H_
#define __ENT_WINDOW_H_

#include <entities\widgets\widget.h>
#include <utility\fpoint.h>
#include <utility\shortcuts.h>

#include <utility\deps\opengl_includes.h>

#include <atomic>
#include "ent_tab.h"

struct key_item
{
	key_item( int Key, int Modifier )
	{
		this->Key = Key;
		this->Modifier = Modifier;
	}
	int Key;
	int Modifier;
};

struct pending_window
{
};

struct theme
{
        rgba Text = color::black;
        rgba Primary = color::gray;
        rgba Primary2 = this->Primary*0.7f;
        rgba PrimaryLit = color::white * 0.6;
        rgba PrimaryBack = color::gray / 2.0f;
        rgba Background = rgb{ 0.25f, 0.25f, 0.25f };
        rgba Highlight = color::light_sky_blue * 0.7f;
        rgba Accent = color::yellow;
};

struct ent_window
{
	GLFWwindow *Reference;
	union { int Width ; int x; };
	union { int Height; int y; };
	
	  /** Useful for scaling text and widgets. */
	std::size_t ScaleReference = 1;
	  /** Pixel size as width and height ratio
	   *  of the window size. */
	fpoint PixelSize;
          /** Index of the currently rendered frame. */
        uint32_t CurrentFrame = 0;
        
	//:: Interface.
	ent_tab ActiveTab;
        
	//::Input.
	
	/**
	 * @brief Currently keyboard-focused widget.
	 * 
	 * You should modify this by hand, use 'SetKeyboardFocus()', 'ResetKeyboardFocus()'
	 * instead. There is also 'GetKeyboardFocus()' if you're feeling fancy.
	 * 
	 * */
	std::weak_ptr<widget> KeyboardFocus;
	//
	std::vector<bool> MouseButtonsState;
	//
	double_buffer<std::string> PendingInput;
	double_buffer<std::vector<key_item>> PendingKeys;
        double_buffer<double> PendingScroll;
	
        //
        
        void SwitchTab( w_tab* );
        
        bool Visible();
    
};

struct ent_window_manager
{
        
        /** @brief A window that is used to setup GLEW and for context sharing. */
        GLFWwindow* GhostWindow = nullptr;
        
        theme Theme;
        
        std::vector<pending_window> PendingWindows;
	std::vector<ent_window> Windows;
        std::vector<GLFWimage> IconLayers;
        
	ent_window& operator[]( std::size_t index );
	ent_window& operator[]( GLFWwindow* const );
	ent_window& Cur( void );
        std::size_t CurIndex();
	ent_window& Last( void );
	ent_window* Focused();
        
        void SetCurrent( std::size_t Index );
        bool SetCurrent( ent_window* const Window );
        
	//:: Input
	const std::vector<int> ButtonsToCheck =
	{
		GLFW_MOUSE_BUTTON_LEFT,
		GLFW_MOUSE_BUTTON_RIGHT,
	};
        
};

extern ent_window_manager TheWindowManager;
extern theme& TheTheme;

#endif
