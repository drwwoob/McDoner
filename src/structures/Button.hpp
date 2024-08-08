#pragma once
#include <string>
#include <functional>
#include <imgui.h>
#include "Spirit.hpp"

class Button{
public:
    std::string _nickname;
    /**
     * imcluding 4 spirits(types of image):
     *  0: unclick spirit
     *  1: selected spirit
     *  2: onclick sprit
     *  3: clicked spirit
     */
    std::array<Spirit, 4> _button_spirits;
    ImVec2 _click_ratio; // the ratio of the clickbox
    ImVec2 _click_position; // the position of the clickbox
    // button mode:
    //  0: clickable
    //  1: unclickable
    int _mode;
    // button status:
    //  0: unclick
    //  1: selected
    //  2: onclick
    //  3: clicked
    int _status;

    using FunctionVariant = std::variant<
        std::function<void()>,
        std::function<void(int&)>,
        std::function<void(double&)>,
        std::function<void(bool&)>,
        std::function<void(std::string&)>
    >;
    std::vector<std::shared_ptr<FunctionVariant>> _func_ptrs;

    void addFunction(std::shared_ptr<FunctionVariant> func_ptr) {
        _func_ptrs.push_back(func_ptr);
    }

    /**
     * creating a blank Button
     * i am thinking of making this id a random number while saving into library
     * or a detection made when the id is generated
     */
    Button(int id){
        _nickname = "button" + std::to_string(id);
    }

    /**
     * decrypt a string into a button
     */
    Button(const std::string& data){
        size_t start = 0;

        // get the stored items
        do{
            start = data.find_first_of("##");
        }while(data.at(start - 1) != '/');

        auto data_block = data.substr(0, start);
        auto buttonFill = [this](const std::array<std::string, 7>& separate_data) {
            _nickname = separate_data[0];
            _mode = std::stoi(separate_data[1]);
            _status = std::stoi(separate_data[2]);
            _click_ratio = ImVec2(std::stof(separate_data[3]), std::stof(separate_data[4]));
            _click_position = ImVec2(std::stof(separate_data[5]), std::stof(separate_data[6]));
        };

        // Tools::decrypt(buttonFill);
        Tools::decrypt<7>(data_block, buttonFill);

        size_t end = start;
        auto spiritCreate = [this](std::array<std::string, 6>& separate_data, size_t index) {
        if (index < _button_spirits.size()) {
            _button_spirits[index] = Spirit(
                separate_data[0],
                separate_data[1],
                std::stof(separate_data[2]),
                std::stof(separate_data[3]),
                std::stof(separate_data[4]),
                std::stof(separate_data[5]));
            }
        };

        // Iterate through each section of the data
        for (int i = 0; i < 4; ++i) {
            // Find the delimiter "##" and ensure it’s properly handled
            do {
                end = data.find("##", end);
                if (end == std::string::npos) break; // Handle case where delimiter not found
                end += 2; // Move past the delimiter
            } while (data[end - 3] != '/'); // Make sure the delimiter is correct

            // Extract the data block for decryption
            std::string data_block = data.substr(start, end - start);

            // Use the lambda function to process the extracted data block
            Tools::decrypt<6>(data_block, [this, &spiritCreate, i](std::array<std::string, 6>& separate_data) {
                spiritCreate(separate_data, i);
            });

            start = end; // Update start for the next section
        }
        // get the functions stored
    }

    /**
     * @param path the path of img inside the project
     * @details existence check in import and select phase
     */
    void setUnclickImg(const std::string& path){
        // if(std::filesystem::exists(path)){
        // }
    }

    void setSelectedImg(const std::string& path){
        
    }

    void setOnclickImg(const std::string& path){

    }

    void setClickedImg(const std::string& path){

    }

    void save(std::string& save_string){

    }

    Spirit currentSpirit(){
        if(_mode == 0 && !_button_spirits[_status]._empty){
            return _button_spirits[_status];
        }
        else{
            return _button_spirits[0];
        }
    }

template <typename... T>
    void activate(T&... args) {
        auto arg_list = std::make_tuple(args...);
        int i = 0;

        for (auto func_ptr : _func_ptrs) {
            std::visit([&](auto&& func) mutable {
                using T_determine = std::decay_t<decltype(func)>;
                
                if constexpr (std::is_same_v<T_determine, std::function<void()>>) {
                    func();
                } else {
                    callFunctionWithTupleArg(func, arg_list, i);
                    i++;
                }
            }, *func_ptr);
        }
    }

    /**
     * @return in the format:
     * name#mode#status#clickRatio.x#clickRatio.y#clickPosition.x#clickPosition.y##spirit0##spirit1##spirit2##spirit3##func0##func1##
     */
    std::string encrypt(){
        std::string save_string = "";
        // name#
        save_string += Tools::wordEncrypt(_nickname);
        save_string += '#';
        // mode#
        // ============= need to check ================
        // will std::to_chars(int) give the int in char or the corresponding char in position int?
        save_string += std::to_string(_mode);
        save_string += '#';
        // status#
        save_string += std::to_string(_status);
        save_string += '#';
        // clickRatio#
        save_string += std::to_string(_click_ratio.x);
        save_string += '#';
        save_string += std::to_string(_click_ratio.y);
        save_string += '#';
        // clickPosition##
        save_string += std::to_string(_click_position.x);
        save_string += '#';
        save_string += std::to_string(_click_position.y);
        save_string += "##";

        for(int i = 0; i < 4; i++){
            if(_button_spirits.at(i)._empty == true){
                save_string.append(2, '#');
            }
            else{
                save_string += _button_spirits.at(i).encrypt();
            }
        }

        // the function name / id 

        return save_string;
    }
};