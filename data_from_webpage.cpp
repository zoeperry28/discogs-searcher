#include "data_from_webpage.hpp"

price_stats json_to_structure::obtain_item_data() {
    std::ifstream ifs(filename);
    j = nlohmann::json::parse(ifs);
    price_stats ps = read_json_from_file();
    return ps;
}

price_stats json_to_structure::read_json_from_file () {
    price_stats ps;
    ps.high =j["high"];
    ps.med = j["med"];
    ps.low = j["low"];
    ps.image = j["image"];

    for (int i = 0 ; i < j["prices"].size(); i++) {
        // create a new entry 
        price_entry pe;
        pe.price = j["prices"][i]["price"];
        pe.link = j["prices"][i]["link"];

        if (j["prices"][i]["availability"] == 1) {
            pe.available = true;
        } else {
            pe.available = false;
        }

        ps.price_entries.push_back(pe);
    }
    std::cout << "done!\n";
    return ps;
}