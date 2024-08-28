/**
 * This class serves for presenting the imgui window
 * for modification on the data.
 */
#pragma once
#include "Data.hpp"
#include <stack>
#include <unordered_map>
#include "Backup.hpp"
#include "../export/ExportWriter.hpp"

class Cast {
public:
	Cast(
		std::shared_ptr<std::shared_ptr<Data>> game_data_ptr,
		std::unique_ptr<Backup> backup_data,
		std::unique_ptr<Page> clipboard_page_ptr
	);
    ~Cast();
    /**
	 * the main menu bar for the host window
	 * @param game_data the game data currently presenting in main
	 */
    void showMenuBar(Page &clipboard_page);
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
	std::shared_ptr<std::shared_ptr<Data>> _game_data_ptr;
	std::unique_ptr<Backup> _backup_data;
	std::unique_ptr<Page> _clipboard_page_ptr;
	std::unordered_map<std::string, std::string> _shortkey_outlay;
	std::unordered_map<std::string, std::string> _menu_language;
    // functions for different cast members, all written here so the the functions (and bugs) sync
    /**
	 *
	 */
	void imguiWindowManipulateBar(ImVec4* backgroundColor);

    void lastPage();
    void nextPage();
    void addPage();
    void duplicatePage();
    void deletePage();
    void copyPage();
	bool newProject();
	bool openProject();
	void askForSave();

    void spiritTreeNode(Spirit& spirit, bool linked, const std::optional<bool> name_changable = false);
	void textboxTreeNode(Textbox& textbox);
	void buttonTreeNode(Button& button);

	// button features
	/**
	 * letting the user to create a value, and store this value into data
	 * @details the value can be in type int, double, bool or std::string
	 */
	void createValue();
	void addTrigger();

	// =========== todo =======================
	void importImage();
	// open a window showing the imported images...?
	void showLibraryImage();
	void showLibrarySpirit();


	void addSpiritTreeNode(Spirit& spirit, const std::optional<std::string>& name = std::nullopt);

	/**
	 * @param map_id
	 * 	0: _shortkey_outlay
	 *  1: _menu_language
	 */
	const char* getMapItem(int map_id, const std::string& key);
    void loadLanguageMap(const std::string &lang_id);
};
