#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <vector>

std::string ToLower(std::string data);
std::string ToUpper(std::string data);

void print_str_array(std::string msg_list[], int len);

//old asking for inputs
int ask_for_int(std::string msg, int default_int=0);
std::string ask_for_str(std::string msg, std::string default_str="STRING");

//splitting and joining
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> StringSplit(std::string input_str, char delim);
std::string StringJoin(std::vector<std::string> string_vector, char delim, bool remove_empty = false);

//getting various paths
std::string get_exe_path();
std::string get_data_path();
std::string get_images_path();

// class T;
// void stripNullsFromVector(std::vector<T*>* vect);

std::string char_to_str(char chr);

void move_window(int x, int y);

inline const char * const BoolToString(bool b, bool verbose=true)
{
    if (verbose)
    return b ? "true" : "false";
    else
    return b ? "T" : "F";
}

const char kPathSeparator =
#ifdef _WIN32
                            '\\';
#else
                            '/';
#endif

#endif
