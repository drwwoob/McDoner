/**
 * This class serves presenting the imgui window 
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
	static void showCastsInPage(bool* p_open, Page *pageInfo);
	//void showWelcomePage(bool& showing);
	/**
	 * showing a ImGui window with buttons that each links to Data::NewFile, Data::OpenFile, Data::OpenDemo.
	 * @param gameData the game data currently presenting in main
	 * @param show_welcome_window decide weather to show this window or not
	 * @param page_setting the switch to open Page's ImGui window
	 */
	static void showWelcomePage(Data& gameData, bool& show_welcome_window, bool& page_setting);
	/**
	 * showing a ImGui window that allows the user to change the page they are on, 
	 * and links to Data::addPage, Data::CopyPage, Data::DeletePage, 
	 * with the two functions adding jumping to the newly added page, 
	 * and deletePage to the previous page (if exist). 
	 */
	static void showAmongPages(bool* p_open, int& pageID, Data& game_data);
	
	/**
	 * I hope this window could show the spirit libary including all the spirits that have been used so far
	 */
	static void showLibararySpirit();

private:
	//static int MyResizeCallback(ImGuiInputTextCallbackData* data);
};

