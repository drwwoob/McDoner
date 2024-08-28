#include "ExportWriter.hpp"
#include <fstream>

#define INDENT std::setw(_indent * 2) << ""

bool ExportWriter::windowWriter(const Data& data, std::string& message){
    std::ofstream file(data._filename, std::ios::out);

    // file 
    // << 
}

