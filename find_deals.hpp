#ifndef __FIND_DEALS_H__
#define __FIND_DEALS_H__
#include "discogs_data.hpp"


class deal {
    private:
        price_stats prices;
    public:

        price_entry find_basic_deal() ;
        double calculate_average_price();
        deal(price_stats  ps) {
            prices = ps;
        }
};

#endif