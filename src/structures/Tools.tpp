#include "Tools.h"
#include <iostream>
#include <stdexcept>
#include <functional>
#include <array>
#include <vector>

template <typename T>
void Tools::decrypt(const std::string& data_block, int decrypt_type, const T& insert_object) {
    std::function<void(std::array<std::string, 6>&)> insertInto;
    size_t size = 0;
    
    switch (decrypt_type) {
        case 0:
            insertInto = [&](std::array<std::string, 6>& separate_data) {
                insert_object(
                    separate_data[0],
                    separate_data[1],
                    std::stof(separate_data[2]),
                    std::stof(separate_data[3]),
                    std::stof(separate_data[4]),
                    std::stof(separate_data[5])
                );
            };
            size = 6;
            break;
        case 1:
            // Define other cases if needed
            break;
        default:
            throw std::runtime_error("Decrypt type error: given " + std::to_string(decrypt_type));
    }

    std::array<std::string, 6> separate_data{""};
    int current_index = 0;

    for (size_t i = 0; i < data_block.size(); i++) {
        switch (data_block.at(i)) {
            case '/':
                i++;
                separate_data[current_index] += data_block.at(i);
                break;
            case '#':
                current_index++;
                if (current_index >= size) {
                    insertInto(separate_data);
                    separate_data.fill("");
                    current_index = 0;
                }
                break;
            default:
                separate_data[current_index] += data_block.at(i);
                break;
        }
    }=

    std::cout << "============ put in ================" << std::endl;
}
