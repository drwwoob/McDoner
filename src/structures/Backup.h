#pragma once
#include <stack>
#include <string>
#include "Data.h"

using namespace std;    // yes i got lazy

class Backup {
public:
    stack<string> backup_data;
    stack<string> forward_data;

    void undo(Data &game_data, int& page_at){
        forward_data.emplace(backup_data.top());
        backup_data.pop();
        game_data.decryptFile(backup_data.top(), page_at);
    };
    bool undoAbailible(){
        return backup_data.empty();
    };
    void addMove(Data &game_data, int& page_at){
        backup_data.emplace(game_data.encryptIntoFile(page_at));
        forward_data = {};
    };
    void cancelUndo(Data &game_data, int& page_at){
        backup_data.emplace(forward_data.top());
        game_data.decryptFile(forward_data.top(), page_at);
        forward_data.pop();
    };
    bool cancelAvailible(){
        return forward_data.empty();
    };
};