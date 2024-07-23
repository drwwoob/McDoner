// Tools is a helper class containing some methods
// that can be generally used in other classes.

#pragma once

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <string>

//image loading helperxw
class Tools
{
public:
	void pageBar();
	/**
	 * setting background picture with the given address of the background picture.
	 * @param background_name the string of the address pointing to the background picture
	 */
	static void drawBackground(std::string background_name);
	/**
	 * emcrypt word into one string with certain laws
	 * @param word the string needed to be encrypted
	 * @return the encrypted string
	 * @details 
	 * 		/ -> //
	 *		# -> /#
	 *		{ -> /{
	 *		} -> /}
	 *		[ -> /[
	 *		] -> /]
	 */
	static std::string wordEncrypt(std::string word);
	/**
	 * load image into OpenGL texture with common settings (originated from ImGui)
	 */
	static bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
};