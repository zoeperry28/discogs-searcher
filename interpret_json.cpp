#include "interpret_json.hpp"

nlohmann::json json_from_discogs::DISCOGS_Get_Wantlist(user_data * ud){
    std::string unformatted = d->get_user_wantlist();
    nlohmann::json test = nlohmann::json::parse(unformatted);

    int total_items = test["pagination"]["items"];
    int per_page = test["pagination"]["per_page"];
    int added = 0;
    int counter = 0;

    while (added != total_items) {
        wantlist_entry we;
        
        we.internal_list_index = added+1;
        we.id =  test["wants"][counter]["basic_information"]["id"];
        we.resource_location = test["wants"][counter]["basic_information"]["resource_url"];
        we.title = test["wants"][counter]["basic_information"]["title"];
        we.artist = test["wants"][counter]["basic_information"]["artists"][0]["name"];
        we.format = test["wants"][counter]["basic_information"]["formats"][0]["name"];
        if (test["wants"][counter]["basic_information"]["formats"][0].contains("text")){
            we.describ = test["wants"][counter]["basic_information"]["formats"][0]["text"];
        } else {
            we.describ = "Standard";
        }
        std::vector<std::string> other;
        for (int i = 0 ; i < test["wants"][counter]["basic_information"]["formats"][0]["descriptions"].size(); i++){
            other.push_back(test["wants"][counter]["basic_information"]["formats"][0]["descriptions"][i]);
        }
        we.additional_details = other;
        ud->all_wants.push_back(we);

        if (counter+1 != per_page) {
            counter++;
        } else {
            std::string new_page = test["pagination"]["urls"]["next"];
            unformatted = d->page_contents(new_page);
            test = nlohmann::json::parse(unformatted);
            counter = 0;
            
        }
        added++;
    }
    ud->show_all_data();
}

