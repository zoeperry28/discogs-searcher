#ifndef __FIND_DEALS_H__
#define __FIND_DEALS_H__
#include "discogs_data.hpp"



class deal {
    private:
        price_stats prices;
        price_entry find_basic_deal() ;
        price_entry calculate_average_price();
        void database_entry();
    public:
        price_entry best_price() ;
        deal(price_stats  ps) {
            prices = ps;
        }
};


#endif