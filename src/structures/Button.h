#include <string>
#include <functional>
#include <imgui.h>

class Button{
public:
    std::string _name;
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

    Button(int id){
        _name = "button" + std::to_string(id);
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

    void draw(){
        
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

    // =============== alter way for above ==============
    // using ParameterVarient = std::variant<
    //     int&,
    //     double&,
    //     std::string&
    // >;
    // void activate(std::vector<ParameterVarient> args){
    //     int i = 0;

    //     for (auto func_ptr : _func_ptrs){
    //         std::visit([&](auto&& arg) {
    //             using T = std::decay_t<decltype(arg)>;
                
    //             if constexpr (std::is_same_v<T, std::function<void(void)>>) {
    //             } 
    //             else{
    //                 (*func_ptr)(std::get<i>(arg_list));
    //                 i++;
    //             }
    //             // else if constexpr (std::is_same_v<T, std::function<void(int&)>>) {
    //             //     std::cout << "Type is std::function<void(int)>" << std::endl;
    //             // } else if constexpr (std::is_same_v<T, std::function<void(double&)>>) {
    //             //     std::cout << "Type is std::function<void(double)>" << std::endl;
    //             // } else if constexpr (std::is_same_v<T, std::function<void(std::string&)>>) {
    //             //     std::cout << "Type is std::function<void(int, double)>" << std::endl;
    //             // }
    //         }, *func_ptr);
    //     }
    // }

    /**
     * @return in the format:
     * name#mode#status#clickRatio[.x#clickRatio.y#clickPosition.x#clickPosition.y##spirit0##spirit1##spirit2##spirit3##func0##func1##
     */
    std::string encrypt(){
        std::string save_string = "";
        for(int i = 0; i < 4; i++){
            if(_button_spirits.at(i)._empty == true){
                save_string.append(7, '#');
            }
            else{
                save_string += _button_spirits.at(i).encrypt();
            }
        }

    }
};

