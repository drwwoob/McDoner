#pragma once

class Page;

struct Library{
    // library storage
    std::map<std::string, Page> _pages_library;  // <name, page>
    std::vector<Button> _button_library; // Button
    std::vector<Spirit> _spirit_library; // Spirit
    std::vector<Textbox> _textbox_library; // Textbox

    void decryptData(const std::string& lib_data){
        
    }

    /**
     * in the format
     * {spirits}
     * {textboxs}
     * {buttons}
     * [page][page]
     */
    void encrypt(){

    }
};