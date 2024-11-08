#include "Cast.hpp"
#include <imgui_stdlib.h>
#include <unordered_map>
#include "Tools.hpp"
#include <tinyfiledialogs.h>
#include <filesystem>
#include <variant>
// #include <opencv2/opencv.hpp>
// #include <string>
// #include "data.h"

typedef void (*ImGuiMarkerCallback)(const char* file, int line, const char* section, void* user_data);
extern ImGuiMarkerCallback GImGuiMarkerCallback;
extern void* GImGuiMarkerCallbackUserData;
ImGuiMarkerCallback GImGuiMarkerCallback = NULL;
void* GImGuiMarkerCallbackUserData = NULL;
#define IMGUI_MARKER(section)                                                                \
	do {                                                                                     \
		if(GImGuiMarkerCallback != NULL)                                                     \
			GImGuiMarkerCallback(__FILE__, __LINE__, section, GImGuiMarkerCallbackUserData); \
	} while(0)

Cast::Cast(std::shared_ptr<
			   std::shared_ptr<Data>>
			   game_data_ptr,
		   std::unique_ptr<Backup> backup_data,
		   std::unique_ptr<Page> clipboard_page_ptr,
		   std::unique_ptr<std::map<std::string, bool>> window_triggers)
	: _game_data_ptr(game_data_ptr),
	  _backup_data(std::move(backup_data)),
	  _clipboard_page_ptr(std::move(clipboard_page_ptr)),
	  _window_triggers(std::move(window_triggers)) {
	// listing out the menu shortkey items
	auto getShortKeys = [](std::unordered_map<std::string, std::string>& shortkey_map, nlohmann::json& keyBindings) {
		std::array<std::string, 7> keys{
			"New",
			"Open",
			"Save",
			"Import",
			"Exit",
			// "Edit",
			"Undo",
			"Redo",
			// "Scene",
			// "Last Page",
			// "Next Page",
		};
		for(auto& key : keys) {
#ifdef __APPLE__
			shortkey_map.emplace(key, keyBindings.at(key).at("Mac").get<std::string>());
#else
			shortkey_map.emplace(key, keyBindings.at(key).at("Windows").get<std::string>());
#endif
		}
	};

	_shortkey_outlay = Tools::loadJson("../src/settings/keyLoad.json", getShortKeys);

	// listing out the language options
	std::array<std::string, 2> language_keys{
		"EN",
		"CH"};
}
Cast::~Cast() {}

void Cast::showMenuBar(Page& clipboard_page) {
	if(ImGui::BeginMainMenuBar()) {
		if(ImGui::BeginMenu("File")) {
			if(ImGui::MenuItem("New", getMapItem(0, "New"))) {
				newProject();
			}
			// Add items to the "File" menu
			if(ImGui::MenuItem("Open", getMapItem(0, "Open"))) {
				openProject();
			}
			if(ImGui::MenuItem("Save", getMapItem(0, "Save"))) {
				// Handle "Save" action
				(*_game_data_ptr)->save();
			}
			// if(ImGui::MenuItem("Import", getMapItem(0, "Import"))) {
			// }
			// if(ImGui::MenuItem("Export", getMapItem(0, "Export"))){
			// 	// this is going to have a few options, 
			// 	// including export as game, script or formatted txt
			// }
			if(ImGui::MenuItem("Exit", getMapItem(0, "Exit"))) {
				// Handle "Exit" action
			}
			ImGui::EndMenu();
		}

		ImGui::SameLine();
		if(ImGui::BeginMenu("Edit")) {
			bool disabled = false;
			if(!_backup_data->undoAvailible()) {
				disabled = true;
				ImGui::BeginDisabled();
			}
			if(ImGui::MenuItem("Undo", getMapItem(0, "Undo"))) {
				_backup_data->undo();
			}
			if(disabled) {
				ImGui::EndDisabled();
				disabled = false;
			}

			if(!_backup_data->redoAvailible()) {
				disabled = true;
				ImGui::BeginDisabled();
			}
			if(ImGui::MenuItem("Redo", getMapItem(0, "Redo"))) {
				_backup_data->redo();
			}
			if(disabled) {
				ImGui::EndDisabled();
				disabled = false;
			}
			ImGui::EndMenu();
		}

		ImGui::SameLine();
		if(ImGui::BeginMenu("Scene")) {
			bool disabled = false;

			if((*_game_data_ptr)->_page_at == 0) {
				disabled = true;
				ImGui::BeginDisabled();
			}
			if(ImGui::MenuItem("last page", "Ctrl+K")) {
				lastPage();
			}
			if(disabled) {
				ImGui::EndDisabled();
				disabled = false;
			}

			if((*_game_data_ptr)->_page_at >= (*_game_data_ptr)->pageSize() - 1) {
				disabled = true;
				ImGui::BeginDisabled();
			}
			if(ImGui::MenuItem("next page", "Ctrl+L")) {
				nextPage();
			}
			if(disabled) {
				ImGui::EndDisabled();
				disabled = false;
			}

			if((*_game_data_ptr)->pageSize() == 1) {
				disabled = true;
				ImGui::BeginDisabled();
			}
			if(ImGui::MenuItem("delete current page")) {
				deletePage();
			}
			if(disabled) {
				ImGui::EndDisabled();
				disabled = false;
			}

			if(ImGui::MenuItem("add blank page")) {
				addPage();
			}
			if(ImGui::MenuItem("duplicate page")) {
				duplicatePage();
			}
			if(ImGui::MenuItem("copy page")) {
				copyPage();
			}
			if(ImGui::MenuItem("cut page")) {
			}
			if(ImGui::MenuItem("past page")) {
			}
			if(ImGui::MenuItem("window")) {
			}
			if(ImGui::MenuItem("bookmark mark")) {
			}
			if(ImGui::MenuItem("bookmark list")) {
			}
			ImGui::EndMenu();
		}
		ImGui::SameLine();
		if(ImGui::BeginMenu("Cast")) {
			if(ImGui::MenuItem("List")) {
			}
			if(ImGui::BeginMenu("Spirit")) {
				if(ImGui::MenuItem("Import")) {
					importImage();
				}
				if(ImGui::MenuItem("Add from list")) {
				}
				if(ImGui::MenuItem("Delete")) {
				}
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Textbox")) {
				if(ImGui::MenuItem("New")) {
				}
				if(ImGui::MenuItem("Add from list")) {
				}
				if(ImGui::MenuItem("Delete")) {
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::SameLine();
		if(ImGui::BeginMenu("View")) {
			if(ImGui::MenuItem("Starting Page")) {
			}
			if(ImGui::MenuItem("Page Gallary")) {
			}
			ImGui::EndMenu();
		}
		ImGui::SameLine();
		if(ImGui::BeginMenu("Run")) {
			if(ImGui::MenuItem("Run New Game")) {
			}
			if(ImGui::MenuItem("Run From Here")) {
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Cast::showCastsInPage() {
	ImGuiWindowFlags window_flags = 0;
	auto page_info = (*_game_data_ptr)->getPage((*_game_data_ptr)->_page_at);

	ImGui::Begin("Cast", &_window_triggers->at("show_cast_window"), window_flags);

	// const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	// ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 100, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(200, 20), ImGuiCond_FirstUseEver);
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	for(auto& draw_item : std::ranges::reverse_view((*_game_data_ptr)->getCurrentPage()->_draw_order)) {
		switch(draw_item.first) {
		case 1:
			for(auto& spirit : (*_game_data_ptr)->getCurrentPage()->_spirits) {
				if(spirit._spirit_file_name == draw_item.second) {
					if(ImGui::Button(spirit._spirit_name.c_str(), ImVec2(200, 20))) {
						// showDetailElement(std::optional<Spirit*>(&spirit));
						_element_detail_list.emplace(&spirit);
					}
					// if(ImGui::CollapsingHeader(spirit._spirit_name.c_str()/**, ImGuiTreeNodeFlags_DefaultOpen**/)) {
					// 	spiritNodeContent(spirit, false, true, true);
					// }
					break;
				}
			}
			break;
		case 2:
			for(auto& textbox : (*_game_data_ptr)->getCurrentPage()->_textboxs) {
				if(textbox._name == draw_item.second) {
					if(ImGui::Button(textbox._name.c_str(), ImVec2(200, 20))) {
						_element_detail_list.emplace(&textbox);
					}
					// 	textboxTreeNode(textbox);
					break;
				}
			}
			break;
		case 3:
			for(auto& button : (*_game_data_ptr)->getCurrentPage()->_buttons) {
				if(button._nickname == draw_item.second) {
					// 	buttonTreeNode(button);
					if(ImGui::Button(button._nickname.c_str(), ImVec2(200, 20))) {
						_element_detail_list.emplace(&button);
					}
					draw_item.second = button._nickname;
					break;
				}
			}
			break;
		default:
			break;
		}
	}

	// // ================ the drawing with the drawing order based on types ==================
	// ImGui::Text("This is the list of cast on the current page");
	// IMGUI_MARKER("Spirit");
	// // ImGui::SetNextWindowCollapsed(false);
	// if(ImGui::CollapsingHeader("Spirit", ImGuiTreeNodeFlags_DefaultOpen)) {
	// 	// for there i think it'll be best to do a draw-order listing so that nothing changes after setting linked / unlinked

	// 	// ImGui::SeparatorText("Unlinked:");
	// 	for(auto& spirit : page_info->_spirits) {
	// 		spiritNodeContent(spirit, false, true);
	// 	}

	// 	// 	ImGui::SeparatorText("Linked:");
	// 	// 	for(auto& spirit : page_info->_spirit_ptrs) {
	// 	// 		spiritNodeContent(*spirit, true);
	// 	// 	}
	// }
	// IMGUI_MARKER("Textbox");
	// if(ImGui::CollapsingHeader("Textbox", ImGuiTreeNodeFlags_DefaultOpen)) {
	// 	for(auto& textbox : page_info->_textboxs) {
	// 		textboxTreeNode(textbox);
	// 	}
	// }

	// IMGUI_MARKER("Button");
	// if(ImGui::CollapsingHeader("Button", ImGuiTreeNodeFlags_DefaultOpen)) {
	// 	for(auto& button : page_info->_buttons) {
	// 		buttonTreeNode(button);
	// 	}
	// }

	// // IMGUI_MARKER("Background");
	// // if(ImGui::CollapsingHeader("Background", ImGuiTreeNodeFlags_DefaultOpen)) {
	// // 	// choose the color for background here
	// // }

	ImGui::End();
}

void Cast::showWelcomePage(bool& show_welcome_window) {
	ImGui::SetNextWindowSize(ImVec2(240, 300));
	ImGui::Begin("Welcome Page", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	if(ImGui::Button("New", ImVec2(-FLT_MIN, 80))) {
		if(newProject()) {
			show_welcome_window = false;
			_window_triggers->at("page_setting") = true;
		}
	}
	if(ImGui::Button("Open", ImVec2(-FLT_MIN, 80))) {
		if(openProject()) {
			show_welcome_window = false;
			_window_triggers->at("page_setting") = true;
		}
	}
	if(ImGui::Button("Demo", ImVec2(-FLT_MIN, 80))) {
		*_game_data_ptr = std::make_shared<Data>("../saves/demo/demo.txt");
		show_welcome_window = false;
		_window_triggers->at("page_setting") = true;
		// =========== todo: here is for saved (*game_data_ptr)->page_at thingy ===============
		// (*game_data_ptr)->page_at = 0;
		(*_game_data_ptr)->loadTexture();
		// std::cout << "loaded texture" << std::endl;
	}
	ImGui::End();
}

void Cast::showAmongPages() {
	ImGui::Begin("Page Setting");

	bool disabled = false;

	if((*_game_data_ptr)->_page_at == 0) {
		disabled = true;
		ImGui::BeginDisabled();
	}
	if(ImGui::Button("Last Page", ImVec2(100, 100))) {
		lastPage();
	}
	if(disabled) {
		ImGui::EndDisabled();
		disabled = false;
	}

	ImGui::SameLine();

	if((*_game_data_ptr)->_page_at >= (*_game_data_ptr)->pageSize() - 1) {
		disabled = true;
		ImGui::BeginDisabled();
	}
	if(ImGui::Button("Next Page", ImVec2(100, 100))) {
		nextPage();
	}
	if(disabled) {
		ImGui::EndDisabled();
		disabled = false;
	}

	if(ImGui::Button("add Page", ImVec2(50, 50))) {
		addPage();
	}

	ImGui::SameLine();

	if(ImGui::Button("duplicate Page", ImVec2(50, 50))) {
		duplicatePage();
	}

	ImGui::SameLine();

	if((*_game_data_ptr)->pageSize() == 1) {
		disabled = true;
		ImGui::BeginDisabled();
	}
	if(ImGui::Button("delete Page", ImVec2(50, 50))) {
		deletePage();
	}
	if(disabled) {
		ImGui::EndDisabled();
		disabled = false;
	}

	ImGui::SameLine();

	if(ImGui::Button("Cut Page", ImVec2(50, 50))) {
	}

	ImGui::SameLine();

	if(ImGui::Button("Copy Page", ImVec2(50, 50))) {
		copyPage();
	}

	ImGui::SameLine();

	if(ImGui::Button("Paste Page", ImVec2(50, 50))) {
	}

	ImGui::SameLine();

	ImGui::End();
}

// ============ why don't i do variant on this ==============
void Cast::showElementWindow() {
	std::set<ElementVariant> closing_list;
	// std::set<std::pair<int, std::string>> changing_list;

	for(const auto& element : _element_detail_list) {
		// auto name = element.second + " Detail";
		bool p_open = true;

		// Pass p_open by reference to modify its value
		std::visit([this, &p_open](auto&& arg) {
			showDetailElement(arg, &p_open); // Pass p_open as a pointer to be modified
		},element);

		if(!p_open) {
			closing_list.emplace(element); // Add element to closing_list if p_open is false
		}
	}

	// Remove closed elements from _element_detail_list
	for(const auto& closed_element : closing_list) {
		_element_detail_list.erase(closed_element);
	}
}

void Cast::lastPage() {
	_backup_data->addMove();
	(*_game_data_ptr)->_page_at = (*_game_data_ptr)->_page_at - 1;
	(*_game_data_ptr)->loadTexture();
	_element_detail_list.clear();
}
void Cast::nextPage() {
	_backup_data->addMove();
	(*_game_data_ptr)->_page_at = (*_game_data_ptr)->_page_at + 1;
	(*_game_data_ptr)->loadTexture();
	_element_detail_list.clear();
}
void Cast::addPage() {
	_backup_data->addMove();
	(*_game_data_ptr)->_page_at = (*_game_data_ptr)->_page_at + 1;
	(*_game_data_ptr)->addPage((*_game_data_ptr)->_page_at);
	(*_game_data_ptr)->loadTexture();
	_element_detail_list.clear();
}
void Cast::duplicatePage() {
	_backup_data->addMove();
	(*_game_data_ptr)->CopyPage((*_game_data_ptr)->_page_at + 1, *((*_game_data_ptr)->getPage((*_game_data_ptr)->_page_at)));
	(*_game_data_ptr)->_page_at++;
	(*_game_data_ptr)->loadTexture();
	_element_detail_list.clear();
}
void Cast::deletePage() {
	_backup_data->addMove();
	(*_game_data_ptr)->deletePage((*_game_data_ptr)->_page_at);
	if((*_game_data_ptr)->_page_at != 0) {
		(*_game_data_ptr)->_page_at--;
		(*_game_data_ptr)->loadTexture();
	}
}
void Cast::copyPage() {
	_backup_data->addMove();
	*_clipboard_page_ptr = *(*_game_data_ptr)->getPage((*_game_data_ptr)->_page_at);
};

bool Cast::newProject() {
	auto lTheSelectFolderName = tinyfd_selectFolderDialog(
		"create new project in", "../saves/");

	if(!lTheSelectFolderName) {
		return false;
	}
	auto lTheProjectName = tinyfd_inputBox(
		"project naming", "please give a name to your project", "project");

	if(!lTheProjectName) {
		return false;
	}

	std::string path = lTheSelectFolderName;
	path += lTheProjectName;
	if(std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
		tinyfd_messageBox(
			"Error",
			"project already exist",
			"ok",
			"error",
			1);
		return false;
	}
	askForSave();
	*_game_data_ptr = std::make_shared<Data>(path, lTheProjectName);
	(*_game_data_ptr)->save();
	return true;
}

bool Cast::openProject() {
	auto lTheSelectFolderName = tinyfd_selectFolderDialog(
		"select project", "../saves/");

	if(!lTheSelectFolderName) {
		return false;
	}

	std::string path = lTheSelectFolderName;
	std::string project_name = path.substr(
		path.substr(0, path.size() - 1).find_last_of("/") + 1);
	project_name = project_name.substr(0, project_name.size() - 1);
	path += project_name;
	path += ".bin";
	if(!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path)) {
		tinyfd_messageBox(
			"Error",
			"project does not exist",
			"ok",
			"error",
			1);
		return false;
	}

	askForSave();

	*_game_data_ptr = std::make_shared<Data>(path);
	(*_game_data_ptr)->loadTexture();
	_backup_data->addMove();
	return true;
}

void Cast::askForSave() {
	if(*_game_data_ptr && (*_game_data_ptr)->pageSize() != 0) {
		auto save_stat = tinyfd_messageBox(
			"Save", "do you wish to save your current project?", "yesno", "question", 1);
		if(save_stat) {
			(*_game_data_ptr)->save();
		}
	}
}

const char* Cast::getMapItem(int map_Id, const std::string& key) {
	switch(map_Id) {
	case 0:
		return _shortkey_outlay.at(key).c_str();
		break;
	case 1:
		return _menu_language.at(key).c_str();
		break;
	default:
		return "";
	}
}

// void Cast::spiritNodeContent(Spirit& spirit, bool &linked, const bool linkable, const std::optional<bool> name_changable, const std::optional<std::string>& name) {
void Cast::spiritNodeContent(Spirit& spirit, bool linked, const bool linkable, const std::optional<bool> name_changable, const std::optional<std::string>& name) {
	// ImGui::SetNextWindowCollapsed(false);
	if(spirit._spirit_name != "") {
	}
	else {
		spirit._spirit_name = *name;
	}
	IMGUI_MARKER(spirit._spirit_name.c_str());

	// ================== how do i put *** UNDO *** in this...?

	// if(ImGui::TreeNode(spirit._spirit_name.c_str())) {
	auto nameStr = &spirit._spirit_name;
	if(name_changable.has_value() && name_changable == true) {
		auto renameLabel = "rename##" + spirit._spirit_name;
		auto nameStrChanging = *nameStr;
		ImGui::InputText(renameLabel.c_str(), &nameStrChanging);
		if(nameStrChanging != "") {
			*nameStr = nameStrChanging;
		}
	}
	else {
		// ImGui::Text("%s", spirit._spirit_name.c_str());
	}
	if(spirit._empty) {
		if(ImGui::Button("Add spirit", ImVec2(200, 50))) {
		}
	}
	else {
		// changing size and position
		auto widthLabel = "width##" + *nameStr;
		ImGui::SliderFloat(widthLabel.c_str(), &spirit._size_ratio[0], -1.0f, 1.0f);
		auto heightLabel = "height##" + *nameStr;
		ImGui::SliderFloat(heightLabel.c_str(), &spirit._size_ratio[1], -1.0f, 1.0f);
		auto xLabel = "x-cord##" + *nameStr;
		ImGui::SliderFloat(xLabel.c_str(), &spirit._position_ratio[0], 0.0f, 1.0f);
		auto yLabel = "y-cord##" + *nameStr;
		ImGui::SliderFloat(yLabel.c_str(), &spirit._position_ratio[1], 0.0f, 1.0f);
		if(linkable && !linked) {
			if(ImGui::Button("Set As Library Spirit")) {
			}
		}
	}
	// 	ImGui::TreePop();
	// }
}
void Cast::textboxTreeNode(Textbox& textbox) {
	// if(ImGui::CollapsingHeader(textbox._name.c_str() /**, ImGuiTreeNodeFlags_DefaultOpen**/)) {
	auto editLabel = "edit##" + textbox._name;
	auto contentStr = &textbox._content;
	ImGui::InputTextMultiline(editLabel.c_str(), contentStr, ImVec2(200, 100));
	auto xLabel = "x-cord##" + textbox._name;
	ImGui::SliderFloat(xLabel.c_str(), &textbox._position_ratio[0], 0.0f, 1.0f);
	auto yLabel = "y-cord##" + textbox._name;
	ImGui::SliderFloat(yLabel.c_str(), &textbox._position_ratio[1], 0.0f, 1.0f);
	// ImGui::TreePop();
	// }
}

void Cast::buttonTreeNode(Button& button) {
	// if(ImGui::CollapsingHeader(button._nickname.c_str() /**, ImGuiTreeNodeFlags_DefaultOpen**/)) {
	auto nameStr = &(button._nickname);
	auto renameLabel = "rename##" + button._nickname;
	ImGui::InputText(renameLabel.c_str(), nameStr);
	auto triggerLabel = "trigger##" + button._nickname;
	if(ImGui::CollapsingHeader("Trigger", ImGuiTreeNodeFlags_DefaultOpen)) {
		addTrigger();
	}
	if(ImGui::Button("show area", ImVec2(100, 30))){
		button._show_area = !button._show_area;
	}
	if(ImGui::CollapsingHeader("Spirit", ImGuiTreeNodeFlags_DefaultOpen)) {
		for(auto& spirit : button._button_spirits) {
			// if(spirit._empty) {
			// 	addSpiritTreeNode(spirit, spirit._spirit_name);
			// }
			// else {
			// addSpiritTreeNode(spirit);
			if(ImGui::TreeNode(spirit._spirit_name.c_str())) {
				spiritNodeContent(spirit, false, false, false);
				ImGui::TreePop();
			}
			// }
		}
	}
	// ImGui::TreePop();
	// }
	// ImGui::End();
}

// let me think about this, i don't think i need a function
void Cast::addSpiritTreeNode(Spirit& spirit, const std::optional<std::string>& name) {
	if(name.has_value()) {
		spirit._spirit_name = *name;
	}
	else {
		spirit._spirit_name = "spirit_default";
	}
	if(ImGui::Button("Add spirit", ImVec2(200, 50))) {
		// spirit._spirit_file_name = importImage();
		// std::cout <<spirit._spirit_file_name;
	}
	// // this will be abandoned now, but this should be the logic for import
	//     ImGui::Begin("New Spirit");
	//     ImGui::Button("Confirm", ImVec2(200, 50)){

	//     }
	//     ImGui::End();
}

// void RenderUI() {
//     // Start a new ImGui window
//     ImGui::Begin("Color Picker with Apply Button");

//     // Color picker widget
//     ImGui::ColorEdit3("Select Background Color", (float*)&selectedColor);

//     // Add a button to apply the selected color
//     if (ImGui::Button("Apply Color")) {
//         currentBackgroundColor = selectedColor;  // Set the background color to the selected color
//     }

//     // Text to display the current state
//     ImGui::Text("Click 'Apply Color' to set this window's background color.");

//     ImGui::End();

//     // Set the window background color using the applied color
//     ImGui::PushStyleColor(ImGuiCol_WindowBg, currentBackgroundColor);

//     // Another window that uses the selected background color
//     ImGui::Begin("Window with Custom Background");

//     ImGui::Text("This window has a custom background color!");

//     ImGui::End();

//     ImGui::PopStyleColor();
// }

std::string Cast::importImage() {
	char const* lFilterPatterns[2] = {"*.png", "*.jpg"};
	auto path = tinyfd_openFileDialog(NULL, (*_game_data_ptr)->_project_path.c_str(), 2, lFilterPatterns, "image files", 1);
	std::string importedPath = "asd";
	return importedPath;

	// cv::Mat image = cv::imread(path);
	// std::cout << (*_game_data_ptr)->_project_path << "/" <<  << std::endl;
	// if (!cv::imwrite((*_game_data_ptr)->_project_path, image)) {
    //     std::cerr << "Could not save the image to path: " << savePath << std::endl;
    //     return;
    // }
	// // std::cout << path << std::endl;

	// im thinking of recording down all the name as default names and store the images with
}

void Cast::showLibraryImage() {
}

// ------------ to do for the bottom two, get a pop-up window -------------
void Cast::showLibrarySpirit() {
	ImGui::Begin("Spirit Library", &_window_triggers->at("show_library"));
	if(ImGui::Button("Add Spirit", ImVec2(-FLT_MIN, 50))) {
		// ImGui::Begin();

		// ImGui::End();
	}
	// if(ImGui::Button("Link to a Current Spirit"))
	ImGui::End();
}

void Cast::addTrigger() {
	if(ImGui::Button("Add Trigger", ImVec2(-FLT_MIN, 50))) {
		// I think a pop-up window is the option
		ImGui::Begin("New Trigger");
		// select value
		//
		ImGui::End();
	}
}

void Cast::createValue() {
}

// // using ElementVariant = std::variant<Spirit, Textbox, Button, Page>;
// void Cast::showDetailElement(ElementVariant element){
// 	ImGui::Begin("Element Detail", &_element_window_open, 0);
// 	if (std::holds_alternative<Spirit&>(element)){
// 		spiritNodeContent(element, false, true, true);
// 	}
// 	else if(std::holds_alternative<Textbox&>(element)){
// 		textboxTreeNode(element);
// 	}
// 	else if(std::holds_alternative<Button&>(element)){
// 		buttonTreeNode(element);
// 	}
// 	ImGui::End();
// }