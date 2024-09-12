#include "Cast.hpp"

template <typename T>
void Cast::showDetailElement(T element, bool* p_open) {
	// if(!element.has_value()){
	//     return;
	// }
	if constexpr(std::is_same_v<T, Spirit*>) {
		// std::cout << element->
		ImGui::Begin(element->_spirit_name.c_str(), p_open, ImGuiTreeNodeFlags_DefaultOpen);
		spiritNodeContent(*element, false, true, true);
		ImGui::End();
	}
	else if constexpr(std::is_same_v<T, Textbox*>) {
		ImGui::Begin(element->_name.c_str(), p_open, ImGuiTreeNodeFlags_DefaultOpen);
		textboxTreeNode(*element);
		ImGui::End();
	}
	else if constexpr(std::is_same_v<T, Button*>) {
        // std::cout << element->_nickname.c_str() << std::endl;
		ImGui::Begin(element->_nickname.c_str(), p_open, ImGuiTreeNodeFlags_DefaultOpen);
		buttonTreeNode(*element);
		ImGui::End();
		// draw_item.second = button._nickname;
	}
	// else if constexpr (std::is_same_v<T, Page>) {
	//     std::cout << "Processing Page" << std::endl;
	// }
}