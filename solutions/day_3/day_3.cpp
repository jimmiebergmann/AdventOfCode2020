#include "utility.hpp"
#include <array>

size_t count_tree_hits(const std::vector<std::string>& lines, const int64_t slope_x, const int64_t slope_y)
{
    struct position
    {
        int64_t x;
        int64_t y;
    };

    size_t tree_hits = 0;
    position pos = { 0, 0 };

    for (pos.y = slope_y; pos.y < lines.size(); pos.y += slope_y)
    {
        auto& line = lines[pos.y];
        pos.x = (pos.x + slope_x) % line.size();

        if (line[pos.x] == '#')
        {
            tree_hits++;
        }
    }

    return tree_hits;
}

size_t solve_part_1(const std::vector<std::string>& lines)
{
    return count_tree_hits(lines, 3, 1);
}

size_t solve_part_2(const std::vector<std::string>& lines)
{
    std::array data = {
        count_tree_hits(lines, 1, 1),
        count_tree_hits(lines, 3, 1),
        count_tree_hits(lines, 5, 1),
        count_tree_hits(lines, 7, 1),
        count_tree_hits(lines, 1, 2)
    };

    return std::accumulate<>(
        data.begin(), 
        data.end(), 
        size_t{ 1 },
        std::multiplies<>());
}

int main()
{
    const auto lines = utils::read_file_lines<std::string>("day_3_input.txt");
    if (lines.empty())
    {
        throw utils::puzzle_exception("Failed to read input file.");
    }

    utils::print_results(
        solve_part_1(lines),
        solve_part_2(lines)
    );
}