#include "Tools.hpp"
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