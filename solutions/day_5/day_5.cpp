#include "utility.hpp"

enum class token_type
{
    low,
    high
};

struct boarding_card
{
    std::array<token_type, 7> row_tokens;
    std::array<token_type, 3> column_tokens;
};

template<size_t VStart, size_t VCount>
std::array<token_type, VCount> read_tokens(const std::string& input)
{
    std::array<token_type, VCount> tokens {};
    for(size_t i = 0; i < VCount; i++)
    {
        switch(input[i + VStart])
        {
            case 'F':
            case 'L': tokens[i] = token_type::low; break;
            case 'B':
            case 'R': tokens[i] = token_type::high; break;
        }
    }
    return tokens;
}

boarding_card read_boarding_card(const std::string& input)
{
    if (input.size() != 10)
    {
        throw utils::puzzle_exception("Invalid length of input row.");
    }

    return boarding_card{
        read_tokens<0, 7>(input),
        read_tokens<7, 3>(input)
    };
}

template<size_t VTokenCount>
int64_t binary_search(const std::array<token_type, VTokenCount>& tokens, int64_t start, int64_t end)
{
    struct range_type
    {
        int64_t low;
        int64_t high;
    };

    range_type range{ start, end };
    token_type token{};
    for(size_t i = 0; i < tokens.size(); i++)
    {
        token = tokens[i];
        if(token == token_type::low)
        {
            range.high -= ((range.high - range.low) / 2) + 1;
        }
        else
        {
            range.low += ((range.high - range.low) / 2) + 1;
        }
    }

    return token == token_type::low ? range.low : range.high;
}

int64_t get_seat_id(const std::string& line)
{
    auto boarding_card = read_boarding_card(line);
    auto row = binary_search(boarding_card.row_tokens, 0, 127);
    auto column = binary_search(boarding_card.column_tokens, 0, 7);
    return (row * 8) + column;
}

size_t solve_part_1(const std::vector<std::string>& lines)
{
    size_t highest_seat_id = 0;
    for (const auto& line : lines)
    {
        int64_t seat_id = get_seat_id(line);
        if(seat_id > highest_seat_id)
        {
            highest_seat_id = seat_id;
        }
    }
    
    return highest_seat_id;
}

size_t solve_part_2(const std::vector<std::string>& lines)
{
    std::set<int64_t> seat_ids;
    for (const auto& line : lines)
    {
        int64_t seat_id = get_seat_id(line);

        if(seat_ids.find(seat_id) != seat_ids.end())
        {
            throw utils::puzzle_exception("Duplicate of seat id.");
        }
        seat_ids.insert(seat_id); 
    }

    auto it = seat_ids.begin();
    int64_t prev_seat_id = *(it++);
    for(it; it != seat_ids.end(); it++)
    {
        auto seat_id = *it;
        if(seat_id != prev_seat_id + 1)
        {
            return seat_id - 1;
        }
        prev_seat_id = seat_id;
    }

    return 0;
}

int main()
{
    utils::solve_puzzles(
        "day_5_input.txt",
        solve_part_1,
        solve_part_2
    );
}