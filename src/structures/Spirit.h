#pragma once
#include <string>
#include "Tools.h"
//#include <winnt.h>

class Spirit
{
private:
	std::string spiritFileName;
	//PWSTR imageAddress;
	//ImVec2 size;
	
public:
	std::string spiritName;
	float sizeRatio[2];
	//ImVec2 Position;
	float positionRatio[2]; // the ratio of (the coord for lefttop of the picture / the whole window)
	// constructor while a new spirit is added
	Spirit(const std::string file_name) {		// creating a new spirit
		// since changing nick-name should not change file name, this file_name is passed in as a copy
		spiritName = file_name;
		spiritFileName = file_name;
		sizeRatio[0] = 1.0;  // default height is the image's origin height
		sizeRatio[1] = 1.0;  // default weight is the image's origin height
		positionRatio[0] = 0.25; // default position in the 1/4 of the window
		positionRatio[1] = 0.25; // same as above
	}
	// constructor for read in data
	Spirit(const std::string& name, const std::string& file_name, const float& sr_x,const float& sr_y, const float& pr_x, const float& pr_y) {
		spiritName = name;
		spiritFileName = file_name;
		sizeRatio[0] = sr_x;
		sizeRatio[1] = sr_y;
		positionRatio[0] = pr_x;
		positionRatio[1] = pr_y;

	}
	// reading a string as a spirit
	//void change(PWSTR address_after_change);
	void change(ImVec2 size_after_change){}

	std::string getFileName() {
		return spiritFileName;
	}

	std::string* getRealNickName() {
		return &spiritName;
	}

	// int win_x, int win_y
	ImVec2 getSize(int x, int y) {
		return ImVec2(sizeRatio[0] * x, sizeRatio[1] * y);
	}

	ImVec2 getPosition(int x, int y) {
		return ImVec2(positionRatio[0] * x, positionRatio[1] * y);
	}

	// encryption follows the format
	// SpiritName#sizeRatio[0]#sizeRatio[1]#positionRatio[0]#positionRatio[1]##
	std::string toString() {
		std::string dataEncr;

		// pass in name
		// dataEncr += spiritName;

		dataEncr += Tools::wordEncrypt(spiritName);

		dataEncr += "#";
		dataEncr += Tools::wordEncrypt(spiritFileName);
		dataEncr += "#";
		dataEncr += std::to_string(sizeRatio[0]);
		dataEncr += "#";
		dataEncr += std::to_string(sizeRatio[1]);
		dataEncr += "#";
		dataEncr += std::to_string(positionRatio[0]);
		dataEncr += "#";
		dataEncr += std::to_string(positionRatio[1]);
		dataEncr += "##";

		return dataEncr;
	}

	// some getter
	std::string fileName() { return spiritFileName; }
	std::string name() { return spiritName; }
};

