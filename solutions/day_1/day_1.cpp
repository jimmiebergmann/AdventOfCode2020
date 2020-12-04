#include "utility.hpp"

int64_t solve_part_1(const std::vector<int64_t>& values)
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

    throw utils::puzzle_exception("Failed to find solution with 2 depths.");
}

int64_t solve_part_2(const std::vector<int64_t>& values)
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

    throw utils::puzzle_exception("Failed to find solution with 3 depths.");
}

int main()
{
    /*auto values = utils::read_file_lines<int64_t>("day_1_input.txt");
    if (values.empty())
    {
        throw utils::puzzle_exception("Failed to read input file.");
    }

    utils::print_results(
        solve_part_1(values),
        solve_part_2(values)
    );*/

    utils::solve_puzzles(
        "day_1_input.txt",
        solve_part_1,
        solve_part_2
    );
}