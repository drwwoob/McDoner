#include "Page.hpp"

// the save data file for the user()
class SaveInfo{
public:
    int _page_at;
    // fullfillment, something that records the number / percentage of pages view...?
    using ParameterVarient = std::variant<
        int,
        double,
        bool,
        std::string,
        Page*
    >;
    std::map<std::string, ParameterVarient> _values;
    std::string _save_file_name; // the name of the saved file

    /**
     * @overload constuctor for saveInfo
     */
    SaveInfo(int page_at, std::map<std::string, ParameterVarient> values)
    :
    _page_at(page_at),
    _values(values){

    }

    /**
     * @overload constuctor for saveInfo
     * @param save_file the name of the file that holds the saves of this game
     */
    SaveInfo(std::string save_file)
    :
    _save_file_name(save_file){

    }
    /**
     * write the current data into the save file's given slot
     * @param slot_id the slot where the save is writing to
     */
    void writeToSave(int slot_id){

    }
}