#pragma once
#include <Button.h>

class ButtonInterface{
private:
    std::vector<Button> _buttons;
public:
    ButtonInterface();
    ButtonInterface(const std::string& data);
    // what about i just write this directly as a c++ file
    // std::string encrpyt();

    void addButton();
    void deleteButton();
};