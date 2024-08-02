#pragma once
#include <stack>
#include <string>
#include "Data.h"

// template <typename T>

// struct changedLog{
//     int changed_type;
//     int changed_page_at;
//     string encrypted_changed;
// };

class Backup {
public:
    Backup(std::shared_ptr<std::shared_ptr<Data>> game_data_ptr) : game_data_ptr(game_data_ptr){};
    ~Backup(){};

    void undo(){
        forward_data.emplace(backup_data.top());
        backup_data.pop();
        std::cout << (*game_data_ptr)->pageSize();
        (*game_data_ptr)->decryptFile(backup_data.top());
    };

    /**
     * @details changed_type:
     *              0: setting
     *              1: page(change its content)
     *              2: structural (add/delete page)
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
        return backup_data.size() > 1;
    };
    void addMove(){
        backup_data.emplace((*game_data_ptr)->encryptIntoFile());
        forward_data = {};
    };
    // void addMove(const int& changed_type, const int& page_at, const std::string& changed_data){
    //     backup_data.emplace(changed_type, page_at, changed_data);
    //     forward_data = {};
    // };
    void redo(){
        backup_data.emplace(forward_data.top());
        (*game_data_ptr)->decryptFile(forward_data.top());
        forward_data.pop();
    };
    bool redoAvailible(){
        return !(forward_data.empty());
    };
private:
    std::shared_ptr<std::shared_ptr<Data>> game_data_ptr;
    std::stack<std::string> backup_data;
    std::stack<std::string> forward_data;
    // stack<tuple<int, int, T>> backup_data;
    // stack<tuple<int, int, T>> forward_data;
    // stack<changedLog> backup_data;
    // stack<changedLog> forward_data;
};