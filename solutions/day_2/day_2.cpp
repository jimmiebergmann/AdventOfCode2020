#include "result_printer.hpp"
#include "file_reader.hpp"

#include <regex>
#include <algorithm>

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
        exit_with_error("Failed to parse password data.");
    }

    if(matches.size() != 5)
    {
        exit_with_error("Ill-formed password data.");
    }

    return {
        std::atoi(matches[1].str().c_str()),
        std::atoi(matches[2].str().c_str()),
        matches[3].str()[0],
        matches[4].str()
    };
}

bool validate_silver_password(const password_data_type& password_data)
{
    int policy_token_count = std::count(
        password_data.password.begin(), 
        password_data.password.end(), 
        password_data.policy);

    return policy_token_count >= password_data.min && policy_token_count <= password_data.max;
}

int solve_silver(const std::vector<std::string>& lines)
{
    int valid_password_count = 0;
    for(const auto& line : lines)
    {
        auto password_data = parse_password_data(line);
        if(validate_silver_password(password_data))
        {
            valid_password_count++;
        }
    }

    return valid_password_count;
}

bool validate_gold_password(const password_data_type& password_data)
{
    int matches =
        (password_data.password[password_data.min - 1] == password_data.policy ? 1 : 0) +
        (password_data.password[password_data.max - 1] == password_data.policy ? 1 : 0);

    return matches == 1;
}

int solve_gold(const std::vector<std::string>& lines)
{
    int valid_password_count = 0;
    for (const auto& line : lines)
    {
        auto password_data = parse_password_data(line);
        if (validate_gold_password(password_data))
        {
            valid_password_count++;
        }
    }

    return valid_password_count;
}

int main()
{
    const auto lines = read_file_lines<std::string>("day_2_input.txt");
    if(lines.empty())
    {
        exit_with_error("Failed to read input file.");
    }

    auto silver_result = solve_silver(lines);
    auto gold_result = solve_gold(lines);

    print_result(silver_result, gold_result);
}