#include "Page.hpp"
#include <iostream>

Page::Page() {
}

Page::Page(const std::string& page_data) {
    // page_data should look like
    //
    // {spirit1##spirit2##spirit3##}
    // {textbox1##textbox2##}
    // {button1##button2##}
    // {formattedPage1_name#formattedPage2_name}
    // {order1first#order1second##order2first#order2second##}
    //

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


// std::function<void (std::array<std::string, Size> &)>
// std::function<void (std::array<std::string, Size> &)>& func)
    auto spiritCreate = [this](std::array<std::string, 6>& seperate_data) {
        _spirits.emplace_back(
            seperate_data[0],
            seperate_data[1],
            std::stof(seperate_data[2]),
            std::stof(seperate_data[3]),
            std::stof(seperate_data[4]),
            std::stof(seperate_data[5]));
    };
    Tools::decrypt<6>(data_block, spiritCreate);

    // textbox
    //inGroup = true;
    start = end + 2;
    do {
        end = page_data.find("}", start);
    } while(page_data.at(end - 1) == '/');
    data_block = page_data.substr(start, end - start);
    auto textCreate = [this](std::array<std::string, 10>& seperate_data) {
        _textboxs.emplace_back(seperate_data);
    };
    Tools::decrypt<10>(data_block, textCreate);

    // buttons
    start = end + 2;
    do {
        end = page_data.find("}", start);
    } while(page_data.at(end - 1) == '/');
    data_block = page_data.substr(start, end - start);
    if(end != start){
        std::string single_button = "";
        int count = 0;
        for (std::size_t i = 0; i < data_block.size(); i++) {
            switch (data_block[i]) {
                case '/':
                    i++;
                    single_button += data_block[i];
                    break;
                case '#':
                    single_button += data_block[i];
                    if (data_block[i + 1] < data_block.size() && data_block[i + 1] == '#') {
                        count++;
                        i++;
                        single_button += data_block[i];
                    }
                    break;
                default:
                    single_button += data_block[i];
                    break;
            }
            if(count == 5){
                count = 0;
                _buttons.push_back(single_button);
                single_button = "";
            }
        }
    }
    // // pages
    // start = end + 2;
    // do {
    //     end = page_data.find("}", start);
    // } while(page_data.at(end - 1) == '/');
    // data_block = page_data.substr(start, end - start);


    // order
    start = end + 2;
    end = page_data.size();
    data_block = page_data.substr(start, end - start - 1);
    auto orderCreate = [this](std::array<std::string, 2>& seperate_data) {
        _draw_order.emplace_back(std::stoi(seperate_data[0]), seperate_data[1]);
    };
    Tools::decrypt<2>(data_block, orderCreate);
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
// button refresher should be at the place detecting the button's status
std::vector<GLuint> Page::loadPage(const std::string& project_path) {
    std::vector<GLuint> textures;

    // std::string file_path;
    for(auto spirit : _spirits) {
        auto file_path = project_path + spirit.fileName();
        loadImageTexture(file_path, textures);
    }
    for(auto button : _buttons){
        auto file_path = project_path + button.currentSpirit().fileName();
        loadImageTexture(file_path, textures);
    }
    for(auto page_ptr : _format_pages_ptrs){
        auto formatted_texture = (*page_ptr.second).loadPage(project_path);
        textures.insert(textures.end(), formatted_texture.begin(), formatted_texture.end());
    }
    return textures;
}

void Page::loadImageTexture(const std::string& name, std::vector<GLuint>& textures) {
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

void Page::drawPage(const std::vector<GLuint>& textures) {
    int i = 0;
    for(auto draw_obj : _draw_order) {
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
        case 3:
            i = _spirits.size();
            for(auto& button : _buttons) {
                if(button._nickname == draw_obj.second) {
                    auto spirit = button.currentSpirit();
                    // std::cout << spirit.fileName() << std::endl;
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
        case 4:
            (*_format_pages_ptrs.at(draw_obj.second)).drawPage(textures);
            break;
        default:
            std::cout << "wrong name: " << draw_obj.second << std::endl;
            break;
        }
    }
}

std::string Page::encrpyt() {
    std::string encrypt = "[";
    // add spirits
    encrypt += '{';
    for(auto spirit : _spirits) {
        encrypt.append(spirit.encrypt());
    }
    encrypt += '}';

    // add textboxs
    encrypt += '{';
    for(auto textbox : _textboxs) {
        encrypt.append(textbox.encrypt());
        //encrypt.append(textbox);
    }
    encrypt += '}';

    // add order
    encrypt += '{';
    for(auto order_obj : _draw_order) {
        encrypt.append(std::to_string(order_obj.first));
        encrypt += '#';
        encrypt.append(order_obj.second);
        encrypt.append(2, '#');
    }
    encrypt += '}';

    // ending this page
    encrypt += ']';
    return encrypt;
}