#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>

#include "utils.h"

std::string ToLower(std::string data){
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    return data;
};

std::string ToUpper(std::string data){
	std::transform(data.begin(), data.end(), data.begin(), ::toupper);
    return data;
};


void print_str_array(std::string msg_list[], int len){

    std::cout << len << std::endl;
    for (int i = 0; i < len; i++){
    std::cout << msg_list[i] << std::endl;
    };
};

int ask_for_int(std::string msg, int default_int){

    // std::cout << msg << std::endl;
    std::cout << msg << std::endl << ">>> ";
    std::string str_answer;
    getline(std::cin, str_answer);
    if (str_answer == ""){
    return default_int;
    }
    else {
    int int_answer = atoi(str_answer.c_str());
    return int_answer;
    }
};

std::string ask_for_str(std::string msg, std::string default_str){

    std::cout << msg << std::endl << ">>> ";
    std::string str_answer;
    getline(std::cin, str_answer);

    if (str_answer == ""){
    return default_str;
    }
    else {
    return str_answer;
    }
};
