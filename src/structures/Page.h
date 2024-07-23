#pragma once

#include <string>
#include <vector>
#include "Spirit.h"
#pragma comment (lib,"Gdiplus.lib")

#include <codecvt>
#include <locale>

#include "Textbox.h"

class Page {
public:
	Page();
	Page(std::string page_data);
	//Page(int page_id, std::vector<std::string> spirits_vec, std::vector<std::string> textboxs_vec);
    /**
     *  putting all the information in this page into a string
     * @return pageID[backgroundName{spirit1##spirit2##spirit3##}{textbox1##textbox2##}]
     */
	std::string exportInString();

	Spirit* getRealSpirits(int id);
	Textbox* getRealTextbox(int id);
	void setFont(ImFont* font_given);
    static void showSpirit(Spirit spirit, ImVec2 window_size);
	static void showTextbox(Textbox textbox, ImVec2 window_size);
	
	//void freeTexture();
	/* in the format of
	[backgroundName{spirit1##spirit2##spirit3##}{textbox1##textbox2##}][backgroundName{spirit1##spirit2##spirit3##}{textbox1##textbox2##}]
	*/
	std::vector<Spirit> spirits;
	std::vector<Textbox> textboxs;
	std::string	backgroundName;

private:
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