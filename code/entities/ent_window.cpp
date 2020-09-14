#include <entities\ent_window.h>

void ent_window::SwitchTab( w_tab* Tab )
{
        if( Tab == nullptr ){
                return;
        }
        this->ActiveTab.SwitchTab( Tab );
        if( Tab->Title.size() != 0 )
        {
                glfwSetWindowTitle( this->Reference, Tab->Title.c_str() );
        }
}

bool ent_window::Visible()
{
        return !glfwGetWindowAttrib( this->Reference, GLFW_ICONIFIED );
}


ent_window_manager TheWindowManager;
theme& TheTheme = TheWindowManager.Theme;

ent_window& ent_window_manager::operator[]( std::size_t index )
{
	return this->Windows[ index ];
}

ent_window& ent_window_manager::operator[]( GLFWwindow* const Window )
{
        for( std::size_t i = 0; i < this->Windows.size(); i++ ) {
                if( (*this)[i].Reference == Window ) {
                        return (*this)[i];
                }
        }
        // It's your fault anyway.
        throw std::out_of_range(
                ClassName( *this ) + "::" + __func__ +
                " -- Window with specified reference doesn't exist in the manager."
        );
}

ent_window& ent_window_manager::Cur( void )
{
        return (*this)[this->CurIndex()];
}

std::size_t ent_window_manager::CurIndex()
{
        const GLFWwindow* CurrentReference = glfwGetCurrentContext();
        
        for( std::size_t i = 0; i < this->Windows.size(); i++ ) {
                
                ent_window& Window = (*this)[i];
                
                if( Window.Reference == CurrentReference ) {
                        return i;
                }
                
        }
        
        return std::size_t(-1);
        
}


ent_window& ent_window_manager::Last( void )
{
	return (*this)[this->Windows.size()-1];
}

ent_window* ent_window_manager::Focused()
{
        for( std::size_t i = 0; i < this->Windows.size(); i++ ) {
                const bool bFocused = glfwGetWindowAttrib( this->Windows[i].Reference, GLFW_FOCUSED );
                if( bFocused ) {
                        return &this->Windows[i];
                }
        }
        return nullptr;
}


void ent_window_manager::SetCurrent( std::size_t Index )
{
        glfwMakeContextCurrent( (*this)[Index].Reference );
}

bool ent_window_manager::SetCurrent( ent_window *const Window )
{
        for( std::size_t i = 0; i < this->Windows.size(); i++ ) {
                ent_window* const Current = &((*this)[i]);
                if( Current == Window ) {
                        this->SetCurrent(i);
                        return true;
                }
        }
        return false;
}


//Yay.
