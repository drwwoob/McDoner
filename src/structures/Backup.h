#pragma once
#include <stack>
#include <string>
#include <Data.h>

using namespace std;    // yes i got lazy

class Backup {
    stack<string> backup_data;
    stack<string> forward_data;

    void undo(Data &game_data, int& page_at){

    };
};