// This class only have inline header file,
// and it stores the information of a single spirit.

#pragma once
#include <string>
#include "Tools.hpp"
#include <imgui.h>
#include <fstream>


class Spirit {
   public:
    std::string _spirit_file_name; // storing the address of the file of this spirit
    std::string _spirit_name;
    float _size_ratio[2];
    float _position_ratio[2]; // the ratio of (the coord for lefttop of the picture / the whole window)
    bool _empty = true;

    Spirit(){};
    // constructor while a new spirit is added
    Spirit(const std::string file_name) { // creating a new spirit
        // since changing nick-name should not change file name, this file_name is passed in as a copy
        _spirit_name = file_name;
        _spirit_file_name = file_name;
        _size_ratio[0] = 1.0;      // default height is the image's origin height
        _size_ratio[1] = 1.0;      // default weight is the image's origin height
        _position_ratio[0] = 0.25; // default position in the 1/4 of the window
        _position_ratio[1] = 0.25; // same as above
        _empty = false;
    }
    // constructor for read in Data
    Spirit(const std::string& name, const std::string& file_name, const float& sr_x, const float& sr_y, const float& pr_x, const float& pr_y) {
        _spirit_name = name;
        _spirit_file_name = file_name;
        _size_ratio[0] = sr_x;
        _size_ratio[1] = sr_y;
        _position_ratio[0] = pr_x;
        _position_ratio[1] = pr_y;
        _empty = false;
    }
    // reading a string as a spirit
    //void change(PWSTR address_after_change);
    void change(ImVec2 size_after_change) {}

    std::string getFileName() {
        return _spirit_file_name;
    }

    // int win_x, int win_y
    ImVec2 getSize(int x, int y) {
        return ImVec2(_size_ratio[0] * x, _size_ratio[1] * y);
    }

    ImVec2 getPosition(int x, int y) {
        return ImVec2(_position_ratio[0] * x, _position_ratio[1] * y);
    }

    // encryption follows the format
    // SpiritName#spiritFileName#sizeRatio[0]#sizeRatio[1]#positionRatio[0]#positionRatio[1]##
    std::string encrypt() {
        std::string dataEncr;

        // pass in name
        // dataEncr += spiritName;

        dataEncr += Tools::wordEncrypt(_spirit_name);

        dataEncr += "#";
        dataEncr += Tools::wordEncrypt(_spirit_file_name);
        dataEncr += "#";
        dataEncr += std::to_string(_size_ratio[0]);
        dataEncr += "#";
        dataEncr += std::to_string(_size_ratio[1]);
        dataEncr += "#";
        dataEncr += std::to_string(_position_ratio[0]);
        dataEncr += "#";
        dataEncr += std::to_string(_position_ratio[1]);
        dataEncr += "##";

        return dataEncr;
    }

    void serialize(std::ofstream& outFile) const {
        // Serialize _spirit_file_name
        size_t fileNameSize = _spirit_file_name.size();
        outFile.write(reinterpret_cast<const char*>(&fileNameSize), sizeof(fileNameSize));
        outFile.write(_spirit_file_name.c_str(), fileNameSize);

        // Serialize _spirit_name
        size_t spiritNameSize = _spirit_name.size();
        outFile.write(reinterpret_cast<const char*>(&spiritNameSize), sizeof(spiritNameSize));
        outFile.write(_spirit_name.c_str(), spiritNameSize);

        // Serialize _size_ratio
        outFile.write(reinterpret_cast<const char*>(&_size_ratio), sizeof(_size_ratio));

        // Serialize _position_ratio
        outFile.write(reinterpret_cast<const char*>(&_position_ratio), sizeof(_position_ratio));

        // Serialize _empty
        outFile.write(reinterpret_cast<const char*>(&_empty), sizeof(_empty));
    }

    void deserialize(std::ifstream& inFile) {
        // Deserialize _spirit_file_name
        size_t fileNameSize;
        inFile.read(reinterpret_cast<char*>(&fileNameSize), sizeof(fileNameSize));
        _spirit_file_name.resize(fileNameSize);
        inFile.read(&_spirit_file_name[0], fileNameSize);

        // Deserialize _spirit_name
        size_t spiritNameSize;
        inFile.read(reinterpret_cast<char*>(&spiritNameSize), sizeof(spiritNameSize));
        _spirit_name.resize(spiritNameSize);
        inFile.read(&_spirit_name[0], spiritNameSize);

        // Deserialize _size_ratio
        inFile.read(reinterpret_cast<char*>(&_size_ratio), sizeof(_size_ratio));

        // Deserialize _position_ratio
        inFile.read(reinterpret_cast<char*>(&_position_ratio), sizeof(_position_ratio));

        // Deserialize _empty
        inFile.read(reinterpret_cast<char*>(&_empty), sizeof(_empty));
    }
};
