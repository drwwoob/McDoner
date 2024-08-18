#include "Page.hpp"
#include <iostream>

Page::Page(){

}

Page::Page(const std::shared_ptr<Library>& library_ptr) 
    :
    _library_ptr(library_ptr){
}

Page::Page(const std::string& page_data, 
        const std::shared_ptr<Library>& library_ptr) 
        :
        _library_ptr(library_ptr){
    // page_data should look like
    //  
    // {spirit1##spirit2##spirit3##}
    // {textbox1##textbox2##}
    // {button1##button2##}
    // {formattedPage1_name#formattedPage2_name#}
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
    // pages
    start = end + 2;
    do {
        end = page_data.find("}", start);
    } while(page_data.at(end - 1) == '/');
    data_block = page_data.substr(start, end - start);
    size_t page_parse_position = 0;
    do{
        page_parse_position = data_block.find(",", page_parse_position++);
    }while(page_parse_position < data_block.size());

    // order
    start = end + 2;
    end = page_data.size();
    data_block = page_data.substr(start, end - start - 1);
    auto orderCreate = [this](std::array<std::string, 2>& seperate_data) {
        _draw_order.emplace_back(std::stoi(seperate_data[0]), seperate_data[1]);
    };
    Tools::decrypt<2>(data_block, orderCreate);
}

// Page& Page::operator=(const Page& other) {
//     if (this != &other) {  // Avoid self-assignment
//         _library_ptr = other._library_ptr;
//         // Copy other members...
//     }
//     return *this;
// }

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
        auto file_path = project_path + spirit._spirit_file_name;
        loadImageTexture(file_path, textures);
    }
    for(auto button : _buttons){
        auto file_path = project_path + button.currentSpirit()._spirit_file_name;
        loadImageTexture(file_path, textures);
    }

    for(auto spirit_ptr : _spirit_ptrs){
        auto file_path = project_path + spirit_ptr->_spirit_file_name;
        loadImageTexture(file_path, textures);
    }

    for(auto button_ptr : _button_ptrs){
        auto file_path = project_path + button_ptr->currentSpirit()._spirit_file_name;
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

void Page::drawPage(const std::vector<GLuint>& textures, int size) {
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
            i = size; // Initialize i here to reset its value for each draw_obj
            for(auto& spirit : _spirits) {
                if(spirit._spirit_file_name == draw_obj.second) {
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
            i = size + _spirits.size();
            for(auto& button : _buttons) {
                if(button._nickname == draw_obj.second) {
                    auto spirit = button.currentSpirit();
                    // std::cout << spirit._spirit_file_name << std::endl;
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
            (*_format_pages_ptrs.at(draw_obj.second)).drawPage(textures, 
                size + _spirits.size() + _buttons.size() + _spirit_ptrs.size() + _button_ptrs.size());
            break;
        case 5:
            i = size + _spirits.size() + _buttons.size();
            for(auto& spirit_ptr : _spirit_ptrs) {
                if(spirit_ptr->_spirit_file_name == draw_obj.second) {
                    auto imgSize = spirit_ptr->getSize(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
                    auto topLeft = spirit_ptr->getPosition(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
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
        case 6:
            i = size + _spirits.size() + _buttons.size() + _spirit_ptrs.size();
            for(auto& button_ptr : _button_ptrs) {
                if(button_ptr->_nickname == draw_obj.second) {
                    auto spirit = button_ptr->currentSpirit();
                    // std::cout << spirit._spirit_file_name << std::endl;
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
        default:
            std::cout << "wrong draw_object: " << draw_obj.first << ", " << draw_obj.second << std::endl;
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

void Page::serialize(std::ofstream& outFile) const {
        // Serialize _spirits
        size_t spiritsSize = _spirits.size();
        outFile.write(reinterpret_cast<const char*>(&spiritsSize), sizeof(spiritsSize));
        for (const auto& spirit : _spirits) {
            spirit.serialize(outFile);  // Assuming Spirit has a serialize method
        }

        // Serialize _spirit_ptrs
        size_t spiritPtrsSize = _spirit_ptrs.size();
        outFile.write(reinterpret_cast<const char*>(&spiritPtrsSize), sizeof(spiritPtrsSize));
        for (const auto& spiritPtr : _spirit_ptrs) {
            spiritPtr->serialize(outFile);  // Serialize the object pointed to
        }

        // Serialize _textboxs
        size_t textboxsSize = _textboxs.size();
        outFile.write(reinterpret_cast<const char*>(&textboxsSize), sizeof(textboxsSize));
        for (const auto& textbox : _textboxs) {
            textbox.serialize(outFile);  // Assuming Textbox has a serialize method
        }

        // Serialize _buttons
        size_t buttonsSize = _buttons.size();
        outFile.write(reinterpret_cast<const char*>(&buttonsSize), sizeof(buttonsSize));
        for (const auto& button : _buttons) {
            button.serialize(outFile);  // Assuming Button has a serialize method
        }

        // Serialize _button_ptrs
        size_t buttonPtrsSize = _button_ptrs.size();
        outFile.write(reinterpret_cast<const char*>(&buttonPtrsSize), sizeof(buttonPtrsSize));
        for (const auto& buttonPtr : _button_ptrs) {
            buttonPtr->serialize(outFile);  // Serialize the object pointed to
        }

        // Serialize _format_pages_ptrs
        size_t formatPagesPtrsSize = _format_pages_ptrs.size();
        outFile.write(reinterpret_cast<const char*>(&formatPagesPtrsSize), sizeof(formatPagesPtrsSize));
        for (const auto& [key, pagePtr] : _format_pages_ptrs) {
            size_t keySize = key.size();
            outFile.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
            outFile.write(key.c_str(), keySize);

            pagePtr->serialize(outFile);  // Serialize the page pointed to
        }

        // Serialize _draw_order
        size_t drawOrderSize = _draw_order.size();
        outFile.write(reinterpret_cast<const char*>(&drawOrderSize), sizeof(drawOrderSize));
        for (const auto& pair : _draw_order) {
            outFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));
            size_t stringSize = pair.second.size();
            outFile.write(reinterpret_cast<const char*>(&stringSize), sizeof(stringSize));
            outFile.write(pair.second.c_str(), stringSize);
        }
    }
    void Page::deserialize(std::ifstream& inFile) {
    // Deserialize _spirits
    size_t spiritsSize;
    inFile.read(reinterpret_cast<char*>(&spiritsSize), sizeof(spiritsSize));
    _spirits.resize(spiritsSize);
    for (auto& spirit : _spirits) {
        spirit.deserialize(inFile);  // Assuming Spirit has a deserialize method
    }

    // Deserialize _spirit_ptrs
    size_t spiritPtrsSize;
    inFile.read(reinterpret_cast<char*>(&spiritPtrsSize), sizeof(spiritPtrsSize));
    _spirit_ptrs.resize(spiritPtrsSize);
    for (auto& spiritPtr : _spirit_ptrs) {
        spiritPtr = std::make_shared<Spirit>();
        spiritPtr->deserialize(inFile);  // Deserialize the object pointed to
    }

    // Deserialize _textboxs
    size_t textboxsSize;
    inFile.read(reinterpret_cast<char*>(&textboxsSize), sizeof(textboxsSize));
    _textboxs.resize(textboxsSize);
    for (auto& textbox : _textboxs) {
        textbox.deserialize(inFile);  // Assuming Textbox has a deserialize method
    }

    // Deserialize _buttons
    size_t buttonsSize;
    inFile.read(reinterpret_cast<char*>(&buttonsSize), sizeof(buttonsSize));
    _buttons.resize(buttonsSize);
    for (auto& button : _buttons) {
        button.deserialize(inFile);  // Assuming Button has a deserialize method
    }

    // Deserialize _button_ptrs
    size_t buttonPtrsSize;
    inFile.read(reinterpret_cast<char*>(&buttonPtrsSize), sizeof(buttonPtrsSize));
    _button_ptrs.resize(buttonPtrsSize);
    for (auto& buttonPtr : _button_ptrs) {
        buttonPtr = std::make_shared<Button>();
        buttonPtr->deserialize(inFile);  // Deserialize the object pointed to
    }

    // Deserialize _format_pages_ptrs
    size_t formatPagesPtrsSize;
    inFile.read(reinterpret_cast<char*>(&formatPagesPtrsSize), sizeof(formatPagesPtrsSize));
    for (size_t i = 0; i < formatPagesPtrsSize; ++i) {
        size_t keySize;
        inFile.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));
        std::string key(keySize, '\0');
        inFile.read(&key[0], keySize);

        auto pagePtr = std::make_shared<Page>(_library_ptr);
        pagePtr->deserialize(inFile);
        _format_pages_ptrs[key] = pagePtr;
    }

    // Deserialize _draw_order
    size_t drawOrderSize;
    inFile.read(reinterpret_cast<char*>(&drawOrderSize), sizeof(drawOrderSize));
    _draw_order.resize(drawOrderSize);
    for (auto& pair : _draw_order) {
        inFile.read(reinterpret_cast<char*>(&pair.first), sizeof(pair.first));
        size_t stringSize;
        inFile.read(reinterpret_cast<char*>(&stringSize), sizeof(stringSize));
        pair.second.resize(stringSize);
        inFile.read(&pair.second[0], stringSize);
    }
}
