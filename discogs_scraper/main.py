#!/usr/bin/python

import sys
import pyautogui
import time
import datetime
import os
import re
import getpass
import data_obtained
import webbrowser

dump = "/home/" + getpass.getuser() + "/Desktop/discogs-searcher/html_dump/"

def path () :
    if not os.path.exists(dump):
        os.makedirs(dump)
# get marketplace listings for the item 
def obtain_url_from_id(id):
    url = "https://www.discogs.com/sell/release/" + id + "?sort=price%2Casc&limit=250&ev=rb&page=1"
    return url;

## ubuntu - get to page with the given id
def control_from_chrome(url):
    #open chromium
    webbrowser.open_new_tab(url)
    time.sleep(10) # change to 10 for pi

def save_html_file(id):
    #save the html of the current page
    pyautogui.hotkey('ctrlright','s');
    time.sleep(10) # 10 for pi
    # delete current name
    pyautogui.hotkey('ctrlright','a');
    pyautogui.keyDown('backspace');
    pyautogui.keyUp('backspace');
    

    html_name = dump + id 
    if os.path.exists(html_name + ".html"):
        os.remove(html_name + ".html")
    
    pyautogui.typewrite(html_name);
    time.sleep(2)
    pyautogui.keyDown('enter');
    pyautogui.keyUp('enter');
    time.sleep(2)
    return html_name;

def exctract_price_from_line(line) :
    trig = 0
    price_found = ""
    for x in line:
        if (x == '£'): 
            trig = 1
        elif ((x == " " and trig == 1)):
            return price_found
        elif (trig == 1):
            price_found = price_found + x
    if (price_found == ""):
        return -1;
    else :
        return price_found

def hasNumbers(inputString):
    return any(char.isdigit() for char in inputString)
# first 3 are low, med and high, rest as listings. 
# 50 on a page

def get_item_link(line):
    link = "";
    trigger = 0;
    inc = 0
    for i in line:
        if (trigger == 1 and i == "\""):
            trigger = 0;
            return link[1:len(link)], inc+2
        if (i == "\""):
            trigger = 1;
        if (trigger == 1) :
            link = link + i;
        inc = inc + 1;
            

def get_image(line, end_index):
    new_line = line[end_index:len(line)]
    image_link = "";
    cumulative_check = "";
    trig = 0;
    for i in new_line:
        cumulative_check = cumulative_check + i
        if (trig == 1 and i == "\""):
            return image_link 
        elif (trig == 1):
            image_link = image_link + i
        if (cumulative_check.find("data-src=\"") != -1 and trig == 0):
            trig = 1;
        
## get deal link and the image
def extract_additional (link_lines) :
    item_link = [];
    image_link = ""
    for line in link_lines:
        item = get_item_link(line);
        item_link.append(item[0])
        image_link = get_image(line,item[1])
    return item_link, image_link;
    
def get_availability_info(fiIn):
    available = []
    for line in fiIn:
        if (line.find("shortcut_navigable") != -1): # then this is a line we want to look at
            if (line.find("unavailable") != -1) :
                available.append(0);
            else:
                available.append(1)
    return available

def verify_prices(price_list) :
    new_price_list = []
    for i in range (len(price_list)):
        ind = price_list[i].index('.')
        if (len(price_list[i]) - ind != 2) :
            new_price_list.append(price_list[i][0:ind+3])
        else :
            new_price_list.append(price_list[i])
    return new_price_list[::2]
            
def keep_valid_link (available, links):
    valid_links = []
    i = 0;
    for avail in available: 
        if (avail == 1):
            valid_links.append(links[i])
        i = i + 1;
    return valid_links


def get_prices_on_current_page(file_loc):
# Get links for item listings
    ## IMPORTANT! You can get both the link AND the image from here! very good for emails!
    link_lines = []
    price_lines = []
    new_lines = []
    lmh = []
    fiIn = open((file_loc + ".html")).readlines()
    for lines in fiIn:
        if (lines.find("class=\"item_description_title\" data-followable=\"true\">") != -1):
            link_lines.append(lines)
        if ((lines.find("total") != -1) and (lines.find("£") != -1)) :
            price_lines.append(lines)
        elif ((lines.find("£") != -1) and (len(lmh) != 3)) :
            lmh.append(lines)


    result = extract_additional(link_lines);
    #clean up the lines to get the prices from the html
    i = 0;

    price_lines = price_lines + new_lines
    for found in price_lines:
        found = found.strip()
        price_lines[i] = exctract_price_from_line(found)
        i = i + 1
    price_lines[:] = (value for value in price_lines if value != -1)
    
    # this covers the case where an item may have never been sold
    ava = get_availability_info(fiIn)

    lnk = keep_valid_link(ava, result[0])
    price_lines = verify_prices(price_lines)

    for x in range(len(lmh)):
        lmh[x] = lmh[x].strip()
        lmh[x] = exctract_price_from_line(lmh[x])

    if (len(price_lines) + len(lmh) != 0) :
        pb = data_obtained.Price_Brackets(lmh[0],lmh[1], lmh[2], price_lines ,[], ava, [])
        pb.links = lnk
        pb.image = ""
        pb.other = price_lines
    
    return pb;

def create_file_for_cpp_use(p_b):
    brackets = "{\"low\":" + p_b.low + ",\"med\":" + p_b.med + ",\"high\":" + p_b.high + ",\"image\":\"" + p_b.image +"\","; 
    prices = "\"prices\":["

    for x in range(len(p_b.other)):
        prices = prices + "{ \"price\":" + p_b.other[x] + ", \"link\":\""+  p_b.links[x] + "\"}";
        if (x != len(p_b.other)-1) :
            prices = prices + ","

    prices = prices + "]}";
    fullstr = brackets  + prices;
    f = open("data.json", "w")
    f.write(fullstr)
    f.close()
    time.sleep(3)


def main(argv):
    time.sleep(5)
    path()
    if (len(argv) == 1):
        url = obtain_url_from_id(argv[0])
        control_from_chrome(url);
        name = save_html_file(argv[0])
        check = get_prices_on_current_page(name);
        create_file_for_cpp_use(check)
        time.sleep(3)
    else : # DW = 3015526, DD = 5623194
        url = obtain_url_from_id("778278")
        control_from_chrome(url);
        name = save_html_file("778278")
        check = get_prices_on_current_page(name);
        create_file_for_cpp_use(check)
        time.sleep(3)
        #print ("Invalid arguments given!");
        #exit();

if __name__ == "__main__":
    main(sys.argv[1:])