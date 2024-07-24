// This class only have inline header file, 
// and it stores the information of a single text box.
#pragma once
#include <string>
#include "Tools.h"
#include <imgui.h>

class Textbox
{
public:
	std::string name;		// name for this textbox
	std::string content;	// content for this textbox
	/**
	 * the ratio of the position the textbox's left-top corner is at
	 * in the format [position.x / winSize.x, position.y / winSize.y]
	*/
	ImVec2 positionRatio;
	ImFont* font;			// the font the textbox uses
	std::string fontPath;	// the path to the data of this font
	ImColor color;			// the color of the text in this textbox
	float fontSize;			// the size of the text

    /**
	 * creating a blank textbox
     * @overload Constructor()
     */
	Textbox();

    /**
	 * creating a textbox with giving settings
     * @overload Constructor(std::string data_str[10])
     * @param data_str  [name, content, positionRatio.x, positionRatio.y, fontPath, color.r, color.g, color.b, color.a, fontSize]
     */
	Textbox(std::vector<std::string> data_str) { // , ImGuiIO& io
		name = data_str[0];
		content = data_str[1];
		positionRatio[0] = std::stof(data_str[2]);
		positionRatio[1] = std::stof(data_str[3]);
		if(data_str[4] == "") {
			fontPath = ""; // into default font Path
		}
		else {
			fontPath = data_str[4];
		}
		//font =  // load font

		font = NULL;
		//font = ImGui::GetIO().Fonts->AddFontDefault(); //for default

		// default for now
		//font = io.Fonts->AddFontDefault();

		color = ImColor(std::stof(data_str[5]),
			std::stof(data_str[6]),
			std::stof(data_str[7]),
			std::stof(data_str[8]));

		fontSize = std::stof(data_str[9]);


	} 
	
    /**
     * get &content of this textbox
     * @return &content
     */
	std::string* getRealContent() {
		return &content;
	}

    /**
     * set the font for this textbox
     * @param font_given the desired font with type ImFont*
     */
	void changeFont(ImFont* font_given) {
		font = font_given;
	}

    /**
     * encrypt the textbox into a std::string
     * @return the encrypted textbox information
     *          in the format: name#content#position[0]#position[1]#fontPath#color.r#color.g#color.b#color.a#fontSize##
     */
	std::string encrypt() {
		std::string WordEncry = Tools::wordEncrypt(name);

		WordEncry += "#";
		WordEncry += Tools::wordEncrypt(content);
		WordEncry += "#";
		WordEncry += std::to_string(positionRatio[0]);
		WordEncry += "#";
		WordEncry += std::to_string(positionRatio[1]);
		WordEncry += "#";
		WordEncry += Tools::wordEncrypt(fontPath);
		WordEncry += "#";
		WordEncry += std::to_string(color.Value.x);
		WordEncry += "#";
		WordEncry += std::to_string(color.Value.y);
		WordEncry += "#";
		WordEncry += std::to_string(color.Value.z);
		WordEncry += "#";
		WordEncry += std::to_string(color.Value.w);
		WordEncry += "#";
		WordEncry += std::to_string(fontSize);
		WordEncry += "##";

		return WordEncry;
	}
};

