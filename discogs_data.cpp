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


void database_adapt::add_entry(int index, int id, double price, int quantity){
    entry e;
    e.id = id;
    e.lowest_price = price;
    e.amount_available = quantity;
    int64_t timestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    e.timestamp = timestamp;

    if (index > db.size()) {
        db.push_back(e);
    } else {
        if (!check_same(index, id)) {
            insert_at(e, index);
        }
        else {
            db[index] = e;
        }
    }
}

bool database_adapt::check_same(int internal_index, int id) {
    if (db[internal_index].id == id) {
        return true;
    } else return false;
}  

void database_adapt::insert_at(entry e, int internal_index) {
    std::vector<entry> temp = {};
    for (int i = 0 ; i < internal_index-1; i++){
        temp.push_back(db[i]);
    }
    temp.push_back(e);
    for (int j = internal_index-1; j < db.size(); j++) {
        temp.push_back(db[j]);
    }
    db = temp;
    temp.clear();
}

bool database_adapt::check_history(int id, int size){
    bool check_stock = false;
    for (int i = 0 ; i < db.size(); i++) {
        if (id == db[i].id && db[i].amount_available > size) {
            check_stock = true;
        }
    }
    return check_stock;
}
