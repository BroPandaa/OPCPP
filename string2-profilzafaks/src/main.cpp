#include "../include/strutils.h"
#include <iostream>
#include <string>

int main()
{
   // Vaši testovi dolaze ovdje.  
    std::string proba;
    char znak;
    std::cin >> proba;
    std::cout << to_lower(proba) << std::endl;

    std::cin >> proba;
    std::cout << to_upper(proba) << std::endl;

    std::cin >> proba;
    std::cout << reverse(proba) <<std::endl;
    
    std::cin >> proba;
    std::cout << trim_left(proba) << std::endl;

    std::cin >> proba;
    std::cout << trim_right(proba) << std::endl;

    std::cin >> proba;
    std::cout << trim(proba) << std::endl;
    
    std::cin >> proba >> znak;
    std::cout << remove(proba, znak) << std::endl;

    std::cin >> proba >> znak;
    std::vector<std::string> novi=tokenize(proba, znak);

    for(int i=0; i<novi.size(); i++)
    {
        std::cout << novi[i]<< std::endl;
    }

    return 0;
}
