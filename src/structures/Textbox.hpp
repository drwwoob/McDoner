// This class only have inline header file,
// and it stores the information of a single text box.
#pragma once
#include <string>
#include "Tools.hpp"
#include <imgui.h>
#include <fstream>
#include <array>

class Textbox {
   public:
    std::string _name;    // name for this textbox
    std::string _content; // content for this textbox
    /**
	 * the ratio of the position the textbox's left-top corner is at
	 * in the format [position.x / winSize.x, position.y / winSize.y]
	*/
    ImVec2 _position_ratio;
    ImFont* _font;         // the font the textbox uses
    std::string _font_path; // the path to the data of this font
    ImColor _color;        // the color of the text in this textbox
    float _font_size;       // the size of the text

    /**
	 * creating a blank textbox
     * @overload Constructor()
     */
    Textbox(){};

    /**
	 * creating a textbox with giving settings
     * @overload Constructor(std::string data_str[10])
     * @param data_str  [name, content, positionRatio.x, positionRatio.y, fontPath, color.r, color.g, color.b, color.a, fontSize]
     */
    Textbox(std::array<std::string, 10> data_str) { // , ImGuiIO& io
        _name = data_str[0];
        _content = data_str[1];
        _position_ratio[0] = std::stof(data_str[2]);
        _position_ratio[1] = std::stof(data_str[3]);
        if(data_str[4] == "") {
            _font_path = ""; // into default font Path
        }
        else {
            _font_path = data_str[4];
        }
        //font =  // load font

        _font = NULL;
        //font = ImGui::GetIO().Fonts->AddFontDefault(); //for default

        // default for now
        //font = io.Fonts->AddFontDefault();

        _color = ImColor(std::stof(data_str[5]),
                        std::stof(data_str[6]),
                        std::stof(data_str[7]),
                        std::stof(data_str[8]));

        _font_size = std::stof(data_str[9]);
    }

    ~Textbox(){
        delete _font;
    }

    /**
     * get &content of this textbox
     * @return &content
     */
    std::string& getRealContent(){
        return _content;
    };  // This function should return a reference


    /**
     * set the font for this textbox
     * @param font_given the desired font with type ImFont*
     */
    void changeFont(ImFont* font_given) {
        _font = font_given;
    }

    /**
     * encrypt the textbox into a std::string
     * @return the encrypted textbox information
     *          in the format: name#content#position[0]#position[1]#fontPath#color.r#color.g#color.b#color.a#fontSize##
     */
    std::string encrypt() {
        std::string WordEncry = Tools::wordEncrypt(_name);

        WordEncry += '#';
        WordEncry += Tools::wordEncrypt(_content);
        WordEncry += '#';
        WordEncry += std::to_string(_position_ratio[0]);
        WordEncry += '#';
        WordEncry += std::to_string(_position_ratio[1]);
        WordEncry += '#';
        WordEncry += Tools::wordEncrypt(_font_path);
        WordEncry += '#';
        WordEncry += std::to_string(_color.Value.x);
        WordEncry += '#';
        WordEncry += std::to_string(_color.Value.y);
        WordEncry += '#';
        WordEncry += std::to_string(_color.Value.z);
        WordEncry += '#';
        WordEncry += std::to_string(_color.Value.w);
        WordEncry += '#';
        WordEncry += std::to_string(_font_size);
        WordEncry += "##";

        return WordEncry;
    }

    void serialize(std::ofstream& outFile) const {
        // Serialize _name
        size_t nameSize = _name.size();
        outFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        outFile.write(_name.c_str(), nameSize);

        // Serialize _content
        size_t contentSize = _content.size();
        outFile.write(reinterpret_cast<const char*>(&contentSize), sizeof(contentSize));
        outFile.write(_content.c_str(), contentSize);

        // Serialize _position_ratio
        outFile.write(reinterpret_cast<const char*>(&_position_ratio), sizeof(_position_ratio));

        // Serialize _font_path
        size_t fontPathSize = _font_path.size();
        outFile.write(reinterpret_cast<const char*>(&fontPathSize), sizeof(fontPathSize));
        outFile.write(_font_path.c_str(), fontPathSize);

        // Serialize _color
        outFile.write(reinterpret_cast<const char*>(&_color), sizeof(_color));

        // Serialize _font_size
        outFile.write(reinterpret_cast<const char*>(&_font_size), sizeof(_font_size));
    }

    void deserialize(std::ifstream& inFile) {
        // Deserialize _name
        size_t nameSize;
        inFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        _name.resize(nameSize);
        inFile.read(&_name[0], nameSize);

        // Deserialize _content
        size_t contentSize;
        inFile.read(reinterpret_cast<char*>(&contentSize), sizeof(contentSize));
        _content.resize(contentSize);
        inFile.read(&_content[0], contentSize);

        // Deserialize _position_ratio
        inFile.read(reinterpret_cast<char*>(&_position_ratio), sizeof(_position_ratio));

        // Deserialize _font_path
        size_t fontPathSize;
        inFile.read(reinterpret_cast<char*>(&fontPathSize), sizeof(fontPathSize));
        _font_path.resize(fontPathSize);
        inFile.read(&_font_path[0], fontPathSize);

        // Deserialize _color
        inFile.read(reinterpret_cast<char*>(&_color), sizeof(_color));

        // Deserialize _font_size
        inFile.read(reinterpret_cast<char*>(&_font_size), sizeof(_font_size));
    }
};
