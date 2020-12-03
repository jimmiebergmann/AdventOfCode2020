#ifndef AOC_UTILITY_FILE_READER_HPP
#define AOC_UTILITY_FILE_READER_HPP

#include <vector>
#include <fstream>
#include <string>
#include <charconv>

namespace utils
{

    template<typename TDataType>
    std::vector<TDataType> read_file_lines(const std::string filename, const bool skip_empty_lines = true)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            return {};
        }

        std::vector<TDataType> output;

        std::string line;
        while (!file.eof())
        {
            std::getline(file, line);
            if (line.empty() && skip_empty_lines)
            {
                continue;
            }

            TDataType value{};
            auto result = std::from_chars(line.data(), line.data() + line.size(), value);
            if (result.ec != std::errc())
            {
                return {};
            }

            output.push_back(value);
        }

        return output;
    }

    template<>
    std::vector<std::string> read_file_lines<std::string>(const std::string filename, const bool skip_empty_lines)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            return {};
        }

        std::vector<std::string> output;

        std::string line;
        while (!file.eof())
        {
            std::getline(file, line);
            if (line.empty() && skip_empty_lines)
            {
                continue;
            }

            output.push_back(line);
        }

        return output;
    }

}

#endif