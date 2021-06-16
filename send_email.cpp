#include "send_email.hpp"

std::string send_emails::send_data() {
    set_email_param();
    write_email();
    std::string command = "curl --ssl-reqd \
    --url '" + e.url + "' \
    --user '" + e.user_email + ":" + e.user_pass + "' \
    --mail-from '" + e.mail_from + "' \
    --mail-rcpt '" + e.mail_rcpt + "' \
    --upload-file " + body_loc ;
    system(command.c_str());
    delete_file();
    return pe.link;
}

void send_emails::set_email_param(){
    e.url = "smtps://smtp.gmail.com:465";
    e.user_email = TO_USER;
    e.user_pass = USER_PASS;
    e.mail_from = FROM_USER;
    e.mail_rcpt = TO_USER;
}

void send_emails::write_email(){
    std::ofstream myfile;
    myfile.open (body_loc);
    myfile << "From: \"Discogs Bot\" <" + e.mail_from + ">\n"; 
    myfile << "To: \"Discogs Bot User\" <" +  e.mail_rcpt + ">\n";
    myfile << "Subject: " << we.title << " by " << we.artist << ": £" << pe.price << "!\n\n";
    myfile << "Hello!\n" << we.title << " by " << we.artist << " is now available at £" << pe.price << "!\n" << "The link can be found here: " << pe.link << std::endl;
    myfile.close();
}

void send_emails::delete_file () {
    std::string x = "rm " + body_loc;
    system(x.c_str());
}
