
#include <iostream>
#include <system_error> // For throwing exeptions around.
#include <fstream> // I think I load shader files using that.
#include <cstring> //strcpy
#include <utility\deps\stb_image.h> // For loading images. Duh.

#include <utility\deps\opengl_includes.h>

#include <coordinates.h> // For coordinates. Come on. (Unit, point.)

#include <entities\ent_window.h>
#include <entities\ent_opengl.h>

        //:: Textures.

unsigned int loadTexture( const uint8_t* Data, int Length, GLint internal_format, GLenum format, const std::string& NameHint, bool mipmaps )
{
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        //Set the texture wrapping/filtering options. (On the currently bound texture object.)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        //Load and generate the texture.
        stbi_set_flip_vertically_on_load(true);
        int width, height, nrChannels;
        uint8_t *ImageData = stbi_load_from_memory( Data, Length, &width, &height, &nrChannels, 0 );
        
        if( ImageData ) {
                glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, ImageData);
                
                if(mipmaps){
                        glGenerateMipmap(GL_TEXTURE_2D);
                }
        } else {
                stbi_image_free(ImageData);
                throw std::runtime_error( ("'"+NameHint+"' couldn't be loaded.").c_str() );
        }
        
        stbi_image_free(ImageData);
        
        return texture;
                
}

GLFWimage LoadGLFWImage( const uint8_t* Data, const std::size_t Length )
{
        stbi_set_flip_vertically_on_load(false);
	int width, height, nrChannels;
        uint8_t* ImageData = stbi_load_from_memory( Data, Length, &width, &height, &nrChannels, 0 );
        
        GLFWimage Image;
        if( ImageData ) {
                Image.height = height;
                Image.width = width;
                Image.pixels = ImageData;
                return Image;
        } else {
                Image.height = 0;
                Image.width = 0;
                Image.pixels = nullptr;
                return Image;
        }
        
}

        //:: Shaders.

GLchar* loadShaderSource( std::string filepath )
{
	
	GLchar* output;
	std::fstream shaderFile;
	shaderFile.open( filepath, std::fstream::in );
	
	if( not shaderFile.is_open() )
		std::cout << "[Error] Can't open shader source file: " << filepath << std::endl;
	
	std::string current_line;
	std::string shaderSourceString;
	
	while( getline( shaderFile, current_line ) ){
		
		shaderSourceString += current_line + "\n";
		
	}
	
	output = new char[shaderSourceString.length()+1];
	strcpy(output, shaderSourceString.c_str());
	
	return output;
	
}

GLuint compileAndLinkShaders( GLchar* vertexShaderSource, GLchar* fragmentShaderSource, std::string optional_comment = "" )
{
	
	if( optional_comment.length() )
		std::cout << "  " << optional_comment;
	
	//Compile Vertex
	std::cout << "..Vertex";
	GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
	glCompileShader( vertexShader );
	
	//Check vertex errors
	GLint success;
	GLchar infolog[512];
	
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
	if( !success )
	{
		glGetShaderInfoLog( vertexShader, 512, NULL, infolog );
		throw std::runtime_error(infolog);
	}
	
	//Compile Fragment
	std::cout << "..Fragment" << std::endl;
	GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
   	glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
   	glCompileShader( fragmentShader );
   	
   	//Check fragment errors
   	glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
   	
   	if( !success )
	{
		glGetShaderInfoLog( fragmentShader, 512, NULL, infolog );
		throw std::runtime_error(infolog);
	}
	
	// Link shaders
    GLuint linked_shader = glCreateProgram( );
    glAttachShader( linked_shader, vertexShader );
    glAttachShader( linked_shader, fragmentShader );
    glLinkProgram( linked_shader );
    
    // Check for linking errors
    glGetProgramiv( linked_shader, GL_LINK_STATUS, &success );
    
    if ( !success )
    {
    	std::cout << "Oh snap! Some kind of shader linking error!\n";
        glGetProgramInfoLog( linked_shader, 512, NULL, infolog );
        throw std::runtime_error(infolog);
    }
    
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );
	
	return linked_shader;
	
}

        //:: Point.
point Localize( const point& origin, const point& position )
{
	return position-origin;
}
