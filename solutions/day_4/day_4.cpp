#include "utility.hpp"

struct key_value_type
{
    std::string key;
    std::string value;
};

template<typename TBeginIt, typename TEndIt>
std::vector<key_value_type> get_passport_fields(TBeginIt& begin, TEndIt end)
{
    std::regex regex(R"(\s*(.[^:\s]*)[\s:]*(.[^\s]*))");
    std::vector<key_value_type> found_fields;

    while(begin != end && !begin->empty())
    {
        const auto& line = *(begin++);
        for(auto it = std::sregex_iterator(line.begin(), line.end(), regex); it != std::sregex_iterator(); ++it)
        {
            auto& matches = *it;
            found_fields.push_back({ matches[1].str(), matches[2].str() });
        }
    }

    if(begin != end)
    {
        ++begin;
    }

    return found_fields;
}

bool validate_passport_part_1(std::vector<key_value_type>& passport_fields)
{
    static const std::array<std::string, 7> required_fields = {
        "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"
    };

    size_t valid_fields = 0;
    for(auto& required_field : required_fields)
    {
        auto it = std::find_if(passport_fields.begin(), passport_fields.end(), [&](const auto& key_value)
        {
            return key_value.key == required_field;
        });

        if(it != passport_fields.end())
        {
            valid_fields++;
        }
    }

    return valid_fields == required_fields.size();
}

bool validate_passport_part_2(std::vector<key_value_type>& passport_fields)
{
    static const auto range_is_valid = [](const std::string& value, int min, int max)
    {
        int num = std::atoi(value.c_str());
        return num >= min && num <= max;
    };

    static const auto is_number = [](const std::string& value)
    {
        int number = 0;
        return std::from_chars(value.data(), value.data() + value.size(), number).ec == std::errc();
    };

    static const auto get_height = [](const std::string& value) -> std::pair<int, std::string>
    {
        int height = 0;
        auto result = std::from_chars(value.data(), value.data() + value.size(), height);
        if(result.ec != std::errc())
        {
            return {};
        }
        return { height, result.ptr };
    };

    static const auto hex_is_valid = [](const std::string& value)
    {
        if(value.size() != 7)
        {
            return false;
        }

        return std::find_if_not(value.begin(), value.end(), [](auto& c)
        {
            return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
        }) != value.end();
    };

    static const std::set<std::string> eye_colors = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };

    static const std::map<std::string, std::function<bool(const std::string&)>> validators =
    {
        { "byr", [](auto& value) { return range_is_valid(value, 1920, 2002); } },
        { "iyr", [](auto& value) { return range_is_valid(value, 2010, 2020); } },
        { "eyr", [](auto& value) { return range_is_valid(value, 2020, 2030); } },
        { "hgt", [](auto& value)
            {
                auto& [height, unit] = get_height(value);
                if (unit == "cm")
                {
                    return height >= 150 && height <= 193;
                }
                else if (unit == "in")
                {
                    return height >= 59 && height <= 76;
                }
                return false;
            }
        },
        { "hcl", [](auto& value) { return hex_is_valid(value); } },
        { "ecl", [](auto& value)
            {
                return eye_colors.find(value) != eye_colors.end();
            }
        },
        { "pid", [](auto& value) { return value.size() == 9 && is_number(value); } }
    };

    size_t valid_fields = 0;
    for(auto& passport_field : passport_fields)
    {
        auto it = validators.find(passport_field.key);
        if(it != validators.end() && it->second(passport_field.value))
        {
            valid_fields++;
        }
    }

    return valid_fields == validators.size();
}

template<typename TValidatorFunction>
size_t count_valid_passports(const std::vector<std::string>& lines, const TValidatorFunction& validator_function)
{
    size_t valid_passport_count = 0;

    auto it = lines.begin();
    while (it != lines.end())
    {
        auto passport_fields = get_passport_fields(it, lines.end());
        if (validator_function(passport_fields))
        {
            valid_passport_count++;
        }
    }

    return valid_passport_count;
}


size_t solve_part_1(const std::vector<std::string>& lines)
{
    return count_valid_passports(lines, validate_passport_part_1);
}

size_t solve_part_2(const std::vector<std::string>& lines)
{
    return count_valid_passports(lines, validate_passport_part_2);
}

int main()
{
    utils::solve_puzzles(
        "day_4_input.txt",
        solve_part_1,
        solve_part_2
    );
}