#include "stdafx.h"
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include "windows.h"
#include <fstream>

#include "utils.h"

std::string ToLower(std::string data){
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    return data;
};

std::string ToUpper(std::string data){
    std::transform(data.begin(), data.end(), data.begin(), ::toupper);
    return data;
};

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (item.length() != 0){
            elems.push_back(item);
        }
    }
    return elems;
};

std::vector<std::string> StringSplit(std::string input_str, char delim)
{
    std::vector<std::string> parts;
    split(input_str, delim, parts);

    return parts;
};

std::string StringJoin(std::vector<std::string> string_vector, char delim, bool remove_empty)
{
    std::string out_string;
    std::vector<std::string>::iterator it = string_vector.begin();
    //if its a blank string
    if (it == string_vector.end())
    {
        return "";
    }

    //remove empty strings
    if (remove_empty == true)
    {
        string_vector.erase(std::remove(string_vector.begin(),
                    string_vector.end(), ""), string_vector.end());
    }

    //add the first part of the vector, so that there's no leading delimiter
    out_string+=*it;
    ++it;
    for(it; it != string_vector.end(); ++it){
        out_string+=(delim+*it);
    }  

    return out_string;

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

    // if (str_answer == ""){
    //     return default_str;
    // }
    // else {
    return str_answer;
    // }
};

std::string get_images_path()
{
    //ex: C:\Users\Mark\Documents\Visual Studio
    //2010\Projects\FirstCPPApplication\Debug\BiochRL++.exe
    std::string exepath = get_exe_path();
    std::vector<std::string> exesplit = StringSplit(exepath, kPathSeparator);

    exesplit.pop_back();
    std::string data = "images";
    data+=kPathSeparator;
    exesplit.push_back(data);

    return StringJoin(exesplit, kPathSeparator);
};

std::string get_data_path()
{
    //ex: C:\Users\Mark\Documents\Visual Studio
    //2010\Projects\FirstCPPApplication\Debug\BiochRL++.exe
    std::string exepath = get_exe_path();
    std::vector<std::string> exesplit = StringSplit(exepath, kPathSeparator);

    exesplit.pop_back();
    std::string data = "data";
    data+=kPathSeparator;
    exesplit.push_back(data);

    return StringJoin(exesplit, kPathSeparator, false);
};

std::string get_exe_path() 
{
    TCHAR exepath[MAX_PATH+1];
    GetModuleFileName(0, exepath, MAX_PATH+1);

    //TCHAR* qwe = exepath;
    //std::basic_string<TCHAR> path = qwe;

    //TODO Fix failure on accented chars in path
#ifdef UNICODE
    std::wstring wide( exepath );  
    std::string path( wide.begin(), wide.end() );
#else
    std::string path = exepath;
#endif

    return path;
};

void move_window(int x, int y)
{
    RECT rcWind;
    HWND hwnd = FindWindow(NULL, _T("FirstCPPApplication"));
    GetWindowRect(hwnd, &rcWind);
    MoveWindow(hwnd, x, y, rcWind.right - rcWind.left, rcWind.bottom - rcWind.top, true);
};

std::string char_to_str(char chr)
{
    std::stringstream ss;
    ss << chr;
    std::string temp_str;
    ss >> temp_str;

    return temp_str;

};

// void stripNullsFromVector(std::vector<T*>* vect)
// {
//     vect->erase(
//             std::remove_if(
//                 vect->begin(),
//                 vect->end(),
//                 [](T s) { return s == NULL; }),
//             vect->end());
// };

// template<class T, class T2>
// bool isInVector<T, T2>(T vect, T2 elem)
// {
// 	return std::find(vect.begin(), vect.end(), elem)!=vect.end();
// };
