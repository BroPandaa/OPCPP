#include "../include/strutils.h"
#include <cctype>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>


// Vaš kod dolazi ovdje.

std::string to_upper(std::string str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::string to_lower(std::string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

std::string reverse(std::string str)
{
    reverse(str.begin(), str.end());
    return str;
}

std::string trim_left(std::string str, std::string znakovi)
{
    std::string novi=str, prazan;
    if(novi== " \t\n\v") return prazan;

    auto i=novi.find_first_not_of(znakovi);

    novi.erase(0, i);
    return novi;
}

std::string trim_right(std::string str, std::string znakovi)
{
    std::string novi=str, prazan;
    if(novi== " \t\n\v") return prazan;

    auto i=novi.find_last_not_of(znakovi)+1;

    novi.erase(i);
    return novi;
}

std::string trim(std::string str, std::string znakovi)
{
    std::string novi=str, prazan;
    if(novi== " \t\n\v") return prazan;

    return trim_right(trim_left(novi, znakovi), znakovi);
}

std::string remove(std::string str, char c)
{
    std::string novi=str;
    novi.erase(std::remove(novi.begin(), novi.end(), c), novi.end());
    return novi;
}

std::vector<std::string> tokenize(std::string const & str, char delimiter)
{
    std::vector<std::string> rezultat;
    std::stringstream podaci(str);
    std::string rijec, prazan; 

    while(getline(podaci, rijec, delimiter))
    {
        if(rijec==prazan) continue;
        rezultat.push_back(rijec);
        }

    return rezultat;
}