#pragma once

#include <string>
#include <vector>
#include "Spirit.h"

#include <codecvt>
#include <locale>

#include "Textbox.h"
#include <functional>

class Page {
public:
	Page();
	Page(std::string page_data);
	//Page(int page_id, std::vector<std::string> spirits_vec, std::vector<std::string> textboxs_vec);
    /**
     *  putting all the information in this page into a string
     * @return 
     *  [
     *      {spirit1##spirit2##spirit3##}
     *      {textbox1##textbox2##}
     *      {order1first#order1second##order2first#order2second##}
     *  ]
     */
	std::string exportInString();

	Spirit* getRealSpirits(int id);
	Textbox* getRealTextbox(int id);
	void setFont(ImFont* font_given);
    std::vector<GLuint> loadPage(const std::string &project_path);
    void drawPage(std::vector<GLuint> &textures);
	std::vector<Spirit> _spirits;
	std::vector<Textbox> _textboxs;

private:
    void decrypt(const std::string &data_block, int size, const std::function<void(std::vector<std::string>&)> &func);
    void loadImageTexture(std::string &name, std::vector<GLuint> &textures);
	
    /**
     * containing the order of the element rendering (on top of another)
     * naming rule: 
     *      < 0, "background" > = background
     *      < 1, "filename" > = spirit with the given file name
     *      < 2, "nickname" > = textbox with the give nickname
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