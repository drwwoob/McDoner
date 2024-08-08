// Tools is a helper class containing some methods
// that can be generally used in other classes.

#ifndef TOOLS_H
#define TOOLS_H

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>

//image loading helperxw
class Tools {
   public:
    void pageBar();
    /**
	 * setting background picture with the given address of the background picture.
	 * @param background_name the string of the address pointing to the background picture
	 */
    static void drawBackground(const std::string& background_name);
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
    static std::string wordEncrypt(const std::string& word);
    /**
	 * load image into OpenGL texture with common settings (originated from ImGui)
	 */
    static bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

    static void drawImage();

    // template <std::size_t Size>
	// static std::unordered_map<std::string, std::string> loadJson(const std::string &filename, std::array<std::string, Size>& keys);
    static std::unordered_map<std::string, std::string> loadJson(const std::string &filename, std::function<void(std::unordered_map<std::string, std::string>&, nlohmann::json&)> func);

// template <std::size_t Size>
//     static void decrypt(const std::function<void(const std::array<std::string, Size> &)>& func);
    // static void decrypt(const std::string &data_block, const std::function<void(const std::array<std::string, Size> &)>& func);
// 	/**
// 	 * @param decrypt_type
// 	 * 	0: Spirit
// 	 * 	1: Textbox
//	 *	2: Button
// 	 */
template <std::size_t Size>
    static void decrypt(const std::string& data_block, const std::function<void(std::array<std::string, Size>&)>& func);
};

#include "Tools.tpp"

#endif // TOOLS_H

