#include "Data.hpp"

#include <fstream>
#include <iostream>

const std::shared_ptr<Library> Data::_library_ptr = std::make_shared<Library>();

Data::Data() {
}

Data::Data(const std::string& file_path) {
    // initiate path
    _file_name = file_path;
    _project_path = file_path.substr(
        0,
        file_path.find_last_of("/") + 1);

    // ============= this is canceled for now, might need to create another Library for this ==============
    // or else it would require an extra value to store which one belongs to what file
    // // reading system library
    // if(std::filesystem::exists("../src/settings/library.txt")){
    //     *_library_ptr->
    // }

    // set up file_data holder
    std::ifstream file;
    std::string file_data = "";
    std::string line;

    // reading project library
    // don't think this would need to be checked once i fully implement Library save method
    if(std::filesystem::exists(_project_path + "library.txt")){
        file.open(_project_path + "library.txt", std::ios::in);
        while(std::getline(file, line)){
            file_data += line;
        }
        _library_ptr->decryptData(file_data);
        file.close();
    }

    // reading save
    file.open(file_path, std::ios::in);
    file_data = "";
    line = "";
    while(std::getline(file, line)) {
        file_data += line;
    }
    decryptFile(file_data);
    file.close();
}

Data::Data(const std::string& project_path, const char * project_name) {
    _project_path = project_path;
    std::filesystem::create_directory(project_path);
    _file_name = project_path + "/" + project_name + ".txt";
    _pages = {Page(_library_ptr)};
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

void Data::draw() {
    _pages.at(_page_at).drawPage(_textures, 0);
}

Page* Data::getPage(int page_id) {
    return &_pages.at(page_id);
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
    int count = 0;
    _page_at = std::stoi(data_str.substr(0, pos));
    while(pos < data_str.size()) {
        if(data_str.at(pos) == '/') {
                    pos++;
        }
        if(data_str.at(pos) == '[') { // whether pos is '[', go to the next letter
            start = pos;
            count++;       // making sure it's in the same scope
        }
        else if(data_str.at(pos) == ']'){
            count--;
            if(count == 0){
                _pages.emplace_back(data_str.substr(start, pos - start), _library_ptr);
                _pages.back().setFont(_font);
            }
        }
        pos++;
    }
}

void Data::ImportFormattedPages(const std::string& path){
}

void Data::changeProjectName(){
}

void Data::save() {
    std::ofstream file;
    //for testing
    auto path = _file_name;
    file.open(path);
    file << encryptIntoFile().c_str();
    file.close();

    path = _project_path + "library.txt";
    file.open(path);
    file << _library_ptr->encrypt().c_str();
    file.close();
}

void Data::addPage(int page_id, Page page) {
    _pages.emplace(_pages.begin() + page_id, page);
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
    loadFormattedPages();
}

void Data::loadFormattedPages(){
    // guess i have to update this
    // // load default interfaces
    // std::ifstream file("../src/settings/buttonInterface.txt", std::ios::in);
    // std::string settings = "";
    // std::string line;
    // while(std::getline(file, line)) {
    //     settings += line;
    // }

    // // load specified interfaces
    // if(std::filesystem::exists(_project_path + "buttonInterface.txt")){
    //     file.open(_project_path + "buttonInterface.txt", std::ios::in);
    //     while(std::getline(file, line)) {
    //         settings += line;
    //     }
    // }

    // // decrypt settings string
    
}