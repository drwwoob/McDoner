#pragma once
#include "Button.hpp"

// not sure how to inherit, but this class should inherit from Page...?
// class ButtonInterface : Page {
class ButtonInterface {
private:
    std::vector<Button> _buttons;
    // a temporary save, during save will reorder _buttons accordingly
    std::vector<int> _draw_order; 
    // interface mode:
    // 0: always showing
    // 1: only show when mouse is in a certain area
    int _interface_mode = 0;
    // only activate in mode 1, the areas where interface will show
    std::vector<ImVec2> _showing_box;
public:
    ButtonInterface();
    ButtonInterface(const std::string& interface_data);
    // what about i just write this directly as a c++ file
    // haha, since i haven't make the game-generated-and-run file yet i don't even know what the c++ files would look like
    void addButton();
    void deleteButton();
    void draw();
    /**
     * in the format
     *  [{button1}{button2}{button3}]
     */
    std::string encrpyt();
    // void saveInLibrary();
};