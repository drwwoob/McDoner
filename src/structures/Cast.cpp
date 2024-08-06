#include <Cast.h>
#include "imgui_stdlib.h"
#include <unordered_map>
#include "Tools.h"
#include <tinyfiledialogs.h>
#include <filesystem>
//#include "data.h"
typedef void (*ImGuiMarkerCallback)(const char* file, int line, const char* section, void* user_data);
extern ImGuiMarkerCallback GImGuiMarkerCallback;
extern void* GImGuiMarkerCallbackUserData;
ImGuiMarkerCallback GImGuiMarkerCallback = NULL;
void* GImGuiMarkerCallbackUserData = NULL;
#define IMGUI_MARKER(section)                                                                                             \
    do {                                                                                                                  \
        if(GImGuiMarkerCallback != NULL) GImGuiMarkerCallback(__FILE__, __LINE__, section, GImGuiMarkerCallbackUserData); \
    } while(0)

Cast::Cast(std::shared_ptr<
        std::shared_ptr<Data>> game_data_ptr, 
        std::unique_ptr<Backup> backup_data)
        : 
        _game_data_ptr(game_data_ptr), 
        _backup_data(std::move(backup_data)){
	_shortkey_outlay = Tools::loadShortkeys("../src/settings/keyLoad.json");
}
Cast::~Cast() {}

void Cast::showMenuBar(Page &clipboard_page) {
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("New", _shortkey_outlay.at("New").c_str())) {
                newProject();
            }
            // Add items to the "File" menu
            if(ImGui::MenuItem("Open", _shortkey_outlay.at("Open").c_str())) {
                openProject();
            }
            if(ImGui::MenuItem("Save", _shortkey_outlay.at("Save").c_str())) {
                // Handle "Save" action
				(*_game_data_ptr)->save();
            }
            if(ImGui::MenuItem("Import", _shortkey_outlay.at("Import").c_str())) {
            }
            if(ImGui::MenuItem("Exit", _shortkey_outlay.at("Exit").c_str())) {
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
			if(ImGui::MenuItem("Undo", "Ctrl+Z")){
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
			if(ImGui::MenuItem("Redo", "Ctrl+Shift+Z")){
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
				copyPage(clipboard_page);
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
        if(ImGui::BeginMenu("View")){
            if(ImGui::MenuItem("Starting Page")){

            }
            if(ImGui::MenuItem("Page Gallary")){

            }
            ImGui::EndMenu();
        }
        ImGui::SameLine();
        if(ImGui::BeginMenu("Run")){
            if(ImGui::MenuItem("Run New Game")){

            }
            if(ImGui::MenuItem("Run From Here")){

            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Cast::showCastsInPage(bool* p_open) {
    ImGuiWindowFlags window_flags = 0;
	auto page_info = (*_game_data_ptr)->getPage((*_game_data_ptr)->_page_at);

    ImGui::Begin("Cast", p_open, window_flags);

    // const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    //ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 100, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200, 20), ImGuiCond_FirstUseEver);
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    ImGui::Text("This is the list of cast on the current page");
    IMGUI_MARKER("Spirit");
    //ImGui::SetNextWindowCollapsed(false);
    if(ImGui::CollapsingHeader("Spirit", ImGuiTreeNodeFlags_DefaultOpen)) {
        for(int id = 0; id < page_info->_spirits.size(); id++) {
            ImGui::SetNextWindowCollapsed(false);
            IMGUI_MARKER(page_info->_spirits.at(id).name().c_str());
            if(ImGui::TreeNode(page_info->_spirits.at(id).name().c_str())) {
                //ImGui::BulletText("(%s)", page_info->spirits.at(id).name().c_str());
                /*static char buff[32] = "";
				ImGui::InputText("testxt", buff, 32);*/

                //ImGui::SeparatorText( page_info->getRealSpirits(id)->name().c_str());

                auto nameStr = page_info->getRealSpirits(id)->getRealNickName();
                auto renameLabel = "rename##" + page_info->_spirits.at(id).getFileName();
                ImGui::InputText(renameLabel.c_str(), nameStr);
                ///*ImGui::InputText("rename", &name_str,
                //ImGuiInputTextFlags_CallbackResize, MyResizeCallback, (void*) &name_str);*/
                //

                // changing size and position
                auto widthLabel = "width##" + page_info->_spirits.at(id).getFileName();
                ImGui::SliderFloat(widthLabel.c_str(), &page_info->getRealSpirits(id)->_size_ratio[0], 0.0f, 1.0f);
                auto heightLabel = "height##" + page_info->_spirits.at(id).getFileName();
                ImGui::SliderFloat(heightLabel.c_str(), &page_info->getRealSpirits(id)->_size_ratio[1], 0.0f, 1.0f);
                auto xLabel = "x-cord##" + page_info->_spirits.at(id).getFileName();
                ImGui::SliderFloat(xLabel.c_str(), &page_info->getRealSpirits(id)->_position_ratio[0], 0.0f, 1.0f);
                auto yLabel = "y-cord##" + page_info->_spirits.at(id).getFileName();
                ImGui::SliderFloat(yLabel.c_str(), &page_info->getRealSpirits(id)->_position_ratio[1], 0.0f, 1.0f);

                ImGui::TreePop();
            }
        }
    }
    IMGUI_MARKER("Textbox");
    if(ImGui::CollapsingHeader("Textbox", ImGuiTreeNodeFlags_DefaultOpen)) {
        for(auto id = 0; id < page_info->_textboxs.size(); id++) {
            if(ImGui::TreeNode(page_info->_textboxs[id]._name.c_str())) {
                auto editLabel = "edit##" + page_info->_textboxs[id]._name;
                auto contentStr = page_info->_textboxs[id].getRealContent();
                //ImGui::BulletText("%s", textbox.content.c_str());
                ImGui::InputTextMultiline(editLabel.c_str(), contentStr);

                auto xLabel = "x-cord##" + page_info->_textboxs.at(id)._name;
                ImGui::SliderFloat(xLabel.c_str(), &page_info->getRealTextbox(id)->_position_ratio[0], 0.0f, 1.0f);
                auto yLabel = "y-cord##" + page_info->_textboxs.at(id)._name;
                ImGui::SliderFloat(yLabel.c_str(), &page_info->getRealTextbox(id)->_position_ratio[1], 0.0f, 1.0f);

                ImGui::TreePop();
            }
            //ImGui::BulletText("(%s)", textboxs.c_str());
        }
    }

    IMGUI_MARKER("Background");
    if(ImGui::CollapsingHeader("Background", ImGuiTreeNodeFlags_DefaultOpen)) {
		// choose the color for background here
    }
    ImGui::End();
}

void Cast::showWelcomePage(bool& show_welcome_window, bool& page_setting) {
    ImGui::SetNextWindowSize(ImVec2(240, 300));
    ImGui::Begin("Welcome Page", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    if(ImGui::Button("New", ImVec2(-FLT_MIN, 80))) {
        if(newProject()){
            show_welcome_window = false;
            page_setting = true;
        }
    }
    if(ImGui::Button("Open", ImVec2(-FLT_MIN, 80))) {
        if(openProject()){
            show_welcome_window = false;
            page_setting = true;
        }
    }
    if(ImGui::Button("Demo", ImVec2(-FLT_MIN, 80))) {
        *_game_data_ptr = std::make_shared<Data>("../saves/demo/demo.txt");
        show_welcome_window = false;
        page_setting = true;
        // =========== todo: here is for saved (*game_data_ptr)->page_at thingy ===============
        // (*game_data_ptr)->page_at = 0;
        (*_game_data_ptr)->loadTexture();
        _backup_data->addMove();
    }
    ImGui::End();
}

void Cast::showAmongPages(bool* p_open) {
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

    if(ImGui::Button("add Page", ImVec2(100, 100))) {
        addPage();
    }

    ImGui::SameLine();

    if(ImGui::Button("duplicate Page", ImVec2(100, 100))) {
        duplicatePage();
    }

    ImGui::SameLine();

    if((*_game_data_ptr)->pageSize() == 1) {
        disabled = true;
        ImGui::BeginDisabled();
    }
    if(ImGui::Button("delete Page", ImVec2(100, 100))) {
        deletePage();
    }
    if(disabled) {
        ImGui::EndDisabled();
        disabled = false;
    }

    ImGui::End();
}

void Cast::lastPage() {
    _backup_data->addMove();
    (*_game_data_ptr)->_page_at = (*_game_data_ptr)->_page_at - 1;
    (*_game_data_ptr)->loadTexture();
}
void Cast::nextPage() {
    _backup_data->addMove();
    (*_game_data_ptr)->_page_at = (*_game_data_ptr)->_page_at + 1;
    (*_game_data_ptr)->loadTexture();
}
void Cast::addPage() {
    _backup_data->addMove();
    (*_game_data_ptr)->_page_at = (*_game_data_ptr)->_page_at + 1;
    (*_game_data_ptr)->addPage((*_game_data_ptr)->_page_at);
    (*_game_data_ptr)->loadTexture();
}
void Cast::duplicatePage() {
    _backup_data->addMove();
    (*_game_data_ptr)->CopyPage((*_game_data_ptr)->_page_at + 1, *((*_game_data_ptr)->getPage((*_game_data_ptr)->_page_at)));
    (*_game_data_ptr)->_page_at++;
    (*_game_data_ptr)->loadTexture();
}
void Cast::deletePage() {
    _backup_data->addMove();
    (*_game_data_ptr)->deletePage((*_game_data_ptr)->_page_at);
    if((*_game_data_ptr)->_page_at != 0) {
        (*_game_data_ptr)->_page_at--;
        (*_game_data_ptr)->loadTexture();
    }
}
void Cast::copyPage(Page& clipboard_page) {
    _backup_data->addMove();
    clipboard_page = *(*_game_data_ptr)->getPage((*_game_data_ptr)->_page_at);
};

bool Cast::newProject(){
	auto lTheSelectFolderName = tinyfd_selectFolderDialog(
		"create new project in", "../saves/");

	if (!lTheSelectFolderName)
	{
        return false;
	}
    auto lTheProjectName = tinyfd_inputBox(
		"project naming", "please give a name to your project", "project");

    if(!lTheProjectName){
        return false;
    }

    std::string path = lTheSelectFolderName;
    path += lTheProjectName;
    if(std::filesystem::exists(path) && std::filesystem::is_directory(path)){
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

bool Cast::openProject()
{
    auto lTheSelectFolderName = tinyfd_selectFolderDialog(
		"select project", "../saves/");

	if (!lTheSelectFolderName)
	{
        return false;
	}

    std::string path = lTheSelectFolderName;
    std::string project_name = path.substr(
        path.substr(0, path.size() - 1).find_last_of("/") + 1
    );
    project_name = project_name.substr(0, project_name.size() - 1);
    path += project_name;
    path += ".txt";
    if(!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path)){
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

void Cast::askForSave(){
    if(*_game_data_ptr && (*_game_data_ptr)->pageSize() != 0){
        auto save_stat = tinyfd_messageBox(
			"Save", "do you wish to save your current project?",
			"yesno", "question", 1);
		if(save_stat){
            (*_game_data_ptr)->save();
        }
    }
}