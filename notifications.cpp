#include "notifications.hpp"
#include <experimental/filesystem>
#include <fstream>

price_entry deal::best_price() {
    price_entry p1 = find_basic_deal();
    price_entry p2 = calculate_average_price();
    if (p1.price > p2.price) {
        return p1;
    } 
    else if (p1.price < p2.price) {
        return p2;
    }
    else {
        return p1; // equal
    }
}

price_entry deal::find_basic_deal() {
    price_entry pe;
    int index = -1;
    int tier = 4;
    for (int i = 0 ; i < prices.price_entries.size(); i++) {
        // valid price - availabe to the uk 
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
    if (index != -1) {
        return prices.price_entries[index];
    } else {
        pe.price = -1;
        pe.link = "";
        return pe;
    }
}

price_entry deal::calculate_average_price(){
    double avg = 0;
    for (int i = 0 ; i < prices.price_entries.size(); i++){
        avg = avg + prices.price_entries[i].price;
    }
    avg = avg / prices.price_entries.size();

    price_entry pe;
    pe.price = -1;
    for (int i = 0 ; i < prices.price_entries.size() ; i++) {
        if (prices.price_entries[i].price <  avg) {
            pe = prices.price_entries[i];
        }
    }

    if (pe.price != -1) {
        return pe;
    }
    else {
        pe.link = "";
        pe.price = -1;
        return pe;
    }
}

