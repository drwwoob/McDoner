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
    Data(const std::string& file_path);
    Data(const std::string &project_path, const char * project_name);
    void openFile(); // reading a file
    //void openFile(std::string path);
    Page* getPage(int page_id); // get the information of a page (decrypt)

	int _page_at;
    void loadTexture();
    void draw();
    void setFont(ImFont* font_given);
    void save();
    int pageSize() { return _pages.size(); };
    void addPage(int page_id);
    void CopyPage(int page_id, Page page);
    void deletePage(int page_id);
    std::string encryptIntoFile(); // encrypt the current pages into text
    void decryptFile(std::string data_str);

   private:
    ImFont* _font;
    std::vector<Page> _pages; // a file includes a collection of pages
    std::string _file_name;
    std::string _project_path;
    std::vector<GLuint> _textures{};
    //std::map<Page> pages;

    // a hash table that stores all pages? or array?

    //void readData(std::string name);
    //from microsoft example code
    // HRESULT basicFileOpen(bool findFile); // The tool to open common window
};