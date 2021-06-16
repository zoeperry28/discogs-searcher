#ifndef __PARSE_JSON_H__
#define  __PARSE_JSON_H__
#include "json.hpp"
#include "discogs_data.hpp"
#include <fstream>
#include <stdio.h>

class interpret_json {
    public:
        nlohmann::json String_To_Json(std::string from_request);
    private:
        nlohmann::json j; 
        const std::string TEMP_NAME = "temp.json";

};

class json_from_discogs {
    private:
        interpret_json ij;
        discogs * d;
    public:
        nlohmann::json DISCOGS_Get_Wantlist(user_data * ud);
  
        json_from_discogs(std::string user, std::string token) {
            d = new discogs(user, token);
        }
};

#endif