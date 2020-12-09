#include "utility.hpp"

template<typename TItBegin>
bool find_prev_pair_sum(TItBegin it_value, const size_t prev_count)
{
    auto it_start = std::prev(it_value, prev_count);
    for (auto it = it_start; it != it_value; it++)
    {
        for(auto it2 = it_start; it2 != it_value; it2++)
        {
            if(it == it2)
            {
                continue;
            }
            if(*it + *it2 == *it_value)
            {
                return true;
            }
        }
    }
    return false;
}

std::vector<int64_t>::const_iterator find_rule_breaker(const std::vector<int64_t>& values, const size_t prev_sum_count)
{
    for (auto it = std::next(values.begin(), prev_sum_count); it != values.end(); it++)
    {
        if (!find_prev_pair_sum(it, prev_sum_count))
        {
            return it;
        }
    }
    throw utils::puzzle_exception("Failed to find rule breaking number.");
}

std::pair<int64_t, int64_t> find_min_max(std::vector<int64_t>::const_iterator begin, std::vector<int64_t>::const_iterator end)
{
    std::pair<size_t, size_t> result = { 0, 0 };
    for(auto it = begin; it != end; it++)
    {
        int64_t sum = 0;
        for (auto it2 = it; it2 != end; it2++)
        {
            
            sum += *it2;
            if(sum == *end)
            {
                return {
                    *std::min_element(it, it2),
                    *std::max_element(it, it2)
                };
            }
            
            if(sum > *end)
            {
                break;
            }
        }
    }

    if(result.second == 0)
    {
        throw utils::puzzle_exception("Failed to find min max.");
    }
}

size_t solve_part_1(const std::vector<int64_t>& values)
{
    return *find_rule_breaker(values, 25);
}

size_t solve_part_2(const std::vector<int64_t>& values)
{
    auto invalid_value = find_rule_breaker(values, 25);
    auto min_max = find_min_max(values.begin(), invalid_value);
    return min_max.first + min_max.second;
}

int main()
{
    utils::solve_puzzles(
        "day_9_input.txt",
        solve_part_1,
        solve_part_2
    );
}