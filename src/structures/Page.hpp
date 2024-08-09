#pragma once

#include <string>
#include <vector>
#include "Spirit.hpp"

#include <codecvt>
#include <locale>
#include "Button.hpp"
#include "Textbox.hpp"
#include <functional>
#include "ButtonInterface.hpp"

class Page {
public:
	Page();
	Page(const std::string& page_data);
	//Page(int page_id, std::vector<std::string> spirits_vec, std::vector<std::string> textboxs_vec);
    /**
     *  putting all the information in this page into a string
     * @return 
     *  [   _button_interface_id,
     *      {spirit1##spirit2##spirit3##}
     *      {textbox1##textbox2##}
     *      {button1##button2##}
     *      {order1first#order1second##order2first#order2second##}
     *  ]
     */
	std::string encrpyt();
	Spirit* getRealSpirits(const int id);
	Textbox* getRealTextbox(const int id);
	void setFont(ImFont* font_given);
    std::vector<GLuint> loadPage(const std::string &project_path);
    void drawPage(const std::vector<GLuint> &textures);
    void replace();
	std::vector<Spirit> _spirits;
	std::vector<Textbox> _textboxs;
    std::vector<Button> _buttons;
    std::map<std::string, std::shared_ptr<Page>> _format_pages_ptrs; // name, formatted page's pointer

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