#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <string>

//image loading helperxw
class Tools
{
public:
	void pageBar();
	static void drawBackground(std::string background_name);
	static std::string wordEncrypt(std::string word);
	static bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
	//helper function
};