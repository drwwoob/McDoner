#include "data.h"

#include <fstream>

data::data(std::string file_path)
{
	// pop up a window to let the user choose a places to save
	// and let them put in the name for the file
	//QString myDir = QFileDialog::getExistingDirectory();

    // initiate demoPath
    filePath = file_path;

	// create a blank page
	pages.emplace_back(0);

	// save this file with a blank page into the location the user choose
}

void data::newFile(bool* start_visual)
{
    basicFileOpen(false);
    pages = {Page(0)};
}

void data::openFile(bool* start_visual)
{
    basicFileOpen(true);
    decryptFile(fileData);
}

void data::openDemo(bool* start_visual) {
    std::ifstream demoFile(filePath + "demo\\demo.txt");
    if (!demoFile.is_open()) {
        // error window popup
        //ImGui::Begin("Oops");
        //ImGui::OpenPopup("ladojf");
        //ImGui::End();
    }
    this->fileData = "";
    filePath += "demo\\";
    fileName = "demo.txt";

    std::string str;
    while (std::getline(demoFile, str))
    //while(demoFile >> str)
    {
        fileData.append(str);
        fileData.append("\n");
    }
    pages.clear();
    decryptFile(fileData);
    
    *start_visual = true;
}

void data::setFont(ImFont* font_given)
{
    font = font_given;
    //for (int i = 0; i < pages.size(); i++) {
    //    pages.at(i).setFont(font_given);
    //}
}


Page* data::getPage(int page_id) {
	return &pages.at(page_id);
}

std::string data::encryptIntoFile()
{
	std::string pagesInfo;

    for(auto page : pages) {
        pagesInfo.append(page.exportInString());
    }

    return pagesInfo;
}

void data::decryptFile(std::string data_str)
{
    int pos = 0;
    int start = 0;
    int pageID = 0;
    while(pos < data_str.size()) {
	    if(data_str.at(pos++) == '[') { // whether pos is '[', go to the next letter
            start = pos;
            while(data_str.at(pos) != ']') {
                if(data_str.at(pos) == '/') {
                    pos++;
                }
                pos++;
            }
            pages.emplace_back(pageID, data_str.substr(start, pos));
            auto test = data_str.substr(start, pos);
            pages.back().setFont(font);
            pageID++;
            pos++;
	    }
    }
}

void data::save() {
    std::ofstream file;
    //for testing
    auto path = filePath + fileName;
    auto data = encryptIntoFile();
    file.open(path.c_str());
    file << encryptIntoFile().c_str();
    file.close();
}

void data::addPage(int page_id)
{
    pages.emplace(pages.begin() + page_id, page_id);
}

void data::CopyPage(int page_id, Page page)
{
    for(int i = 0; i < page.textboxs.size(); i++) {
        page.textboxs.at(i).content = "Enter new text";
    }
    pages.insert(pages.begin() + page_id, page);
}

void data::deletePage(int page_id)
{
    pages.erase(pages.begin() + page_id);
}