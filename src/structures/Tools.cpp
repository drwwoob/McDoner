#include "Tools.h"
#include "imgui.h"
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Tools::pageBar() {}

std::string Tools::wordEncrypt(std::string word) {
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

// #if defined(__APPLE__)

// #include <Cocoa/Cocoa.h>

// std::string Tools::openFileDialog() {
// 	@autoreleasepool {
// 		NSOpenPanel* openDlg = [NSOpenPanel openPanel];
// 		[openDlg setCanChooseFiles:YES];
// 		[openDlg setAllowsMultipleSelection:NO];
// 		[openDlg setCanChooseDirectories:NO];

// 		if ([openDlg runModal] == NSModalResponseOK) {
// 			NSURL* fileURL = [openDlg URLs][0];
// 			return std::string([[fileURL path] UTF8String]);
// 		}
// 	}
// 	return "";
// }

// #endif

#if defined(_WIN32)

#include <windows.h>
#include <commdlg.h>

std::string Tools::openFileDialog() {
    char filename[MAX_PATH] = "";
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "";

    if(GetOpenFileName(&ofn)) {
        return std::string(filename);
    }
    return "";
}

#endif

nlohmann::json Tools::jsonloadKeyBindings(const std::string& filename){
    if (!std::filesystem::exists(filename)) {
        throw std::runtime_error("Unable to open keybindings file: " + filename);
    }
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open keybindings file: " + filename);
    }
    
    nlohmann::json keyBindings;
    file >> keyBindings;
    return keyBindings;
}

std::string Tools::getPlatformKey(const nlohmann::json& keyBindings, const std::string& action) {
#ifdef __APPLE__
    return keyBindings.at(action).at("Mac").get<std::string>();
#else
    return keyBindings.at(action).at("Windows").get<std::string>();
#endif
}