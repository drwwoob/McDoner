#include "Tools.h"
#include <stdexcept>
#include <vector>

template <std::size_t Size>
void Tools::decrypt(const std::string& data_block, const std::function<void(std::array<std::string, Size>&)>& func) {
    // Ensure the array size matches the provided size
    if (Size == 0) {
        throw std::runtime_error("Array size must be greater than 0.");
    }

    std::array<std::string, Size> separate_data{};
    std::size_t current_index = 0;

    for (std::size_t i = 0; i < data_block.size(); i++) {
        switch (data_block[i]) {
            case '/':
                i++;
                separate_data[current_index] += data_block[i];
                break;
            case '#':
                current_index++;
                if (current_index >= Size) {
                    i++;
                    func(separate_data); 
                    separate_data.fill(""); // Reset the array
                    current_index = 0;
                }
                break;
            default:
                separate_data[current_index] += data_block[i];
                break;
        }
    }
}


// #include "Tools.h"
// #include <iostream>
// #include <stdexcept>
// #include <functional>
// #include <array>
// #include <vector>

// template <typename T>
// void Tools::decrypt(const std::string& data_block, int decrypt_type, T& insert_object) {
//     std::function<void(std::array<std::string, 6>&)> insertInto;
//     size_t size = 0;
    
    // switch (decrypt_type) {
    //     case 0:
    //         insertInto = [&](std::array<std::string, 6>& separate_data) {
    //             insert_object = T(
    //                 separate_data[0],
    //                 separate_data[1],
    //                 std::stof(separate_data[2]),
    //                 std::stof(separate_data[3]),
    //                 std::stof(separate_data[4]),
    //                 std::stof(separate_data[5])
    //             );
    //         };
    //         size = 6;
    //         break;
    //     case 1:
    //         // Define other cases if needed
    //         break;
    //     case 2:
    //         insertInto = [&](const std::array<std::string, 7>& seperate_data){
    //             _nickname = seperate_data[0];
    //             _mode = std::stoi(seperate_data[1]);
    //             _status = std::stoi(seperate_data[2]);
    //             _click_ratio = ImVec2(std::stof(seperate_data[3]), std::stof(seperate_data[4]));
    //             _click_position = ImVec2(std::stof(seperate_data[5]), std::stof(seperate_data[6]));
    //         };
    //         size = 7;
    //         break;
    //     default:
    //         throw std::runtime_error("Decrypt type error: given " + std::to_string(decrypt_type));
    // }

//     std::array<std::string, 6> separate_data{""};
//     int current_index = 0;

//     for (size_t i = 0; i < data_block.size(); i++) {
//         switch (data_block.at(i)) {
//             case '/':
//                 i++;
//                 separate_data[current_index] += data_block.at(i);
//                 break;
//             case '#':
//                 current_index++;
//                 if (current_index >= size) {
//                     insertInto(separate_data);
//                     separate_data.fill("");
//                     current_index = 0;
//                 }
//                 break;
//             default:
//                 separate_data[current_index] += data_block.at(i);
//                 break;
//         }
//     }

//     std::cout << "============ put in ================" << std::endl;
// }
