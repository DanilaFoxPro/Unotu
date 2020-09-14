#include <utility\clipboard.h>

#include <utility\deps\opengl_includes.h>

std::string GetClipboard()
{
        const char* UTF8_String = glfwGetClipboardString(nullptr);
        std::string Output;
        // HACK: You don't convert UTF-8 to ASCII like that.
        for( std::size_t i = 0; UTF8_String[i] != 0; i++ ) {
                const uint8_t Current = UTF8_String[i];
                if( Current & 128 ) {
                        break;
                } else {
                        Output += Current;
                }
        }
        return Output;
}

void SetClipboard( const std::string& String )
{
        glfwSetClipboardString( nullptr, String.data() );
}

