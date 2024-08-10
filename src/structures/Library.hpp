#pragma once

class Page;

struct Library{
    // library storage
    std::map<std::string, Page> _pages_library;  // <name, page>
    std::vector<Button> _button_Library; // Button
    std::vector<Spirit> _spirit_Library; // Spirit
    std::vector<Textbox> _textbox_Library; // Textbox
};