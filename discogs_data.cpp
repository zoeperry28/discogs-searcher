#include "discogs_data.hpp"
#include <fstream>
void user_data::show_all_data(){
    std::string data_present = "";
    for (int i = 0 ; i < all_wants.size(); i++) {
        data_present = data_present + std::to_string(all_wants[i].internal_list_index) +": " + all_wants[i].title + ", " + all_wants[i].artist + ", " + all_wants[i].format + ", " + all_wants[i].describ + "\n"; 
    }
    std::cout << data_present;
}

int user_data::Internal_to_ID (int internal) {
    return all_wants[internal-1].id;
}

double user_data::Price_String_to_Double(std::string price) {
    return std::stod(price);
}

void pricing_info::Read_Json() {
    std::ifstream ifs("data.json");
    j = nlohmann::json::parse(ifs);
}
