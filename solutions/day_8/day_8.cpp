#include "utility.hpp"

enum class instruction_token
{
    nop,
    jmp,
    acc   
};

struct instruction_type
{
    instruction_token token;
    int64_t value;
};

instruction_type get_instruction(const std::string& line)
{
    auto it = std::find(line.begin(), line.end(), ' ');
    if(it == line.end())
    {
        throw utils::puzzle_exception("Failed to find instruction and value separator.");
    }

    auto instruction = std::string_view{ line.data(),  static_cast<size_t>(&*it - line.data())};
    auto value = static_cast<int64_t>(std::atoi(&*std::next(it)));

    static std::map<std::string, instruction_token> instruction_map = {
        {"nop", instruction_token::nop },
        {"acc", instruction_token::acc },     
        {"jmp", instruction_token::jmp }
    };

    auto inst_it = instruction_map.find(std::string{ instruction });
    if(inst_it == instruction_map.end())
    {
        throw utils::puzzle_exception("Found unknown instruction name." );
    }

    return { inst_it->second, value };
}

std::vector<instruction_type> get_instructions(const std::vector<std::string>& lines)
{
    std::vector<instruction_type> instructions;
    instructions.reserve(lines.size());
    for(auto& line : lines)
    {
        instructions.push_back(get_instruction(line));
    }
    return instructions;
}

bool run_program(const std::vector<instruction_type>& instructions, int64_t& output)
{
    std::set<size_t> endless_loop_tracker;

    for (size_t i = 0; i < instructions.size();)
    {
        if (endless_loop_tracker.find(i) != endless_loop_tracker.end())
        {
            return false;    
        }
        endless_loop_tracker.insert(i);

        auto& instruction = instructions.at(i);
        switch(instruction.token)
        {
            case instruction_token::nop: ++i; break;
            case instruction_token::acc: ++i; output += instruction.value; break;
            case instruction_token::jmp: i += instruction.value; break;
        }
    }
    return true;
}

void swap_nop_to_jmp(instruction_type& instruction)
{
    instruction.token = static_cast<instruction_token>(static_cast<int>(instruction.token) ^ 1);
}

int64_t solve_part_1(const std::vector<std::string>& lines)
{
    auto instructions = get_instructions(lines);
    int64_t output = 0;
    run_program(instructions, output);
    return output;
}

size_t solve_part_2(const std::vector<std::string>& lines)
{
    auto instructions = get_instructions(lines);

    for(size_t i = 0; i < instructions.size(); i++)
    {
        if (instructions[i].token == instruction_token::acc)
        {
            continue;
        }

        swap_nop_to_jmp(instructions[i]);

        int64_t output = 0;
        if(run_program(instructions, output))
        {
            return output;
        }

        swap_nop_to_jmp(instructions[i]);
    }

    throw utils::puzzle_exception("Failed to fix program.");
}

int main()
{
    utils::solve_puzzles(
        "day_8_input.txt",
        solve_part_1,
        solve_part_2
    );
}