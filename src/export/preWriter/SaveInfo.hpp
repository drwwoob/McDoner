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

    SaveInfo(int page_at, std::map<std::string, ParameterVarient> values)
    :
    _page_at(page_at),
    _values(values)
    {};
}