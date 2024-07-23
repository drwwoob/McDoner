#include "Page.h"
#include <iostream>

Page::Page() {
	
}

Page::Page(std::string page_data)
{
	// page_data should look like
	// backgroundName{spirit1##spirit2##spirit3##}{textbox1##textbox2##}

	auto pos = page_data.find_first_of('{');
	background_name = page_data.substr(0, pos);
	//auto backBrac = page_data.find_first_of('}', frontBrac);

	pos++;
	auto start = pos; // start of spirits
	bool inGroup = true;
	auto end = pos;
	std::string str;

	// get the spirits data string
	while(inGroup) {
		if(page_data.at(pos) == '}') { // find the first "}" without / infront
			inGroup = false;
			end = pos;
		}
		if (page_data.at(pos) == '/') { // to skip what comes after '/'
			pos += 1;
		}
		pos++;
		//str = page_data.substr(start, pos);
	}

	auto spiritsData = page_data.substr(start, end - start);
	auto s_start = 0;
	auto s_end = spiritsData.find("##") + 2;

	int para = 0;
	std::string seperateData[6]; // strings for each parameter


	for (int i = 0; i < spiritsData.size(); i++) {
		switch (spiritsData.at(i)) {
		case ('/'):
			i++;
			seperateData[para] += spiritsData.at(i);
			break;
		case('#'):
			para++;
			break;
		default:
			seperateData[para] += spiritsData.at(i);
			break;
		}
		if (para == 7) {
			spirits.emplace_back(seperateData[0], seperateData[1],
				std::stof(seperateData[2]), std::stof(seperateData[3]),
				std::stof(seperateData[4]), std::stof(seperateData[5]));
			for (int s_i = 0; s_i < 6; s_i++) {
				seperateData[s_i].clear();
			}
			para = 0;
		}
	}


	//inGroup = true;
	auto textData = page_data.substr(end + 2, page_data.size() - 4 - end);
	s_start = end + 2;
	para = 0;
	std::string seperateTextData[10];
	for (int i = 0; i < textData.size(); i++) {
		switch (textData.at(i)) {
		case ('/'):
			i++;
			seperateTextData[para] += textData.at(i);
			break;
		case('#'):
			para++;
			break;
		default:
			seperateTextData[para] += textData.at(i);
			break;
		}
		if (para == 10) {
			textboxs.emplace_back(seperateTextData);
			for(int t_i = 0; t_i < 10; t_i++) {
				seperateTextData[t_i].clear();
			}
			para = 0;
		}
	}
	//	}
	//}
}

Spirit* Page::getRealSpirits(int id)
{
	return &spirits.at(id);
}

Textbox* Page::getRealTextbox(int id)
{
	return &textboxs.at(id);
}

void Page::setFont(ImFont* font_given)
{
	for(int i = 0; i < textboxs.size(); i++) {
		textboxs.at(i).changeFont(font_given);
	}
}

// ============= todo: change this, so that the texture only load when needed ==============
void Page::drawPage(ImVec2 window_size, std::string project_path){
    Tools::drawBackground((project_path + background_name));
    for(auto spirit : spirits){
        spiritToTexture(spirit, window_size, project_path);
    }
    for(auto textbox : textboxs){
        drawTextbox(textbox, window_size);
    }
}

void Page::spiritToTexture(Spirit spirit, ImVec2 window_size, std::string project_path) {
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    bool ret = Tools::LoadTextureFromFile((project_path.append(spirit.fileName())).c_str(), &my_image_texture, &my_image_width, &my_image_height);
    if (!ret)
    {
        printf("Failed to load texture: %s\n", spirit.spiritName.c_str());
        printf("location: %s\n", project_path.c_str());
        return;
    }
    ImGui::GetBackgroundDrawList()->AddImage((void*)(intptr_t)my_image_texture, ImVec2(0, 0),
        ImVec2(ImGui::GetIO().DisplaySize.x / 4, ImGui::GetIO().DisplaySize.y / 4), ImVec2(0, 0), ImVec2(1, 1));

	auto imgSize = spirit.getSize(window_size.x, window_size.y);
	auto topLeft = spirit.getPosition(window_size.x, window_size.y);

	// // ---------------------to change---------------------
	// // im thinking the uv for here can change so that the whole picture does not to be added,
	// // but only the part inside the window
	ImGui::GetBackgroundDrawList()->AddImage((void*)my_image_texture, topLeft,
		ImVec2(topLeft.x + imgSize.x, topLeft.y + imgSize.y),
		ImVec2(0, 0), ImVec2(1, 1));

}

void Page::drawTextbox(Textbox textbox, ImVec2 window_size)
{
    // std::cout << textbox.encrypt() << std::endl;
	// ImGui::GetBackgroundDrawList()->AddText(textbox.font, textbox.fontSize,
	// 	ImVec2(textbox.positionRatio.x * window_size.x, textbox.positionRatio.y * window_size.y),
	// 	textbox.color, textbox.content.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(textbox.positionRatio.x * window_size.x, textbox.positionRatio.y * window_size.y),
		textbox.color, textbox.content.c_str());
}

std::string Page::exportInString()
{
	std::string encrypt = "[";

	// add background name
	encrypt.append(background_name);

	// add spirits
	encrypt.append("{");
	for(auto spirit : spirits) {
		encrypt.append(spirit.toString());
	}
	encrypt.append("}");

	// add textboxs;
	encrypt.append("{");
	for (auto textbox : textboxs) {
		encrypt.append(textbox.encrypt());
		//encrypt.append(textbox);
	}
	encrypt.append("}");
	

	// ending this page
	encrypt.append("]");
	return encrypt;
}