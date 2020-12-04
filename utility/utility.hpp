#ifndef AOC_UTILITY_UTILITY_HPP
#define AOC_UTILITY_UTILITY_HPP

#include "result_printer.hpp"
#include "file_reader.hpp"
#include <functional>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <regex>
#include <array>
#include <map>
#include <set>

namespace utils
{

    template<typename ... TSolvers>
    void solve_puzzles(const std::string& input_filename, TSolvers ... solvers)
    {
        size_t part = 0;
        const auto data = read_file_lines<std::string>(input_filename);
        (solve_puzzle(data, ++part, solvers), ...);
    }

    template<typename TReturnType, typename TDataType>
    void solve_puzzle(const std::vector<std::string>& data, const size_t part, TReturnType(*solver)(const std::vector<TDataType>&))
    {
        try
        {
            std::vector<TDataType> transformed_data;
            std::transform(data.begin(), data.end(), std::back_inserter(transformed_data),
                [](const std::string& d)
            {
                TDataType value{};
                auto result = std::from_chars(d.data(), d.data() + d.size(), value);
                if (result.ec != std::errc())
                {
                    throw std::exception("Failed to convert data types for input data.");
                }
                return value;
            });

            auto now = std::chrono::high_resolution_clock::now();
            auto result = solver(transformed_data);
            auto complete = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(complete - now);

            std::cout << "Part " << part << " result: " << result << "    time: " << duration.count() << " us" << "\n";
        }
        catch (puzzle_exception& e)
        {
            std::cerr << "Failed to solve part " << part << ": " << e.what() << std::endl;
        }
    }

    template<typename TReturnType>
    void solve_puzzle(const std::vector<std::string>& data, const size_t part, TReturnType(*solver)(const std::vector<std::string>&))
    {
        try
        {
            auto now = std::chrono::high_resolution_clock::now();
            auto result = solver(data);
            auto complete = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(complete - now);

            std::cout << "Part " << part << " result: " << result << "    time: " << duration.count() << " us" << "\n";
        }
        catch (puzzle_exception& e)
        {
            std::cerr << "Failed to solve part " << part << ": " << e.what() << std::endl;
        }
        catch(std::exception& e)
        {
            std::cerr << "Internal error on " << part << ": " << e.what() << std::endl;
        }
    }

}

#endif