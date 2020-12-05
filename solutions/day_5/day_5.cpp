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
    auto low = start;
    auto high = end;

    token_type token{};
    for(size_t i = 0; i < tokens.size(); i++)
    {
        token = tokens[i];
        if(token == token_type::low)
        {
            high -= ((high - low) / 2) + 1;
        }
        else
        {
            low += ((high - low) / 2) + 1;
        }
    }

    return token == token_type::low ? low : high;
}

std::set<int64_t> get_seat_ids(const std::vector<std::string>& lines)
{
    std::set<int64_t> seats;
    for (const auto& line : lines)
    {
        auto boarding_card = read_boarding_card(line);
        auto row = binary_search(boarding_card.row_tokens, 0, 127);
        auto column = binary_search(boarding_card.column_tokens, 0, 7);
        int64_t seat_id = (row * 8) + column;
        seats.insert(seat_id);
    }

    return seats;
}

size_t solve_part_1(const std::vector<std::string>& lines)
{
    return *get_seat_ids(lines).rbegin();
}

size_t solve_part_2(const std::vector<std::string>& lines)
{
    std::set<int64_t> seat_ids = get_seat_ids(lines);

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