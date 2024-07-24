#include "Data.h"

#include <fstream>

Data::Data(){
}

Data::Data(std::string file_path)
{
    // initiate demoPath
    file_name = file_path;
    project_path = file_path.substr(
        0,
        file_path.find_last_of("/") + 1
    );

    std::ifstream file(file_path, std::ios::in);
    std::string file_data = "";
    std::string line;
    while(std::getline(file, line)){
        file_data += line;
    }
    decryptFile(file_data);
}

void Data::newFile()
{
    pages = {Page()};
}

void Data::openFile()
{
    // get the file_path here first

    // decryptFile();
}

void Data::setFont(ImFont* font_given)
{
    font = font_given;
    //for (int i = 0; i < pages.size(); i++) {
    //    pages.at(i).setFont(font_given);
    //}
}

void Data::loadTexture(int page_at)
{
    textures = pages.at(page_at).loadPage(project_path);
}

void Data::draw(int page_at)
{
    pages.at(page_at).drawPage(textures);
}

Page* Data::getPage(int page_id) {
	return &pages.at(page_id);
}

std::string Data::encryptIntoFile()
{
	std::string pagesInfo;

    for(auto page : pages) {
        pagesInfo.append(page.exportInString());
    }

    return pagesInfo;
}

void Data::decryptFile(std::string data_str)
{
    int pos = 0;
    int start = 0;
    while(pos < data_str.size()) {
	    if(data_str.at(pos++) == '[') { // whether pos is '[', go to the next letter
            start = pos;
            while(data_str.at(pos) != ']') {
                if(data_str.at(pos) == '/') {
                    pos++;
                }
                pos++;
            }
            pages.emplace_back(data_str.substr(start, pos));
            pages.back().setFont(font);
            pos++;
	    }
    }
}

void Data::save() {
    std::ofstream file;
    //for testing
    auto path = project_path + file_name;
    auto data = encryptIntoFile();
    file.open(path.c_str());
    file << encryptIntoFile().c_str();
    file.close();
}

void Data::addPage(int page_id)
{
    pages.emplace(pages.begin() + page_id);
}

void Data::CopyPage(int page_id, Page page)
{
    for(int i = 0; i < page.textboxs.size(); i++) {
        page.textboxs.at(i).content = "Enter new text";
    }
    pages.insert(pages.begin() + page_id, page);
}

void Data::deletePage(int page_id)
{
    pages.erase(pages.begin() + page_id);
}