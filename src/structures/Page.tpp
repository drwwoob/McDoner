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

template <typename T>
void Page::serializeVector(std::vector<T> vector_to_save, std::ofstream& outFile) const{
    size_t obj_size = vector_to_save.size();
    outFile.write(reinterpret_cast<const char*>(&obj_size), sizeof(obj_size));
    for(const auto& obj : vector_to_save){
        obj.serialize(outFile);
    }
}

template <typename T>
void Page::deserializeVector(std::vector<T> vector_to_read, std::ifstream& inFile){
    size_t obj_size;
    inFile.read(reinterpret_cast<chat*>(&obj_size), sizof(obj_size));
    vector_to_read.resize(obj_size);
    for(auto& obj_to_read : vector_to_read ){
        obj_to_read.deserialize(inFile);
    }
}