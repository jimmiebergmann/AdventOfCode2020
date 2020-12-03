#ifndef AOC_UTILITY_RESULT_PRINTER_HPP
#define AOC_UTILITY_RESULT_PRINTER_HPP

#include <iostream>
#include <exception>
#include <string>

namespace utils
{

    struct puzzle_exception : public std::exception
    {

        puzzle_exception(const std::string& message) :
            std::exception(message.c_str())
        {}

    };

    template<typename ... TResults>
    void print_results(const TResults& ... results)
    {
        size_t part = 0;
        ((std::cout << "Part " << (++part) << " result: " << results << "\n"), ...);

    }

}

#endif