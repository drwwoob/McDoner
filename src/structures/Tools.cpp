#include "Tools.h"
#include <iostream>
#include <string>


void Tools::pageBar()
{
}

std::string Tools::wordEncrypt(std::string word) {
	std::string EncryptedWord;
	for (const auto letter : word) {
		switch (letter) {
		case '#':
			EncryptedWord.append("/#");
			break;
		case '/':
			EncryptedWord.append("//");
			break;
		case '{':
			EncryptedWord.append("/{");
			break;
		case '}':
			EncryptedWord.append("/}");
			break;
		default:
			EncryptedWord.push_back(letter);
		}

	}
	return EncryptedWord;
}

void Tools::setBackground(std::string background_name)
{
}