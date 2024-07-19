#pragma once
#include "../../libs/imgui/imconfig.h"
#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/imgui_internal.h"
#include "../../libs/imgui/imstb_rectpack.h"
#include "../../libs/imgui/imstb_textedit.h"
#include "../../libs/imgui/imstb_truetype.h"
#include "../../libs/imgui/imgui_stdlib.h"


// #include <d3d11.h>
#include <string>
#pragma comment(lib, "d3d11.lib")

//image loading helper


//#include "cast.h"

class Tools
{
public:
	void pageBar();
	static void setBackground(std::string background_name);
	static std::string wordEncrypt(std::string word);
	//helper function
};


// struct texture {
// 	int w = 0;
// 	int h = 0;
// 	ID3D11ShaderResourceView* t = NULL;
// };
