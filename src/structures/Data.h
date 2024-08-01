#pragma once
#include <map>
#include "Page.h"
#include <Tools.h>
//#include "shobjidl_core.h"

class Data {
   public:
    /**
     * creating a blank path
     * @overload constructor for a blank project
     */
    Data();
    /**
     * read the project with given path
     * @overload constructor, reading from given file
     * @param file_path string of the given file path
     */
    Data(const std::string& file_path, int& page_at);
    void newFile();
    void openFile(); // reading a file
    //void openFile(std::string path);
    Page* getPage(int page_id); // get the information of a page (decrypt)

    void loadTexture(int page_at);
    void draw(int page_at);
    void setFont(ImFont* font_given);
    void save(int& page_at);
    int pageSize() { return pages.size(); };
    void addPage(int page_id);
    void CopyPage(int page_id, Page page);
    void deletePage(int page_id);
    std::string encryptIntoFile(int &page_at); // encrypt the current pages into text
    void decryptFile(std::string data_str, int& page_at);

   private:
    ImFont* font;
    std::vector<Page> pages; // a file includes a collection of pages
    std::string file_name;
    std::string project_path;
    std::vector<GLuint> textures{};
    //std::map<Page> pages;

    // a hash table that stores all pages? or array?

    //void readData(std::string name);
    //from microsoft example code
    // HRESULT basicFileOpen(bool findFile); // The tool to open common window
};