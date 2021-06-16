#ifndef __SEND_EMAIL_H__
#define __SEND_EMAIL_H__
#include "discogs_data.hpp"
#include "find_deals.hpp"
#include "define_email.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

struct email {
    std::string url;
    std::string user_email;
    std::string user_pass;
    std::string mail_from;
    std::string mail_rcpt;
};

class send_emails {
    private: 
        email e;
        void delete_file();
        void write_email (); 
        void set_email_param();
        wantlist_entry we;
        price_entry pe;
        std::string img;
        std::string body_loc = "send.txt";
    public:
        std::string send_data();
        send_emails(wantlist_entry we_, price_entry pe_, std::string image) {
            we = we_;
            pe = pe_;
            img = image;
        }
};
 
#endif