#include "discogs_data.hpp"
#include "discogs_requests.hpp"
#include "interpret_json.hpp"
#include <fstream>
#include "data_from_webpage.hpp"
#include "notifications.hpp"
#include "send_email.hpp"
#include <chrono>
#include <string>
#include <iostream>

std::string get_token_from_file () {
    std::string to_return = "";
    std::ifstream file;

    file.open("token");
    std::string line;

    if (!file) {
        std::cerr << "error: could not find the token.\n";
        exit(-1);
    }
    else {
        if (std::getline(file, line)) to_return = line; // get and print the line.
        file.close(); // remember to close the file.
    }

    if (to_return != "") {
        return to_return;
    } else {
        std::cerr << "error: invalid token given! did you set up the configuration properly?\n";
    }
}

void run_python(int id) {
    std::string run_command = "/usr/bin/python3 discogs_scraper/main.py " + std::to_string(id);
    system(run_command.c_str());
}

int main() {

    database_adapt da;
    std::vector <std::string> all_links;
    std::string token = get_token_from_file();
    discogs * d = new discogs("zoeperry28", token);
    json_from_discogs * j = new json_from_discogs("zoeperry28", token);
    //test that the token works

    if (!d->check_running()) {
        std::cerr << "error: token not working. generate another token and try again.\n";
    } else { // token confirmed to work - can begin analysis
        while (true) {
            user_data ud;
            j->DISCOGS_Get_Wantlist(&ud);
            size_t item_total = ud.all_wants.size();
			
            // go through all items in the wantlist
            for (int i = 0 ; i < item_total; i++) {
                run_python(ud.all_wants[i].id);
            
                ud.album_info.album = ud.all_wants[i];
                pricing_info pi; 
                pi.Read_Json();
                json_to_structure * jts = new json_to_structure();
                price_stats found = jts->obtain_item_data();
                // now we have all the available info for the current album.
                // from here we can analyse all of the prices retrieved from the page
                deal d(found);
                price_entry best_deal = d.best_price();
                // now we have the best contender for a good deal on the item, it can be checked to see if it is valid
                // also check that link hasnt already been sent
                if ((best_deal.price != -1) && (std::find(all_links.begin(), all_links.end(),best_deal.link) == all_links.end())) { // valid
                    // a new, valid link
                    send_emails e(ud.all_wants[i], best_deal, found.image);
                    std::string link_found = e.send_data();
                    all_links.push_back(link_found);
                }
                da.add_entry(ud.album_info.album.internal_list_index, ud.all_wants[i].id, best_deal.price, found.price_entries.size());
                system("pkill -o chromium");
            }
        }
    }
}
