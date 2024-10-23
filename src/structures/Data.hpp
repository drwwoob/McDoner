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
    ~Data();

    Page* getPage(int page_id); // get the information of a page (decrypt)
    Page* getCurrentPage(); // get the page currently on

	int _page_at;
    void loadTexture();
    void draw(const bool edit_mode);
    void showEdit();
    void setFont(ImFont* font_given);
    void save() const;
    void loadBinary();
    void loadString();
    void exportToString();
    int pageSize() { return _pages.size(); };
    void addPage(int page_id, Page page = Page(_library_ptr));
    void CopyPage(int page_id, Page page);
    void deletePage(int page_id);
    std::string encryptIntoFile(); // encrypt the current pages into text
    void decryptFile(const std::string& data_str);

    void importFormattedPages(const std::string& path);
    
    void changeProjectName();
    std::string _project_name;
    std::string _project_path;



   private:
    ImFont* _font;
    std::vector<Page> _pages; // a file includes a collection of pages
    std::vector<GLuint> _textures{};
    void loadSettings();
    void loadFormattedPages();
    using ParameterVarient = std::variant<
        int,
        double,
        bool,
        std::string,
        Page*
    >;
    std::map<std::string, ParameterVarient> _values; // name, initial value
    // std::shared_ptr<std::vector<Pages>> _library_ptr; // not sure if i should 
    static const std::shared_ptr<Library> _library_ptr;
};