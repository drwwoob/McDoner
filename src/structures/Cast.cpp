#include <Cast.h>
#include "imgui_stdlib.h"
//#include "Tools.h"
//#include "data.h"
typedef void (*ImGuiMarkerCallback)(const char* file, int line, const char* section, void* user_data);
extern ImGuiMarkerCallback      GImGuiMarkerCallback;
extern void* GImGuiMarkerCallbackUserData;
ImGuiMarkerCallback             GImGuiMarkerCallback = NULL;
void* GImGuiMarkerCallbackUserData = NULL;
#define IMGUI_MARKER(section)  do { if (GImGuiMarkerCallback != NULL) GImGuiMarkerCallback(__FILE__, __LINE__, section, GImGuiMarkerCallbackUserData); } while (0)


void Cast::showCastsInPage(bool* p_open, Page *page_info) {
	ImGuiWindowFlags window_flags = 0;
	//Page page_info = gameData.getPage(pageID);

	//if(!ImGui::Begin("Cast", p_open, window_flags)) {
	//	ImGui::End();
	//	return;
	//}

	ImGui::Begin("Cast", p_open, window_flags);

	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	//ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 100, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(200, 20), ImGuiCond_FirstUseEver);
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	ImGui::Text("This is the list of cast on the current page");
	IMGUI_MARKER("Spirit");
	//ImGui::SetNextWindowCollapsed(false);
	if (ImGui::CollapsingHeader("Spirit", ImGuiTreeNodeFlags_DefaultOpen)) {
		for (int id = 0; id < page_info->spirits.size(); id++) {
			ImGui::SetNextWindowCollapsed(false);
			IMGUI_MARKER(page_info->spirits.at(id).name().c_str());
			if (ImGui::TreeNode(page_info->spirits.at(id).name().c_str())) {
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
	if (ImGui::CollapsingHeader("Textbox", ImGuiTreeNodeFlags_DefaultOpen)) {
		for (auto id = 0; id < page_info->textboxs.size(); id++) {
			if(ImGui::TreeNode(page_info->textboxs[id].name.c_str())) {

				auto editLabel = "edit##" + page_info->textboxs[id].name;
				auto contentStr = page_info->textboxs[id].getRealContent();
				//ImGui::BulletText("%s", textbox.content.c_str());
				ImGui::InputTextMultiline(editLabel.c_str(),contentStr);


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
		ImGui::Text("%s", page_info->background_name.c_str());
	}
	ImGui::End();
}

void Cast::showWelcomePage(Data& game_data, bool& show_welcome_window, bool& page_setting, int &page_at) {
    ImGui::SetNextWindowSize(ImVec2(240, 300));
    ImGui::Begin("Welcome Page", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	if (ImGui::Button("New", ImVec2(-FLT_MIN, 80))) {
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
		game_data = Data("../saves/demo/demo.txt");
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

	if (page_at == 0) {
		disabled = true;
		ImGui::BeginDisabled();
	}
	if(ImGui::Button("Last Page", ImVec2(100, 100))){
		page_at = page_at - 1;
		game_data.loadTexture(page_at);
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
		page_at = page_at + 1;
		game_data.loadTexture(page_at);
	}
	if (disabled) {
		ImGui::EndDisabled();
		disabled = false;
	}

	if(ImGui::Button("add Page", ImVec2(100, 100))) {
		page_at = page_at + 1;
		game_data.addPage(page_at);
		game_data.loadTexture(page_at);
	}

	ImGui::SameLine();

	if(ImGui::Button("copy Page", ImVec2(100, 100))) {
		game_data.CopyPage(page_at + 1, *(game_data.getPage(page_at)));
		page_at = page_at + 1;
		game_data.loadTexture(page_at);
	}

	ImGui::SameLine();

	if(game_data.pageSize() == 1) {
		disabled = true;
		ImGui::BeginDisabled();
	}
	if (ImGui::Button("delete Page", ImVec2(100, 100))) {
		game_data.deletePage(page_at);
		if(page_at != 0) {
			page_at = page_at - 1;
			game_data.loadTexture(page_at);
		}
	}
	if (disabled) {
		ImGui::EndDisabled();
		disabled = false;
	}
	
	ImGui::End();
};