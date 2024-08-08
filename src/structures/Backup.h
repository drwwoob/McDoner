#pragma once
#include <stack>
#include <string>
#include "Data.h"

class Backup {
public:
    Backup(std::shared_ptr<std::shared_ptr<Data>> game_data_ptr) : _game_data_ptr(game_data_ptr){};
    ~Backup(){};

    void undo(){
        _forward_data.emplace(_backup_data.top());
        _backup_data.pop();
        // std::cout << (*_game_data_ptr)->pageSize();
        (*_game_data_ptr)->decryptFile(_backup_data.top());
    };

    /**
     * @details changed_type:
     *              0: Setting
     *              1: Page(change its content)
     *              2: Structural (add/delete page)
     *              3: Button Interface
     *              4: Spirit Library
     */
    // void undo(Data& (*game_data_ptr)){
        // forward_data.emplace(backup_data.top());
        // backup_data.pop();
        // switch (backup_data.top().changed_type){
        // case 0:
        //     /* ========== future edition ========= */
        //     break;
        // case 1:
        //     (*game_data_ptr).getPage(backup_data.top().changed_page_at)
        //     break;
        // default:
        //     break;
    //     }
    // };
    bool undoAvailible(){
        return _backup_data.size() > 1;
    };
    void addMove(){
        _backup_data.emplace((*_game_data_ptr)->encryptIntoFile());
        _forward_data = {};
    };
    // void addMove(const int& changed_type, const int& page_at, const std::string& changed_data){
    //     backup_data.emplace(changed_type, page_at, changed_data);
    //     forward_data = {};
    // };
    void redo(){
        _backup_data.emplace(_forward_data.top());
        (*_game_data_ptr)->decryptFile(_forward_data.top());
        _forward_data.pop();
    };
    bool redoAvailible(){
        return !(_forward_data.empty());
    };
private:
    std::shared_ptr<std::shared_ptr<Data>> _game_data_ptr;
    std::stack<std::string> _backup_data;
    std::stack<std::string> _forward_data;
    // stack<tuple<int, int, T>> backup_data;
    // stack<tuple<int, int, T>> forward_data;
    // stack<changedLog> backup_data;
    // stack<changedLog> forward_data;
};