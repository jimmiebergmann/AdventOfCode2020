#include "result_printer.hpp"
#include "file_reader.hpp"

int64_t solve_silver(const std::vector<int64_t>& values)
{
    for (size_t i = 0; i < values.size(); i++)
    {
        for (size_t j = i + 1; j < values.size(); j++)
        {
            auto sum = values[i] + values[j] ;
            if (sum == 2020)
            {
                return values[i] * values[j] ;
            }
        }
    }

    exit_with_error("Failed to find solution with 2 depths.");
    return 0;
}

int64_t solve_gold(const std::vector<int64_t>& values)
{
    for (size_t i = 0; i < values.size(); i++)
    {
        for (size_t j = i + 1; j < values.size(); j++)
        {
            for (size_t k = j + 1; k < values.size(); k++)
            {
                auto sum = values[i] + values[j] + values[k];
                if (sum == 2020)
                {
                    return values[i] * values[j] * values[k];
                }
            }
        }
    }

    exit_with_error("Failed to find solution with 3 depths.");
    return 0;
}

int main()
{
    auto values = read_file_lines<int64_t>("day_1_input.txt");
    if(values.empty())
    {
        exit_with_error("Failed to read input file.");
    }

    auto silver_result = solve_silver(values);
    auto gold_result = solve_gold(values);

    print_result(silver_result, gold_result);
}