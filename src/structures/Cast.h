/**
 * This class serves for presenting the imgui window 
 * for modification on the data.
 */
#pragma once
#include <Data.h>
class Cast
{
public:
	/**
	 * 
	 */
	static void showMenuBar();
	/**
	 * 
	 */
	static void showCastsInPage(bool* p_open, Page *pageInfo);
	//void showWelcomePage(bool& showing);
	/**
	 * showing a ImGui window with buttons that each links to Data::NewFile, Data::OpenFile, Data::OpenDemo.
	 * @param game_data the game data currently presenting in main
	 * @param show_welcome_window decide weather to show this window or not
	 * @param page_setting the switch to open Page's ImGui window
	 * @param page_at the page the host window's showing
	 * @note I think that if i am implementing the function to "open at last close", then I need to pass page_at in here and change it here and load the texture here.
	 * 	Currently i did implemented, just that since page_at is not tested so this feature is an unknow
	 */
	static void showWelcomePage(Data& game_data, bool& show_welcome_window, bool& page_setting, int &page_at);
	/**
	 * showing a ImGui window that allows the user to change the page they are on, 
	 * and links to Data::addPage, Data::CopyPage, Data::DeletePage, 
	 * with the two functions adding jumping to the newly added page, 
	 * and deletePage to the previous page (if exist). 
	 */
	static void showAmongPages(bool* p_open, int& page_at, Data& game_data);
	
	/**
	 * I hope this window could show the spirit libary including all the spirits that have been used so far
	 */
	static void showLibararySpirit();

	static void loadIcon(){
		
	}

private:
	static std::vector<GLuint> _icon_textures{};
};

