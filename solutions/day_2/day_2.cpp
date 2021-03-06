#include "utility.hpp"

struct password_data_type
{
    int min;
    int max;
    char policy;
    std::string password;
};

password_data_type parse_password_data(const std::string& data)
{
    std::regex regex(R"(^(.[^-]*)-(.[^\s]*)\s(.):\s(.*)$)");

    std::smatch matches;
    if (!std::regex_search(data, matches, regex))
    {
        throw utils::puzzle_exception("Failed to parse password data.");
    }

    if(matches.size() != 5)
    {
        throw utils::puzzle_exception("Ill-formed password data.");
    }

    return {
        std::atoi(matches[1].str().c_str()),
        std::atoi(matches[2].str().c_str()),
        matches[3].str()[0],
        matches[4].str()
    };
}

bool validate_part_1_password(const password_data_type& password_data)
{
    int policy_token_count = std::count(
        password_data.password.begin(), 
        password_data.password.end(), 
        password_data.policy);

    return policy_token_count >= password_data.min && policy_token_count <= password_data.max;
}

bool validate_part_2_password(const password_data_type& password_data)
{
    int matches =
        (password_data.password[password_data.min - 1] == password_data.policy ? 1 : 0) +
        (password_data.password[password_data.max - 1] == password_data.policy ? 1 : 0);

    return matches == 1;
}

int count_valid_passwords(const std::vector<std::string>& lines, std::function<bool(const password_data_type&)> password_validator)
{
    int valid_password_count = 0;
    for (const auto& line : lines)
    {
        auto password_data = parse_password_data(line);
        if (password_validator(password_data))
        {
            valid_password_count++;
        }
    }

    return valid_password_count;
}

int solve_part_1(const std::vector<std::string>& data)
{
    return count_valid_passwords(data, validate_part_1_password);
}

int solve_part_2(const std::vector<std::string>& data)
{
    return count_valid_passwords(data, validate_part_2_password);
}

int main()
{
    utils::solve_puzzles(
        "day_2_input.txt",
        solve_part_1,
        solve_part_2
    );
}