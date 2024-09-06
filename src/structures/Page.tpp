template <typename T>
T Page::findElement(std::pair<int, std::string> name_obj) {
    switch(name_obj.first){
        case 1:
            for(auto& spirit : _spirits){
                if(spirit._spirit_file_name == name_obj.second){
                    return spirit;
                }
            }
            return NULL;
        default:
            return NULL;
    }
}