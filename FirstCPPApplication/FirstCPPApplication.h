
#pragma once

#include <sstream>

void WelcomeMessage();
std::string ToLower(std::string data);
std::string ToUpper(std::string data);
void print_str_array(std::string msg_list[], int len);
int ask_for_int(std::string msg, int default_int=0);
std::string ask_for_str(std::string msg, std::string default_str="STRING");

