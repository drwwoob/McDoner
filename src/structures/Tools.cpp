#include "Tools.h"
#include "imgui.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Tools::pageBar() {}

std::string Tools::wordEncrypt(const std::string& word) {
    std::string EncryptedWord;
    for(const auto letter : word) {
        switch(letter) {
        case '#':
            EncryptedWord.append("/#");
            break;
        case '/':
            EncryptedWord.append("//");
            break;
        case '{':
            EncryptedWord.append("/{");
            break;
        case '}':
            EncryptedWord.append("/}");
            break;
        default:
            EncryptedWord.push_back(letter);
        }
    }
    return EncryptedWord;
}

void Tools::drawBackground(const std::string& background_name) {
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    bool ret = LoadTextureFromFile(background_name.c_str(), &my_image_texture, &my_image_width, &my_image_height);
    if(!ret) {
        printf("Failed to load texture: %s\n", background_name.c_str());
        return;
    }
    ImGui::GetBackgroundDrawList()->AddImage((void*)(intptr_t)my_image_texture, ImVec2(0, 0), ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImVec2(0, 0), ImVec2(1, 1));
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool Tools::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height) {
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if(image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

std::unordered_map<std::string, std::string> Tools::loadJson(const std::string& filename, std::function<void(std::unordered_map<std::string, std::string>&, nlohmann::json&)> func){
    if (!std::filesystem::exists(filename)) {
        throw std::runtime_error("Keybindings file not exist: " + filename);
    }
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open keybindings file: " + filename);
    }
    
    nlohmann::json key_bindings;
    file >> key_bindings;

    std::unordered_map<std::string, std::string> shortkey_map {};
    func(shortkey_map, key_bindings);
    return shortkey_map;
}