#ifndef __DISCOGS_DATA_H__
#define __DISCOGS_DATA_H__
#include <vector>
#include <iostream>
#include <stdio.h>
#include "discogs_requests.hpp"
#include "json.hpp"
/*
* for now only need enough information to tell the user what release is being referenced. 
*/ 



struct price_entry { 
    double price;
    std::string link;
    bool available;
};

struct price_stats {
    double low;
    double med;
    double high;
    std::string image;

    std::vector<price_entry> price_entries;
};

struct wantlist_entry {
    int id;
    int internal_list_index;
    std::string resource_location;
    std::string title;
    std::string artist;
    std::string format;
    std::string describ; 
    std::vector<std::string> additional_details;
};

class user_data {
    public: 
        //std::vector<std::string> all_sent = {};
        void show_all_data();
        void Current_From_ID(int id) ;
        static double Price_String_to_Double(std::string price);
        std::vector<wantlist_entry> all_wants;  
        int Internal_to_ID (int internal);
        struct gathered_info { 
            wantlist_entry album;
            price_stats prices;
        } album_info;

};

class pricing_info {
    public: 
        nlohmann::json j;
        void Get_Prices_From_Json(price_stats * price);
        void Read_Json();
};

#endif