#include "find_deals.hpp"

price_entry deal::find_basic_deal() {
    price_entry pe;
    int index = -1;
    int tier = 4;
    for (int i = 0 ; i < prices.price_entries.size(); i++) {
        // valid price - availabe to the uk 
        if (prices.price_entries[i].available == true) {
            pe = prices.price_entries[i];
            if (pe.price <= prices.high && tier > 3) {
                tier = 3;
                index = i;
            }
            else if (pe.price <= prices.med  && tier > 2) {
                tier = 2;
                index = i;
            }
            else if (pe.price <= prices.low  && tier > 1) {
                tier = 1;
                index = i;
            } 
        }
    }
    if (index != -1) {
        std::cout << "hello!\n";
        return prices.price_entries[index];
    } else {
        pe.price = -1;
        pe.link = "invalid";
        pe.available = false;
        return pe;
    }
}

double deal::calculate_average_price(){
    double avg = 0;
    for (int i = 0 ; i < prices.price_entries.size(); i++){
        avg = avg + prices.price_entries[i].price;
    }
    avg = avg / prices.price_entries.size();

    // can then find outliers and hence determine a deal this way as well. 
}