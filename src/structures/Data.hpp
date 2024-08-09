#pragma once
#include <map>
#include "Page.hpp"
#include "Tools.hpp"
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
    /**
     * creating new project
     * @overload constructor, creating a project
     * @param project_path the path to store the project
     * @param project_name the name for the porject
     */
    Data(const std::string &project_path, const char * project_name);
    std::unique_ptr<Page> getPage(int page_id); // get the information of a page (decrypt)

	int _page_at;
    void loadTexture();
    void draw();
    void setFont(ImFont* font_given);
    void save();
    int pageSize() { return _pages.size(); };
    void addPage(int page_id, Page page = {});
    void CopyPage(int page_id, Page page);
    void deletePage(int page_id);
    std::string encryptIntoFile(); // encrypt the current pages into text
    void decryptFile(const std::string& data_str);
    void ImportFormattedPages(const std::string& path);
    void changeProjectName();
    void replace_all();

   private:
    ImFont* _font;
    std::vector<Page> _pages; // a file includes a collection of pages
    std::string _file_name;
    std::string _project_path;
    std::vector<GLuint> _textures{};
    void loadSettings();
    void loadFormattedPages();
    using ParameterVarient = std::variant<
        int,
        double,
        bool,
        std::string
    >;
    std::map<std::string, ParameterVarient> _values; // name, initial value

    // library storage
    std::map<std::string, Page> _pages_library;  // <name, page>
    std::vector<Button> _button_Library; // Button
    std::vector<Spirit> _spirit_Library; // Spirit
    std::vector<Textbox> _textbox_Library; // Textbox
};