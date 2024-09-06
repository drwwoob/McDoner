#include "Data.hpp"

#include <fstream>
#include <iostream>

const std::shared_ptr<Library> Data::_library_ptr = std::make_shared<Library>();

Data::Data() {
}

Data::Data(const std::string& file_path) {
	// initiate path
	_project_name = file_path.substr(file_path.find_last_of("/") + 1, file_path.find_last_of(".") - file_path.find_last_of("/") - 1);
	_project_path = file_path.substr(
		0,
		file_path.find_last_of("/") + 1);

	// ============= this is canceled for now, might need to create another Library for this ==============
	// or else it would require an extra value to store which one belongs to what file
	// // reading system library
	// if(std::filesystem::exists("../src/settings/library.txt")){
	//     *_library_ptr->
	// }

    loadBinary();
    // loadString();
}

Data::Data(const std::string& project_path, const char* project_name) {
	_project_path = project_path + "/";
	std::filesystem::create_directory(project_path);

	_project_name = project_name;
	_pages = {Page(_library_ptr)};
	_page_at = 0;
}

Data::~Data() {
	delete _font;
}

void Data::loadString(){
	// set up file_data holder
	std::ifstream file;
	std::string file_data = "";
	std::string line;

	// reading project library
	// don't think this would need to be checked once i fully implement Library save method
	if(std::filesystem::exists(_project_path + "library.txt")) {
		file.open(_project_path + "library.txt", std::ios::in);
		while(std::getline(file, line)) {
			file_data += line;
		}
		_library_ptr->decryptDataBinary(file_data);
		file.close();
	}

	// reading save
	file.open(_project_path + _project_name + ".txt", std::ios::in);
	file_data = "";
	line = "";
	while(std::getline(file, line)) {
		file_data += line;
	}
	decryptFile(file_data);
	file.close();
}

void Data::setFont(ImFont* font_given) {
	_font = font_given;
	//for (int i = 0; i < pages.size(); i++) {
	//    pages.at(i).setFont(font_given);
	//}
}

void Data::loadTexture() {
	_textures = _pages.at(_page_at).loadPage(_project_path);
}

void Data::draw() {
	_pages.at(_page_at).drawPage(_textures, 0);
}

Page* Data::getPage(int page_id) {
	return &_pages.at(page_id);
}

Page* Data::getCurrentPage(){
	return &_pages.at(_page_at);
}

std::string Data::encryptIntoFile() {
	std::string pagesInfo;
	pagesInfo.append(std::to_string(_page_at) + ",");
	for(auto page : _pages) {
		pagesInfo.append(page.encrpyt());
	}

	return pagesInfo;
}

void Data::decryptFile(const std::string& data_str) {
	// clear all old data
	// have to default font here
	_pages.clear();

	// get new data
	int pos = 0;
	int start = 0;
	pos = data_str.find(",");
	int count = 0;
	_page_at = std::stoi(data_str.substr(0, pos));
	while(pos < data_str.size()) {
		if(data_str.at(pos) == '/') {
			pos++;
		}
		if(data_str.at(pos) == '[') { // whether pos is '[', go to the next letter
			start = pos;
			count++; // making sure it's in the same scope
		}
		else if(data_str.at(pos) == ']') {
			count--;
			if(count == 0) {
				_pages.emplace_back(data_str.substr(start, pos - start), _library_ptr);
				_pages.back().setFont(_font);
			}
		}
		pos++;
	}
}

void Data::importFormattedPages(const std::string& path) {
}

void Data::changeProjectName() {
}

void Data::save() const {
	std::ofstream outFile(_project_path + _project_name + ".bin", std::ios::binary);
	if(!outFile) {
		throw std::runtime_error("Could not open file for writing");
	}

	// Serialize _page_at
	outFile.write(reinterpret_cast<const char*>(&_page_at), sizeof(_page_at));

	// Serialize _pages
	size_t pagesSize = _pages.size();
	outFile.write(reinterpret_cast<const char*>(&pagesSize), sizeof(pagesSize));
	for(const auto& page : _pages) {
		// Assuming Page has a method to serialize to binary
		page.serialize(outFile);
	}

	// Serialize _values
	size_t valuesSize = _values.size();
	outFile.write(reinterpret_cast<const char*>(&valuesSize), sizeof(valuesSize));
	for(const auto& [key, value] : _values) {
		size_t keySize = key.size();
		outFile.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
		outFile.write(key.c_str(), keySize);

		// Serialize the variant type and value
		uint8_t index = value.index();
		outFile.write(reinterpret_cast<const char*>(&index), sizeof(index));
		std::visit([&outFile](auto&& arg) {
			outFile.write(reinterpret_cast<const char*>(&arg), sizeof(arg));
		},
				   value);
	}

	outFile.close();
}

void Data::exportToString() {
	std::ofstream file;
	//for testing
	auto path = _project_path + _project_name + ".txt";
	file.open(path);
	file << encryptIntoFile().c_str();
	file.close();

	path = _project_path + "library.txt";
	file.open(path);
	file << _library_ptr->encrypt().c_str();
	file.close();
}

void Data::addPage(int page_id, Page page) {
	_pages.emplace(_pages.begin() + page_id, page);
}

void Data::CopyPage(int page_id, Page page) {
	for(int i = 0; i < page._textboxs.size(); i++) {
		page._textboxs.at(i)._content = "Enter new text";
	}
	_pages.insert(_pages.begin() + page_id, page);
}

void Data::deletePage(int page_id) {
	_pages.erase(_pages.begin() + page_id);
}

void Data::loadSettings() {
	loadFormattedPages();
}

void Data::loadFormattedPages() {
	// guess i have to update this
	// // load default interfaces
	// std::ifstream file("../src/settings/buttonInterface.txt", std::ios::in);
	// std::string settings = "";
	// std::string line;
	// while(std::getline(file, line)) {
	//     settings += line;
	// }

	// // load specified interfaces
	// if(std::filesystem::exists(_project_path + "buttonInterface.txt")){
	//     file.open(_project_path + "buttonInterface.txt", std::ios::in);
	//     while(std::getline(file, line)) {
	//         settings += line;
	//     }
	// }

	// // decrypt settings string
}

    void Data::loadBinary() {
        std::ifstream in(_project_path + _project_name + ".bin");

        // Deserialize _page_at
        in.read(reinterpret_cast<char*>(&_page_at), sizeof(_page_at));

        // Deserialize _font (assuming _font is nullptr for now)
        _font = nullptr;  // As _font was not serialized, we set it to nullptr

        // Deserialize _pages
        size_t pages_size;
        in.read(reinterpret_cast<char*>(&pages_size), sizeof(pages_size));
        _pages.resize(pages_size);
        for (auto& page : _pages) {
            page.deserialize(in);
        }

        // Deserialize _values
        size_t values_size;
        in.read(reinterpret_cast<char*>(&values_size), sizeof(values_size));
        for (size_t i = 0; i < values_size; ++i) {
            // Deserialize key (string)
            size_t key_size;
            in.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));
            std::string key;
            key.resize(key_size);
            in.read(&key[0], key_size);

            // Deserialize value (ParameterVarient)
            ParameterVarient value;
            int type;
            in.read(reinterpret_cast<char*>(&type), sizeof(type));

            switch (type) {
                case 0: { // int
                    int int_value;
                    in.read(reinterpret_cast<char*>(&int_value), sizeof(int_value));
                    value = int_value;
                    break;
                }
                case 1: { // double
                    double double_value;
                    in.read(reinterpret_cast<char*>(&double_value), sizeof(double_value));
                    value = double_value;
                    break;
                }
                case 2: { // bool
                    bool bool_value;
                    in.read(reinterpret_cast<char*>(&bool_value), sizeof(bool_value));
                    value = bool_value;
                    break;
                }
                case 3: { // string
                    size_t string_size;
                    in.read(reinterpret_cast<char*>(&string_size), sizeof(string_size));
                    std::string string_value;
                    string_value.resize(string_size);
                    in.read(&string_value[0], string_size);
                    value = string_value;
                    break;
                }
                default:
                    throw std::runtime_error("Unknown type in ParameterVarient");
            }

            _values.emplace(std::move(key), std::move(value));

            // std::cout << "loaded data" << std::endl;
        }

        // _library_ptr is a static pointer and doesn't need to be deserialized here.
    }