#pragma once
#include <stack>
#include <string>
#include "Data.h"

using namespace std;    // yes i got lazy

class Backup {
public:
    stack<string> backup_data;
    stack<string> forward_data;

    void undo(Data &game_data){
        forward_data.emplace(backup_data.top());
        backup_data.pop();
        game_data.decryptFile(backup_data.top());
    };
    bool undoAvailible(){
        return backup_data.size() > 1;
    };
    void addMove(Data &game_data){
        backup_data.emplace(game_data.encryptIntoFile());
        forward_data = {};
    };
    void redo(Data &game_data){
        backup_data.emplace(forward_data.top());
        game_data.decryptFile(forward_data.top());
        forward_data.pop();
    };
    bool redoAvailible(){
        return !(forward_data.empty());
    };
};