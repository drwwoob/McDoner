#pragma once

class Page;

struct Library {
	// library storage
	std::map<std::string, Page> _pages_library; // <name, page>
	std::vector<Button> _button_library; // Button
	std::vector<Spirit> _spirit_library; // Spirit
	std::vector<Textbox> _textbox_library; // Textbox

	void decryptDataBinary(const std::string& lib_data) {
	}

    void create(){

    }

    void link(){

    }

    void unlink(){

    }

	// Page getPage(std::string name) {
	// 	if(_pages_library.contains(name)) {
	// 		_pages_library.at(name);
	// 	}
    //     tinyfd_messageBox("Page Error", "No Page element find", "ok",  "error", 1);
	// }

	/**
     * in the format
     * {spirits}
     * {textboxs}
     * {buttons}
     * [page][page]
     */
	std::string encrypt() {
		return "";
	}
};