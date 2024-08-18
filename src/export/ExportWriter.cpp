#include "ExportWriter.hpp"
#include <fstream>

bool ExportWriter::windowWriter(const Data& data, std::string& message){
    std::ofstream file(data._filename, std::ios::out);
}
