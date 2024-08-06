#pragma once
#include <string>
#include <functional>
#include <imgui.h>

class Button{
public:
    std::string _name;
    std::string _unclick_img_path;
    std::string _selecting_img_path;
    std::string _onclick_img_path;
    std::string _clicked_img_path;
    ImVec2 ratio;
    ImVec2 position;
    // button mode:
    // 0: always showing
    // 1: only show when mouse is in a certain area
    int mode = 0;

    using FunctionVariant = std::variant<
        std::function<void(void)>,
        std::function<void(int&)>,
        std::function<void(double&)>,
        std::function<void(std::string&)>
    >;
    std::vector<std::shared_ptr<FunctionVariant>> _func_ptrs;

    void addFunction(std::shared_ptr<FunctionVariant> func_ptr) {
        _func_ptrs.push_back(func_ptr);
    }

    Button(int id){
        _name = "button" + std::to_string(id);
    }

    void setUnclickImg(const std::string& path){

    }

    void setOnclickImg(const std::string& path){

    }

    void setClickedImg(const std::string& path){

    }


// template <typename... T>
//     void activate(T&... args){
//         auto arg_list = std::make_tuple(args...);
//         int i = 0;

//         for (auto func_ptr : _func_ptrs){
//             std::visit([](auto&& arg) {
//                 using T = std::decay_t<decltype(arg)>;
                
//                 if constexpr (std::is_same_v<T, std::function<void(void)>>) {
//                 } 
//                 else{
//                     (*func_ptr)(std::get<i>(arg_list));
//                     i++;
//                 }
//                 // else if constexpr (std::is_same_v<T, std::function<void(int&)>>) {
//                 //     std::cout << "Type is std::function<void(int)>" << std::endl;
//                 // } else if constexpr (std::is_same_v<T, std::function<void(double&)>>) {
//                 //     std::cout << "Type is std::function<void(double)>" << std::endl;
//                 // } else if constexpr (std::is_same_v<T, std::function<void(std::string&)>>) {
//                 //     std::cout << "Type is std::function<void(int, double)>" << std::endl;
//                 // }
//             }, *func_ptr);
//         }
//     }
};