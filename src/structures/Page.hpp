#pragma once

#include <string>
#include <vector>
#include "Spirit.hpp"

#include <codecvt>
#include <locale>
#include "Button.hpp"
#include "Textbox.hpp"
#include <functional>
#include "Library.hpp"

class Page {
public:
    /**
     * Page constuctor without linking to a library
     */
    Page();
    /**
     * Page constructor linking to a library
     */
	Page(const std::shared_ptr<Library>& library_ptr);
    /**
     * @param page_data the string of the data
     * @param library_ptr the pointer to the library object
     */
	Page(const std::string& page_data, const std::shared_ptr<Library>& library_ptr);
	//Page(int page_id, std::vector<std::string> spirits_vec, std::vector<std::string> textboxs_vec);
    // Page& operator=(const Page& other);
    /**
     *  putting all the information in this page into a string
     * @return 
     *  [
     *      {spirit1##spirit2##spirit3##}
     *      {textbox1##textbox2##}
     *      {button1##button2##}
     *      {formattedPage1_name#formattedPage2_name#}
     *      {order1first#order1second##order2first#order2second##}
     *  ]
     */
	std::string encrpyt();
	Spirit* getRealSpirits(const int id);
	Textbox* getRealTextbox(const int id);
	void setFont(ImFont* font_given);
    std::vector<GLuint> loadPage(const std::string &project_path);
    void drawPage(const std::vector<GLuint> &textures, int size);
    void replace();
    std::shared_ptr<Library> _library_ptr;
	std::vector<Spirit> _spirits;
    std::vector<std::shared_ptr<Spirit>> _spirit_ptrs;
	std::vector<Textbox> _textboxs;
    std::vector<Button> _buttons;
    std::vector<std::shared_ptr<Button>> _button_ptrs;
    std::map<std::string, std::shared_ptr<Page>> _format_pages_ptrs; // name, formatted page's pointer

    void serialize(std::ofstream& outFile) const;
    void deserialize(std::ifstream& inFile);
    
private:
    void loadImageTexture(const std::string &name, std::vector<GLuint> &textures);
	
    /**
     * containing the order of the element rendering (on top of another)
     * naming rule: 
    //  *      < 0, "background" > = background
     *      < 1, "filename" > = spirit with the given file name
     *      < 2, "nickname" > = textbox with the give nickname
     *      < 3, "nickname" > = button with the given nickname
     *      < 4, "name"> = formatted page with the given name
     *      < 5, "filename" > = spirit pointer to the library object with the given file name
     *      < 6, "nickname" > = textbox pointer to the library object with the give nickname
     *      < 7, "nickname" > = button pointer to the library object with the given nickname
    //  *      < 4, "filename" > = spirit from formatting page with given file name
    //  *      < 5, "nickname" > = textbox from formatting page with the give nickname
     */
    std::vector<std::pair<int, std::string>> _draw_order;
	// std::vector<std::string> textboxs settings;
	//std::string path;
	//static std::string wordEncrypt(std::string word);

	/*exceptions:
	 *	# -> /#
	 *	/ -> //
	 *	{ -> /{
	 *	} -> /}
	 *	[ -> /[
	 *	] -> /]
	*/
};
