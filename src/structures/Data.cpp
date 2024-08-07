#include "Data.h"

#include <fstream>
#include <iostream>

Data::Data() {
}

Data::Data(const std::string& file_path) {
    // initiate demoPath
    _file_name = file_path;
    _project_path = file_path.substr(
        0,
        file_path.find_last_of("/") + 1);
    std::ifstream file(file_path, std::ios::in);
    std::string file_data = "";
    std::string line;
    while(std::getline(file, line)) {
        file_data += line;
    }
    decryptFile(file_data);
}

Data::Data(const std::string& project_path, const char * project_name) {
    _project_path = project_path;
    std::filesystem::create_directory(project_path);
    _file_name = project_path + "/" + project_name + ".txt";
    _pages = {Page()};
    _page_at = 0;
}

void Data::setFont(ImFont* font_given) {
    _font = font_given;
    //for (int i = 0; i < pages.size(); i++) {
    //    pages.at(i).setFont(font_given);
    //}
}

void Data::loadTexture() {
    _textures = _pages.at(_page_at).loadPage(_project_path);
}

void Data::draw(const bool show_buttons) {
    _pages.at(_page_at).drawPage(_textures, show_buttons);
}

std::unique_ptr<Page> Data::getPage(int page_id) {
    return std::make_unique<Page>(_pages.at(page_id));
}

std::string Data::encryptIntoFile() {
    std::string pagesInfo;
    pagesInfo.append(std::to_string(_page_at) + ",");
    for(auto page : _pages) {
        pagesInfo.append(page.encrpyt());
    }

    return pagesInfo;
}

void Data::decryptFile(const std::string& data_str) {
    // clear all old data
    // have to default font here
    _pages.clear();

    // get new data
    int pos = 0;
    int start = 0;
    pos = data_str.find(",");
    _page_at = std::stoi(data_str.substr(0, pos));
    while(pos < data_str.size()) {
        if(data_str.at(pos++) == '[') { // whether pos is '[', go to the next letter
            start = pos;
            while(data_str.at(pos) != ']') {
                if(data_str.at(pos) == '/') {
                    pos++;
                }
                pos++;
            }
            _pages.emplace_back(data_str.substr(start, pos - start));
            _pages.back().setFont(_font);
            pos++;
        }
    }
}

void Data::ImportButtonInterface(const std::string& path){
}

void Data::changeProjectName(){
}

void Data::save() {
    std::ofstream file;
    //for testing
    auto path = _file_name;
    auto data = encryptIntoFile();
    file.open(path);
    file << encryptIntoFile().c_str();
    file.close();
}

void Data::addPage(int page_id) {
    _pages.emplace(_pages.begin() + page_id);
}

void Data::CopyPage(int page_id, Page page) {
    for(int i = 0; i < page._textboxs.size(); i++) {
        page._textboxs.at(i)._content = "Enter new text";
    }
    _pages.insert(_pages.begin() + page_id, page);
}

void Data::deletePage(int page_id) {
    _pages.erase(_pages.begin() + page_id);
}

void Data::loadSettings(){
    loadButtonInterfaces();
}

void Data::loadButtonInterfaces(){
    // load default interfaces
    std::ifstream file("../src/settings/buttonInterface.txt", std::ios::in);
    std::string settings = "";
    std::string line;
    while(std::getline(file, line)) {
        settings += line;
    }

    // load specified interfaces
    if(std::filesystem::exists(_project_path + "buttonInterface.txt")){
        file.open(_project_path + "buttonInterface.txt", std::ios::in);
        while(std::getline(file, line)) {
            settings += line;
        }
    }

    // decrypt settings string
    
}
