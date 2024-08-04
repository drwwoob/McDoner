#include "Page.h"
#include <iostream>

Page::Page() {
}

Page::Page(std::string page_data) {
    // page_data should look like
    // {spirit1##spirit2##spirit3##}{textbox1##textbox2##}{order1first#order1second##order2first#order2second##}

    auto pos = page_data.find_first_of('{');
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
        if(page_data.at(pos) == '/') { // to skip what comes after '/'
            pos += 1;
        }
        pos++;
        //str = page_data.substr(start, pos);
    }

    auto data_block = page_data.substr(start, end - start);

    auto spiritCreate = [this](std::vector<std::string>& seperate_data) {
        _spirits.emplace_back(
            seperate_data[0],
            seperate_data[1],
            std::stof(seperate_data[2]),
            std::stof(seperate_data[3]),
            std::stof(seperate_data[4]),
            std::stof(seperate_data[5]));
    };
    decrypt(data_block, 6, spiritCreate);

    // textbox
    //inGroup = true;
    start = end + 2;
    do {
        end = page_data.find("}", end + 1);
    } while(page_data.at(end - 1) == '/');
    data_block = page_data.substr(start, end - start - 1);
    auto textCreate = [this](std::vector<std::string>& seperate_data) {
        _textboxs.emplace_back(seperate_data);
    };
    decrypt(data_block, 9, textCreate);

    // order
    start = end + 2;
    end = page_data.size();
    data_block = page_data.substr(start, end - start - 1);
    auto orderCreate = [this](std::vector<std::string>& seperate_data) {
        _draw_order.emplace_back(std::stoi(seperate_data[0]), seperate_data[1]);
    };
    decrypt(data_block, 2, orderCreate);
}

Spirit* Page::getRealSpirits(int id) {
    return &_spirits.at(id);
}

Textbox* Page::getRealTextbox(int id) {
    return &_textboxs.at(id);
}

void Page::setFont(ImFont* font_given) {
    for(int i = 0; i < _textboxs.size(); i++) {
        _textboxs.at(i).changeFont(font_given);
    }
}

// ============= only load when needed ==============
std::vector<GLuint> Page::loadPage(const std::string& project_path) {
    std::vector<GLuint> textures;

    // std::string file_path;
    for(auto spirit : _spirits) {
        auto file_path = project_path + spirit.fileName();
        loadImageTexture(file_path, textures);
    }
    return textures;
}

void Page::decrypt(const std::string& data_block, int size, const std::function<void(std::vector<std::string>&)>& func) {
    std::vector<std::string> seperate_data{""};
    for(int i = 0; i < data_block.size(); i++) {
        switch(data_block.at(i)) {
        case('/'):
            i++;
            seperate_data.back() += data_block.at(i);
            break;
        case('#'):
            seperate_data.push_back("");
            break;
        default:
            seperate_data.back() += data_block.at(i);
            break;
        }
        if(seperate_data.size() == size + 2) {
            func(seperate_data);
            seperate_data = {""};
            // seperate_data.clear();
            // seperate_data.push_back("");
        }
    }
}

void Page::loadImageTexture(std::string& name, std::vector<GLuint>& textures) {
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    bool ret = Tools::LoadTextureFromFile(name.c_str(), &my_image_texture, &my_image_width, &my_image_height);
    if(!ret) {
        printf("Failed to load texture: %s\n", name.c_str());
        return;
    }
    textures.emplace_back(my_image_texture);
}

void Page::drawPage(std::vector<GLuint>& textures) {
    int i = 0;
    for(auto draw_obj : _draw_order) {
        // std::cout << draw_obj.first << ", " << draw_obj.second << std::endl;
        switch(draw_obj.first) {
        // case 0:
        //     ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)(uintptr_t)textures[0],
        //                                              ImVec2(0, 0),
        //                                              ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y),
        //                                              ImVec2(0, 0),
        //                                              ImVec2(1, 1));
        //     break;
        case 1:
            i = 0; // Initialize i here to reset its value for each draw_obj
            for(auto& spirit : _spirits) {
                if(spirit.fileName() == draw_obj.second) {
                    auto imgSize = spirit.getSize(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
                    auto topLeft = spirit.getPosition(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
                    ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)(uintptr_t)textures[i],
                                                             topLeft,
                                                             ImVec2(topLeft.x + imgSize.x, topLeft.y + imgSize.y),
                                                             ImVec2(0, 0),
                                                             ImVec2(1, 1));
                    break;
                }
                i++;
            }
            break;
        case 2:
            for(auto& textbox : _textboxs) {
                if(textbox._name == draw_obj.second) {
                    ImGui::GetBackgroundDrawList()->AddText(
                        ImVec2(
                            textbox._position_ratio.x * ImGui::GetIO().DisplaySize.x,
                            textbox._position_ratio.y * ImGui::GetIO().DisplaySize.y),
                        textbox._color,
                        textbox._content.c_str());
                }
                break;
            }
            break;
        default:
            std::cout << "wrong name: " << draw_obj.second << std::endl;
            break;
        }
    }
}

std::string Page::exportInString() {
    std::string encrypt = "[";
    // add spirits
    encrypt.append("{");
    for(auto spirit : _spirits) {
        encrypt.append(spirit.toString());
    }
    encrypt.append("}");

    // add textboxs
    encrypt.append("{");
    for(auto textbox : _textboxs) {
        encrypt.append(textbox.encrypt());
        //encrypt.append(textbox);
    }
    encrypt.append("}");

    // add order
    encrypt.append("{");
    for(auto order_obj : _draw_order) {
        encrypt.append(std::to_string(order_obj.first));
        encrypt.append("#");
        encrypt.append(order_obj.second);
        encrypt.append("##");
    }
    encrypt.append("}");

    // ending this page
    encrypt.append("]");
    return encrypt;
}