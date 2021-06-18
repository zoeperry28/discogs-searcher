#include "discogs_requests.hpp"
#include "interpret_json.hpp"
#include <curl/curl.h>

std::string discogs::get_lowest_price_from_id(int id) {
    std::string url = base_url + "marketplace/" + "stats/" + std::to_string(id) + "?GBP" + token_url_to_append;
    return page_contents(url);
}

std::string discogs::by_wantlist_id(int id){
    std::string url = base_url + "users/" + user + "/wants" + std::to_string(id);
    return page_contents(url);
}

std::string discogs::get_user_wantlist(){
    std::string url = base_url + "users/" + user + "/wants";
    return page_contents(url);
}

bool discogs::check_running(){
    std::string test_url = base_url + "releases/249504";
    if (page_contents(test_url.c_str()).size() != 0){
        return true;
    }
    return false;
}

size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::string *s) {
    size_t newLength = size*nmemb;
    try {
        s->append((char*)contents, newLength);
    }
    catch(std::bad_alloc &e) {
        //handle memory problem
        return 0;
    }
    return newLength;
}

std::string discogs::page_contents(std::string url) { 
    CURL *curl;
	CURLcode res;
    std::string s; 
    int to_return = 0;
	curl = curl_easy_init();
	if (!curl) { return ""; }
    
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	/* example.com is redirected, so we tell libcurl to follow redirection */
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Dark Secret Ninja/1.0");     
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //only for https
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); //only for https
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	/* Perform the request, res will get the return code */
	res = curl_easy_perform(curl);
	/* Check for errors */
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        to_return = -1;
	}

	/* always cleanup */
	curl_easy_cleanup(curl);

	return s;
}
