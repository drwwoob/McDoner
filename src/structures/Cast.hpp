/**
 * This class serves for presenting the imgui window
 * for modification on the data.
 */

#ifndef CAST_HPP
#define CAST_HPP

#pragma once
#include "Data.hpp"
#include <stack>
#include <unordered_map>
#include "Backup.hpp"
#include "../export/ExportWriter.hpp"
#include <set>

class Cast {
public:
	Cast(
		std::shared_ptr<std::shared_ptr<Data>> game_data_ptr,
		std::unique_ptr<Backup> backup_data,
		std::unique_ptr<Page> clipboard_page_ptr,
		std::unique_ptr<std::map<std::string, bool>> window_triggers
		);
	~Cast();
	/**
	 * the main menu bar for the host window
	 * @param game_data the game data currently presenting in main
	 */
	void showMenuBar(Page& clipboard_page);
	/**
	 *
	 */
	void showCastsInPage();
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
	void showWelcomePage(bool& show_welcome_window);
	/**
	 * showing a ImGui window that allows the user to change the page they are on,
	 * and links to Data::addPage, Data::CopyPage, Data::DeletePage,
	 * with the two functions adding jumping to the newly added page,
	 * and deletePage to the previous page (if exist).
	 */
	void showAmongPages();

	/**
	 * I hope this window could show the spirit libary including all the spirits that have been used so far
	 */
	void showLibararySpirit();

	void loadIcon() {
	}
	void showElementWindow();

private:

template <typename T>
	void showDetailElement(T element, bool* p_open);
	// void showDetailElement(std::optional<T*> element, bool* p_open);
using ElementVariant = std::variant<
	Spirit*, 
	Textbox*, 
	Button*, 
	Page*>;
	std::set<ElementVariant> _element_detail_list;
// 	void showDetailElement(ElementVariant element);

	std::shared_ptr<std::shared_ptr<Data>> _game_data_ptr;
	std::unique_ptr<Backup> _backup_data;
	std::unique_ptr<Page> _clipboard_page_ptr;
	std::unique_ptr<std::map<std::string, bool>> _window_triggers;
	std::unordered_map<std::string, std::string> _shortkey_outlay;
	std::unordered_map<std::string, std::string> _menu_language;

	// std::set<std::pair<int, std::string>> _element_detail_list;
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

	/**
	 * showing spirit in treenode
	 * 
	 */
	void spiritNodeContent(Spirit& spirit, bool linked, const bool linkable, const std::optional<bool> name_changable = false, const std::optional<std::string>& name = "spirit_default");
	// void spiritNodeContent(Spirit& spirit, bool &linked, const bool linkable, const std::optional<bool> name_changable = false, const std::optional<std::string>& name = "spirit_default");
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
	std::string importImage();
	// open a window showing the imported images...?
	void showLibraryImage();

	/**
	 * an imgui window showing all library spirits
	 */
	void showLibrarySpirit();

	void addSpiritTreeNode(Spirit& spirit, const std::optional<std::string>& name = std::nullopt);

	/**
	 * @param map_id
	 * 	0: _shortkey_outlay
	 *  1: _menu_language
	 */
	const char* getMapItem(int map_id, const std::string& key);
	void loadLanguageMap(const std::string& lang_id);
};

#include "Cast.tpp"

#endif