#include "ButtonInterface.h"
#include <numeric>

ButtonInterface::ButtonInterface(){
}

ButtonInterface::ButtonInterface(const std::string &interface_data){
    _draw_order.resize(_buttons.size());
    std::iota(_draw_order.begin(), _draw_order.end(), 0);
}

void ButtonInterface::addButton(){
    _draw_order.emplace_back(_buttons.size());
    _buttons.emplace_back(_buttons.size());
}

void ButtonInterface::deleteButton(){

}

void ButtonInterface::draw(){
    // draw the buttons out
    for(auto draw_id : _draw_order){
        _buttons.at(draw_id).draw();
    }

    // set the detect zone for each button
}

std::string ButtonInterface::encrpyt(){
    std::string save_string = "{";
    // reorder the _buttons and write it that way
    for(auto draw_id : _draw_order){
        _buttons.at(draw_id).save(save_string);
        save_string += '#';
    }
    save_string += '}';
    return save_string;
}