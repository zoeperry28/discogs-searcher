#ifndef __DISCOGS_REQUESTS_H__
#define __DISCOGS_REQUESTS_H__
#include <stdio.h>
#include <iostream>

class discogs {
    public:
        bool check_running();
        std::string get_user_wantlist();
        std::string by_wantlist_id(int id);
        std::string page_contents(std::string url);
        std::string get_lowest_price_from_id(int id);
        std::string get_price_suggestions_by_id(int id);
        discogs (std::string username, std::string token_) {
            user = username;
            token = token_;
            token_url_to_append = token_url_to_append + token;
        }
    private:
        std::string token_url_to_append = "&token=";
        std::string base_url = "https://api.discogs.com/";
        std::string user;
        std::string token;
};

#endif