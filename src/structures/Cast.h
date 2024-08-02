/**
 * This class serves for presenting the imgui window
 * for modification on the data.
 */
#pragma once
#include <Data.h>
#include <stack>
#include "Backup.h"
class Cast {
public:
	Cast(std::shared_ptr<std::shared_ptr<Data>> game_data_ptr);
    ~Cast();
    /**
	 * the main menu bar for the host window
	 * @param game_data the game data currently presenting in main
	 */
    void showMenuBar(Page &clipboard_page, Backup &backup_data);
    /**
	 *
	 */
    void showCastsInPage(bool* p_open);
    // void showWelcomePage(bool& showing);
    /**
	 * showing a ImGui window with buttons that each links to Data::NewFile, Data::OpenFile, Data::OpenDemo.
	 * @param game_data the game data currently presenting in main
	 * @param show_welcome_window decide weather to show this window or not
	 * @param page_setting the switch to open Page's ImGui window
	 * @param page_at the page the host window's showing
	 * @note I think that if i am implementing the function to "open at last close", then I need to pass page_at in here and change it here and load the texture here.
	 * 	Currently i did implemented, just that since page_at is not tested so this feature is an unknow
	 */
    void showWelcomePage(bool& show_welcome_window, bool& page_setting);
    /**
	 * showing a ImGui window that allows the user to change the page they are on,
	 * and links to Data::addPage, Data::CopyPage, Data::DeletePage,
	 * with the two functions adding jumping to the newly added page,
	 * and deletePage to the previous page (if exist).
	 */
    void showAmongPages(bool* p_open);

    /**
	 * I hope this window could show the spirit libary including all the spirits that have been used so far
	 */
    void showLibararySpirit();

    void loadIcon() {
    }

private:
	std::shared_ptr<std::shared_ptr<Data>> game_data_ptr;
    // functions for different cast members, all written here so the the functions (and bugs) sync
    /**
	 *
	 */
    void lastPage();
    void nextPage();
    void addPage();
    void duplicatePage();
    void deletePage();
    void copyPage(Page& clipboard_page);
};
