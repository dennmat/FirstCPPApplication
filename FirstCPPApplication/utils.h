#ifndef UTILS_H
#define UTILS_H
#pragma once

#include <sstream>
#include <vector>
#include <iostream>

std::string ToLower(std::string data);
std::string ToUpper(std::string data);
void print_str_array(std::string msg_list[], int len);
int ask_for_int(std::string msg, int default_int=0);
std::string ask_for_str(std::string msg, std::string default_str="STRING");
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> StringSplit(std::string input_str, char delim);
std::string StringJoin(std::vector<std::string> string_vector, char delim);
std::string get_exe_path();
std::string get_data_path();
std::string get_images_path();

const char kPathSeparator =
#ifdef _WIN32
                            '\\';
#else
                            '/';
#endif

#endif
