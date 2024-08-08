#pragma once
#include <vector>

template <typename T>
struct buttonFeature{
    std::string _name;
    std::string _unclick_img_path;
    std::string _on_click_img_path;
    std::unique_ptr<T> _page_to;
    buttonFeature(
        const std::string& name, 
        std::unique_ptr<T> page_to,
        const std::string& unclick_img_path,
        const std::string& on_click_img_path)
        :
        _name(name),
        _page_to(std::move(page_to)),
        _unclick_img_path(unclick_img_path),
        _on_click_img_path(on_click_img_path){
    };
};

struct otherPage{
    
};

struct MainPage{
    std::string _name;
    std::string _background_img_path;
    std::vector<buttonFeature<int>> _buttons;
    MainPage(
        const std::string& name)
    :
    _name(name){
    };
};

class OtherPage{
    OtherPage(){
    //     _setting_list = {
    //         "New Game",
    //         "Load Game", 
    //         "Setting",
    //         "Credit",
    //         "Exit"
    //     };

    };
    ~OtherPage();

private:
    // std::vector<std::string> _setting_list;
    std::vector<MainPage> _main_page;
};