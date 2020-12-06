#include "utility.hpp"

size_t solve_part_1(const std::vector<std::string>& lines)
{
    std::vector<size_t> group_answer_count;
    std::set<char> yes_answers;

    for (auto& line : lines)
    {
        if (line.empty())
        {
            group_answer_count.push_back(yes_answers.size());
            yes_answers.clear();
            continue;
        }
        std::copy(line.begin(), line.end(), std::inserter(yes_answers, yes_answers.begin()));
    }

    return std::accumulate(group_answer_count.begin(), group_answer_count.end(), size_t{ 0 });
}

size_t solve_part_2(const std::vector<std::string>& lines)
{
    std::vector<size_t> group_answer_count;
    int32_t persons_in_group = 0;
    std::map<char, int32_t> yes_answers;

    auto get_group_answer = [&]() -> size_t
    {
        size_t count = 0;
        for(auto& yes_answer : yes_answers)
        {
            if(yes_answer.second == persons_in_group)
            {
                count++;
            }
        }
        return count;
    };

    for (auto& line : lines)
    {
        if (line.empty())
        {
            group_answer_count.push_back(get_group_answer());
            yes_answers.clear();
            persons_in_group = 0;
            continue;
        }

        persons_in_group++;
        for(auto& line_c : line)
        {
            auto it = yes_answers.find(line_c);
            if(it != yes_answers.end())
            {
                it->second++;
            }
            else
            {
                yes_answers.insert({ line_c, 1 });         
            }
        }
    }

    return std::accumulate(group_answer_count.begin(), group_answer_count.end(), size_t{ 0 });
}


int main()
{
    utils::solve_puzzles(
        "day_6_input.txt",
        solve_part_1,
        solve_part_2
    );
}