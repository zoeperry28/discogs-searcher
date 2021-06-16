#ifndef __DATA_FROM_WEBPAGE_H__
#define __DATA_FROM_WEBPAGE_H__
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "json.hpp"
#include "discogs_data.hpp"

class json_to_structure {
    public:     
        price_stats obtain_item_data();
    private: 
        std::string filename = "data.json";
        nlohmann::json j;
        price_stats read_json_from_file () ;

};

#endif