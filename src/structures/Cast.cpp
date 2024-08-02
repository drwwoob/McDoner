#include <Cast.h>
#include "imgui_stdlib.h"
#include <unordered_map>
#include "Tools.h"
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

Cast::Cast(std::shared_ptr<std::shared_ptr<Data>> game_data_ptr, 
        std::unique_ptr<Backup> backup_data) : game_data_ptr(game_data_ptr), backup_data(std::move(backup_data)){
	shortkey_outlay = Tools::loadShortkeys("../src/settings/keyLoad.json");
}
Cast::~Cast() {}

void Cast::showMenuBar(Page &clipboard_page) {
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("New", shortkey_outlay.at("New").c_str())) {
            }
            // Add items to the "File" menu
            if(ImGui::MenuItem("Open", shortkey_outlay.at("Open").c_str())) {
                // Handle "Open" action
            }
            if(ImGui::MenuItem("Save", shortkey_outlay.at("Save").c_str())) {
                // Handle "Save" action
				(*game_data_ptr)->save();
            }
            if(ImGui::MenuItem("Import", shortkey_outlay.at("Import").c_str())) {
            }

            if(ImGui::MenuItem("Exit", shortkey_outlay.at("Exit").c_str())) {
                // Handle "Exit" action
            }
            ImGui::EndMenu();
        }

        ImGui::SameLine();
        if(ImGui::BeginMenu("Edit")) {
            bool disabled = false;
			if(!backup_data->undoAvailible()) {
                disabled = true;
                ImGui::BeginDisabled();
            }
			if(ImGui::MenuItem("Undo", "Ctrl+Z")){
				backup_data->undo();
			}
            if(disabled) {
                ImGui::EndDisabled();
                disabled = false;
            }

			if(!backup_data->redoAvailible()) {
                disabled = true;
                ImGui::BeginDisabled();
            }
			if(ImGui::MenuItem("Redo", "Ctrl+Shift+Z")){
				backup_data->redo();
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

            if((*game_data_ptr)->page_at == 0) {
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

            if((*game_data_ptr)->page_at >= (*game_data_ptr)->pageSize() - 1) {
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

            if((*game_data_ptr)->pageSize() == 1) {
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
            if(ImGui::MenuItem("list")) {
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
        ImGui::EndMainMenuBar();
    }
}

void Cast::showCastsInPage(bool* p_open) {
    ImGuiWindowFlags window_flags = 0;
	auto page_info = (*game_data_ptr)->getPage((*game_data_ptr)->page_at);

    ImGui::Begin("Cast", p_open, window_flags);

    // const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    //ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 100, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200, 20), ImGuiCond_FirstUseEver);
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    ImGui::Text("This is the list of cast on the current page");
    IMGUI_MARKER("Spirit");
    //ImGui::SetNextWindowCollapsed(false);
    if(ImGui::CollapsingHeader("Spirit", ImGuiTreeNodeFlags_DefaultOpen)) {
        for(int id = 0; id < page_info->spirits.size(); id++) {
            ImGui::SetNextWindowCollapsed(false);
            IMGUI_MARKER(page_info->spirits.at(id).name().c_str());
            if(ImGui::TreeNode(page_info->spirits.at(id).name().c_str())) {
                //ImGui::BulletText("(%s)", page_info->spirits.at(id).name().c_str());
                /*static char buff[32] = "";
				ImGui::InputText("testxt", buff, 32);*/

                //ImGui::SeparatorText( page_info->getRealSpirits(id)->name().c_str());

                auto nameStr = page_info->getRealSpirits(id)->getRealNickName();
                auto renameLabel = "rename##" + page_info->spirits.at(id).getFileName();
                ImGui::InputText(renameLabel.c_str(), nameStr);
                ///*ImGui::InputText("rename", &name_str,
                //ImGuiInputTextFlags_CallbackResize, MyResizeCallback, (void*) &name_str);*/
                //

                // changing size and position
                auto widthLabel = "width##" + page_info->spirits.at(id).getFileName();
                ImGui::SliderFloat(widthLabel.c_str(), &page_info->getRealSpirits(id)->sizeRatio[0], 0.0f, 1.0f);
                auto heightLabel = "height##" + page_info->spirits.at(id).getFileName();
                ImGui::SliderFloat(heightLabel.c_str(), &page_info->getRealSpirits(id)->sizeRatio[1], 0.0f, 1.0f);
                auto xLabel = "x-cord##" + page_info->spirits.at(id).getFileName();
                ImGui::SliderFloat(xLabel.c_str(), &page_info->getRealSpirits(id)->positionRatio[0], 0.0f, 1.0f);
                auto yLabel = "y-cord##" + page_info->spirits.at(id).getFileName();
                ImGui::SliderFloat(yLabel.c_str(), &page_info->getRealSpirits(id)->positionRatio[1], 0.0f, 1.0f);

                ImGui::TreePop();
            }
        }
    }
    IMGUI_MARKER("Textbox");
    if(ImGui::CollapsingHeader("Textbox", ImGuiTreeNodeFlags_DefaultOpen)) {
        for(auto id = 0; id < page_info->textboxs.size(); id++) {
            if(ImGui::TreeNode(page_info->textboxs[id].name.c_str())) {
                auto editLabel = "edit##" + page_info->textboxs[id].name;
                auto contentStr = page_info->textboxs[id].getRealContent();
                //ImGui::BulletText("%s", textbox.content.c_str());
                ImGui::InputTextMultiline(editLabel.c_str(), contentStr);

                auto xLabel = "x-cord##" + page_info->textboxs.at(id).name;
                ImGui::SliderFloat(xLabel.c_str(), &page_info->getRealTextbox(id)->positionRatio[0], 0.0f, 1.0f);
                auto yLabel = "y-cord##" + page_info->textboxs.at(id).name;
                ImGui::SliderFloat(yLabel.c_str(), &page_info->getRealTextbox(id)->positionRatio[1], 0.0f, 1.0f);

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
        (*game_data_ptr)->newFile();
        show_welcome_window = false;
        page_setting = true;
    }
    if(ImGui::Button("Open", ImVec2(-FLT_MIN, 80))) {
        (*game_data_ptr)->openFile();
        show_welcome_window = false;
        page_setting = true;
        (*game_data_ptr)->loadTexture();
    }
    if(ImGui::Button("Demo", ImVec2(-FLT_MIN, 80))) {
        *game_data_ptr = std::make_shared<Data>("../saves/demo/demo.txt");
        show_welcome_window = false;
        page_setting = true;
        // =========== todo: here is for saved (*game_data_ptr)->page_at thingy ===============
        // (*game_data_ptr)->page_at = 0;
        (*game_data_ptr)->loadTexture();
        backup_data->addMove();
    }
    ImGui::End();
}

void Cast::showAmongPages(bool* p_open) {
    ImGui::Begin("Page Setting");

    bool disabled = false;

    if((*game_data_ptr)->page_at == 0) {
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

    if((*game_data_ptr)->page_at >= (*game_data_ptr)->pageSize() - 1) {
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

    if((*game_data_ptr)->pageSize() == 1) {
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
    backup_data->addMove();
    (*game_data_ptr)->page_at = (*game_data_ptr)->page_at - 1;
    (*game_data_ptr)->loadTexture();
}
void Cast::nextPage() {
    backup_data->addMove();
    (*game_data_ptr)->page_at = (*game_data_ptr)->page_at + 1;
    (*game_data_ptr)->loadTexture();
}
void Cast::addPage() {
    backup_data->addMove();
    (*game_data_ptr)->page_at = (*game_data_ptr)->page_at + 1;
    (*game_data_ptr)->addPage((*game_data_ptr)->page_at);
    (*game_data_ptr)->loadTexture();
}
void Cast::duplicatePage() {
    backup_data->addMove();
    (*game_data_ptr)->CopyPage((*game_data_ptr)->page_at + 1, *((*game_data_ptr)->getPage((*game_data_ptr)->page_at)));
    (*game_data_ptr)->page_at++;
    (*game_data_ptr)->loadTexture();
}
void Cast::deletePage() {
    backup_data->addMove();
    (*game_data_ptr)->deletePage((*game_data_ptr)->page_at);
    if((*game_data_ptr)->page_at != 0) {
        (*game_data_ptr)->page_at--;
        (*game_data_ptr)->loadTexture();
    }
}
void Cast::copyPage(Page& clipboard_page) {
    backup_data->addMove();
    clipboard_page = *(*game_data_ptr)->getPage((*game_data_ptr)->page_at);
};