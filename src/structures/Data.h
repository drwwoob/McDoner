#pragma once

#include <map>
#include "Page.h"
#include "Tools.h"
//#include "shobjidl_core.h"

class data
{
public:
	data(std::string file_path);		// creating a blank file
	void newFile(bool* start_visual);
	void openFile(bool* start_visual);	// reading a file
	void openDemo(bool* start_visual);	// open the demo file
	//void openFile(std::string path);
	Page* getPage(int page_id);	// get the information of a page (decrypt)
	int leaveAt = 0;	// the page that the user left at last time
	void setFont(ImFont* font_given);
	std::string filedata() { return fileData; };
	void save();
	int pageSize() { return pages.size(); };
	void addPage(int page_id);
	void CopyPage(int page_id, Page page);
	void deletePage(int page_id);

private:
	ImFont* font;
	std::vector<Page> pages;	// a file includes a collection of pages
	std::string fileName;
	std::string fileData;
	std::string filePath;
	//std::map<Page> pages;

	// a hash table that stores all pages? or array?

	//void readData(std::string name);
	std::string encryptIntoFile(); // encrypt the current pages into text
	void decryptFile(std::string data_str);
	//from microsoft example code
	HRESULT basicFileOpen(bool findFile); // The tool to open common window
};