#pragma once
#include <string>
#include <functional>
#include <imgui.h>
#include <variant>
#include "Spirit.hpp"
#include <iostream>
#include <fstream>
#include <cstring> // For memcpy

// This is used for std::variant to have std::function<void<Page&>>,
// I am thinking of this one and Data&, but can't really think of a reason why
// class Page;

class Button {
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
	ImVec2 _click_ratio;	// the ratio of the clickbox
	ImVec2 _click_position; // the position of the clickbox
	/** 
     * button mode:
     * 0: clickable
     * 1: unclickable
     * // 2: stay-clicked
     * @details stay-clicked: this seems kinda 
     * unreasonable as long as i check if there is a clicked spirit
    */
	int _mode;
	// button status:
	//  0: unclick
	//  1: selected
	//  2: onclick
	//  3: clicked
	int _status;
	bool _show_area = false;
	ImU32 _click_area_color = 0x4C8B3333;

	// <template> ?
	using FunctionVariant = std::variant<
		// functions to change values
		// I think there should be some setted funtion for this
		std::function<void()>,
		// std::function<void(int&)>, // i think this function could do the "jump to another page" with something like void<int& _page_at>
		std::function<void(double&)>,
		std::function<void(bool&)>,
		std::function<void(std::string&)>
		// std::function<void(Page*)>
		>;
	std::vector<std::shared_ptr<FunctionVariant>> _func_ptrs;

	void addFunction(std::shared_ptr<FunctionVariant> func_ptr) {
		_func_ptrs.push_back(func_ptr);
	}

	/**
     * creating a blank Button
     * ============== remember to check for similar naming while creating ==============
     * i am thinking of making this id a random number while saving into library
     * or a detection made when the id is generated
     */

	Button(): _nickname("button"), _mode(0), _status(0), _click_ratio(), _click_position() {
		_button_spirits.at(0)._spirit_name = "unclick";
		_button_spirits.at(1)._spirit_name = "selected";
		_button_spirits.at(2)._spirit_name = "onclick";
		_button_spirits.at(3)._spirit_name = "clicked";
	}
	Button(int id): _status(0) {
		_nickname = "button" + std::to_string(id);

		// changing the name for buttons
		_button_spirits.at(0)._spirit_name = "unclick";
		_button_spirits.at(1)._spirit_name = "selected";
		_button_spirits.at(2)._spirit_name = "onclick";
		_button_spirits.at(3)._spirit_name = "clicked";
	}

	/**
     * decrypt a string into a button
     */
	Button(const std::string& data) {
		size_t start = 0;

		// get the stored items
		do {
			start = data.find("##", start + 1);
		} while(data.at(start - 1) == '/');
		start += 2;

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
			if(index < _button_spirits.size()) {
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
		for(int i = 0; i < 4; i++) {
			// Find the delimiter "##" and ensure it’s properly handled
			do {
				end = data.find("##", end);
				if(end == std::string::npos) break; // Handle case where delimiter not found
				end += 2;							// Move past the delimiter
			} while(data[end - 3] == '/'); // Make sure the delimiter is correct

			// Extract the data block for decryption
			data_block = data.substr(start, end - start);
			// Use the lambda function to process the extracted data block
			Tools::decrypt<6>(data_block, [this, &spiritCreate, i](std::array<std::string, 6>& separate_data) {
				spiritCreate(separate_data, i);
			});

			start = end; // Update start for the next section
		}
		// get the functions stored

		// changing the name for buttons
		_button_spirits.at(0)._spirit_name = "unclick";
		_button_spirits.at(1)._spirit_name = "selected";
		_button_spirits.at(2)._spirit_name = "onclick";
		_button_spirits.at(3)._spirit_name = "clicked";
	}

	/**
     * @param path the path of img inside the project
     * @details existence check in import and select phase
     */
	void setUnclickImg(const std::string& path) {
		// if(std::filesystem::exists(path)){
		// }
	}

	void setSelectedImg(const std::string& path) {
	}

	void setOnclickImg(const std::string& path) {
	}

	void setClickedImg(const std::string& path) {
	}

	void save(std::string& save_string) {
	}

	bool mouseOver(int mouse_ratio_x, int mouse_ratio_y, bool clicking) {
		bool over = mouse_ratio_x > _click_position.x && mouse_ratio_x < (_click_position.x + _click_ratio.x) && mouse_ratio_y > _click_position.y && mouse_ratio_y < (_click_position.y + _click_ratio.y);

		if(over) {
			if(_mode == 0) {
				// if(_status == 0){
				if(over) {
					_status = 1;
				}
				// }
			}
			else if(_mode == 1) {
				// do nothing for now
			}
		}

		return over;
	}

	Spirit currentSpirit() {
		// if no image is at the status

		switch(_mode) {
		case 0:
			if(_button_spirits[_status]._empty) {
				return _button_spirits[0];
			}
			else {
				return _button_spirits[_status];
			}
			break;
		case 1:
		default:
			return _button_spirits[0];
			break;
		}
	}

	template <typename... T>
	void activate(T&... args) {
		auto arg_list = std::make_tuple(args...);
		int i = 0;

		for(auto func_ptr : _func_ptrs) {
			std::visit([&](auto&& func) mutable {
				using T_determine = std::decay_t<decltype(func)>;

				if constexpr(std::is_same_v<T_determine, std::function<void()>>) {
					func();
				}
				else {
					callFunctionWithTupleArg(func, arg_list, i);
					i++;
				}
			},
					   *func_ptr);
		}
	}

	/**
     * @return in the format:
     * name#mode#status#clickRatio.x#clickRatio.y#clickPosition.x#clickPosition.y##spirit0##spirit1##spirit2##spirit3##func0##func1##
     */
	std::string encrypt() {
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

		for(int i = 0; i < 4; i++) {
			if(_button_spirits.at(i)._empty == true) {
				save_string.append(2, '#');
			}
			else {
				save_string += _button_spirits.at(i).encrypt();
			}
		}

		// the function name / id

		return save_string;
	}

	void serialize(std::ofstream& out) const {
		// Serialize nickname
		size_t name_size = _nickname.size();
		out.write(reinterpret_cast<const char*>(&name_size), sizeof(size_t));
		out.write(_nickname.data(), name_size);

		// Serialize mode, status, click ratio, and click position
		out.write(reinterpret_cast<const char*>(&_mode), sizeof(int));
		out.write(reinterpret_cast<const char*>(&_status), sizeof(int));
		out.write(reinterpret_cast<const char*>(&_click_ratio), sizeof(ImVec2));
		out.write(reinterpret_cast<const char*>(&_click_position), sizeof(ImVec2));

		// Serialize button spirits
		for(const auto& spirit : _button_spirits) {
			spirit.serialize(out);
		}

		// Serialize function pointers
		// This part requires a custom approach if you need to store them.
	}

	void deserialize(std::ifstream& in) {
		// Deserialize nickname
		size_t name_size;
		in.read(reinterpret_cast<char*>(&name_size), sizeof(size_t));
		_nickname.resize(name_size);
		in.read(&_nickname[0], name_size);

		// Deserialize mode, status, click ratio, and click position
		in.read(reinterpret_cast<char*>(&_mode), sizeof(int));
		in.read(reinterpret_cast<char*>(&_status), sizeof(int));
		in.read(reinterpret_cast<char*>(&_click_ratio), sizeof(ImVec2));
		in.read(reinterpret_cast<char*>(&_click_position), sizeof(ImVec2));

		// Deserialize button spirits
		for(auto& spirit : _button_spirits) {
			spirit.deserialize(in);
		}

		// Deserialize function pointers
		// Implement custom deserialization if needed.
	}
};