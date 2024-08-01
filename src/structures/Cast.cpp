#include <Cast.h>
#include "imgui_stdlib.h"
//#include "Tools.h"
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

void Cast::showMenuBar(Data& game_data, int& page_at, Page &clipboard_page, Backup &backup_data) {
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("New", "Ctrl+N")) {
            }
            // Add items to the "File" menu
            if(ImGui::MenuItem("Open", "Ctrl+O")) {
                // Handle "Open" action
            }
            if(ImGui::MenuItem("Save", "Ctrl+S")) {
                // Handle "Save" action
				game_data.save(page_at);
            }
            if(ImGui::MenuItem("Import", "Ctrl+Shift+O")) {
            }

            if(ImGui::MenuItem("Exit", "Alt+F4")) {
                // Handle "Exit" action
            }
            ImGui::EndMenu();
        }

        ImGui::SameLine();
        if(ImGui::BeginMenu("Edit")) {
			if(ImGui::MenuItem("Undo", "Crtl+Z")){
				// undo(page_at, game_data, backup_data);
			}
            ImGui::EndMenu();
        }

        ImGui::SameLine();
        if(ImGui::BeginMenu("Scene")) {
            bool disabled = false;

            if(page_at == 0) {
                disabled = true;
                ImGui::BeginDisabled();
            }
            if(ImGui::MenuItem("last page", "Ctrl+K")) {
                lastPage(page_at, game_data);
            }
            if(disabled) {
                ImGui::EndDisabled();
                disabled = false;
            }

            if(page_at >= game_data.pageSize() - 1) {
                disabled = true;
                ImGui::BeginDisabled();
            }
            if(ImGui::MenuItem("next page", "Ctrl+L")) {
                nextPage(page_at, game_data);
            }
            if(disabled) {
                ImGui::EndDisabled();
                disabled = false;
            }

            if(game_data.pageSize() == 1) {
                disabled = true;
                ImGui::BeginDisabled();
            }
            if(ImGui::MenuItem("delete current page")) {
                deletePage(page_at, game_data);
            }
            if(disabled) {
                ImGui::EndDisabled();
                disabled = false;
            }

            if(ImGui::MenuItem("add blank page")) {
                addPage(page_at, game_data);
            }
            if(ImGui::MenuItem("duplicate page")) {
                duplicatePage(page_at, game_data);
            }
            if(ImGui::MenuItem("copy page")) {
				copyPage(page_at, game_data, clipboard_page);
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

void Cast::showCastsInPage(bool* p_open, Page* page_info) {
    ImGuiWindowFlags window_flags = 0;

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

void Cast::showWelcomePage(Data& game_data, bool& show_welcome_window, bool& page_setting, int& page_at) {
    ImGui::SetNextWindowSize(ImVec2(240, 300));
    ImGui::Begin("Welcome Page", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    if(ImGui::Button("New", ImVec2(-FLT_MIN, 80))) {
        game_data.newFile();
        show_welcome_window = false;
        page_setting = true;
    }
    if(ImGui::Button("Open", ImVec2(-FLT_MIN, 80))) {
        game_data.openFile();
        show_welcome_window = false;
        page_setting = true;
        game_data.loadTexture(page_at);
    }
    if(ImGui::Button("Demo", ImVec2(-FLT_MIN, 80))) {
        game_data = Data("../saves/demo/demo.txt", page_at);
        show_welcome_window = false;
        page_setting = true;
        // =========== todo: here is for saved page_at thingy ===============
        // page_at = 0;
        game_data.loadTexture(page_at);
    }
    ImGui::End();
}

void Cast::showAmongPages(bool* p_open, int& page_at, Data& game_data) {
    ImGui::Begin("Page Setting");

    bool disabled = false;

    if(page_at == 0) {
        disabled = true;
        ImGui::BeginDisabled();
    }
    if(ImGui::Button("Last Page", ImVec2(100, 100))) {
        lastPage(page_at, game_data);
    }
    if(disabled) {
        ImGui::EndDisabled();
        disabled = false;
    }

    ImGui::SameLine();

    if(page_at >= game_data.pageSize() - 1) {
        disabled = true;
        ImGui::BeginDisabled();
    }
    if(ImGui::Button("Next Page", ImVec2(100, 100))) {
        nextPage(page_at, game_data);
    }
    if(disabled) {
        ImGui::EndDisabled();
        disabled = false;
    }

    if(ImGui::Button("add Page", ImVec2(100, 100))) {
        addPage(page_at, game_data);
    }

    ImGui::SameLine();

    if(ImGui::Button("duplicate Page", ImVec2(100, 100))) {
        duplicatePage(page_at, game_data);
    }

    ImGui::SameLine();

    if(game_data.pageSize() == 1) {
        disabled = true;
        ImGui::BeginDisabled();
    }
    if(ImGui::Button("delete Page", ImVec2(100, 100))) {
        deletePage(page_at, game_data);
    }
    if(disabled) {
        ImGui::EndDisabled();
        disabled = false;
    }

    ImGui::End();
}

void Cast::lastPage(int& page_at, Data& game_data) {
    page_at = page_at - 1;
    game_data.loadTexture(page_at);
}
void Cast::nextPage(int& page_at, Data& game_data) {
    page_at = page_at + 1;
    game_data.loadTexture(page_at);
}
void Cast::addPage(int& page_at, Data& game_data) {
    page_at = page_at + 1;
    game_data.addPage(page_at);
    game_data.loadTexture(page_at);
}
void Cast::duplicatePage(int& page_at, Data& game_data) {
    game_data.CopyPage(page_at + 1, *(game_data.getPage(page_at)));
    page_at = page_at + 1;
    game_data.loadTexture(page_at);
}
void Cast::deletePage(int& page_at, Data& game_data) {
    game_data.deletePage(page_at);
    if(page_at != 0) {
        page_at = page_at - 1;
        game_data.loadTexture(page_at);
    }
}
void Cast::copyPage(int& page_at, Data& game_data, Page& clipboard_page) {
    clipboard_page = *game_data.getPage(page_at);
};