#ifndef AOC_UTILITY_RESULT_PRINTER_HPP
#define AOC_UTILITY_RESULT_PRINTER_HPP

#include <iostream>

template<typename TSilverResultType, typename TGoldResultType>
void print_result(const TSilverResultType& silverResult, const TGoldResultType& goldResult)
{
    std::cout << "Silver result: " << silverResult << "\n";
    std::cout << "Gold result:   " << goldResult << "\n";
}

void exit_with_error(const std::string& error_message)
{
    std::cerr << "Failed puzzle: " << error_message << "\n";
    exit(-1);
}


#endif