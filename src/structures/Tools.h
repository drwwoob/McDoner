#pragma once
#include "imconfig.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imstb_rectpack.h"
#include "imstb_textedit.h"
#include "imstb_truetype.h"
#include "imgui_stdlib.h"
#include <string>

#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include <imgui_impl_opengl3_loader.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>

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


// struct texture {
// 	int w = 0;
// 	int h = 0;
// 	ID3D11ShaderResourceView* t = NULL;
// };

