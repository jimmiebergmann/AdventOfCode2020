#include "utility.hpp"

struct tree_type
{

    struct node_type;
    struct child_node_type;
    using node_type_ptr = std::shared_ptr<node_type>;
    using node_children_type = std::vector<child_node_type>;

    struct node_type
    {
        node_type(std::string_view name) :
            name(name)
        {}

        std::string name;
        node_children_type children;
    };

    struct child_node_type
    {
        node_type_ptr child;
        size_t count;
    };

    void parse(const std::vector<std::string>& lines)
    {
        for(auto& line : lines)
        {
            parse_bag(line);
        }
    }

    void parse_bag(const std::string& line)
    {
        size_t bag_tree_start = 0;
        auto [bag_name, bag_content_start] = parse_bag_name(line, bag_tree_start);
        auto new_bag = find_or_create_bag(bag_name);
        auto bag_content = parse_bag_content({ line.data() + bag_content_start, line.size() - bag_content_start - 1 });
        new_bag->children = std::move(bag_content);
    }

    std::pair<std::string_view, size_t> parse_bag_name(const std::string& line, size_t& bag_tree_start)
    {
        static const std::string bag_name_identifier = "bags contain ";

        auto pos = line.find(bag_name_identifier);
        if(pos == std::string::npos)
        {
            throw utils::puzzle_exception("Failed to parse bag name.");
        }
        return { { line.data(), pos - 1 }, pos + bag_name_identifier.size() };
    }

    node_children_type parse_bag_content(std::string_view data)
    {
        node_children_type children;

        auto it = data.begin();
        while(it != data.end())
        {
            auto start_it = it;
            it = std::find(start_it, data.end(), ',');
            auto part = std::string_view{ &*start_it, static_cast<size_t>(it - start_it) };

            if(part == "no other bags")
            {
                break;
            }
            if(it < data.end() - 2)
            {
                it += 2;
            }

            auto [bag_count, bag_name] = parse_bag_content_part(part);
            children.push_back({ find_or_create_bag(bag_name), bag_count });
        }

        return children;
    }

    std::pair<size_t, std::string_view> parse_bag_content_part(std::string_view data)
    {
        auto count_pos = data.find( ' ');
        if(count_pos == std::string::npos)
        {
            throw utils::puzzle_exception("Failed to find bag count.");
        }

        size_t count = 0;
        auto result = std::from_chars(data.data(), data.data() + count_pos, count);

        auto name_end_pos = data.find(" bag");
        if (name_end_pos == std::string::npos)
        {
            throw utils::puzzle_exception("Failed to find bag content end.");
        }

        auto name = std::string_view{ data.data() + count_pos + 1, name_end_pos - count_pos - 1 };
        return { count, name };
    }

    node_type_ptr find_or_create_bag(std::string_view name)
    {
        std::string str_name = std::string{ name };
        auto it = bags.find(str_name);
        if(it != bags.end())
        {
            return it->second;
        }

        auto new_bag = std::make_shared<node_type>(name);
        bags.insert({ std::move(str_name), new_bag });
        return new_bag;
    }

    std::map<std::string, node_type_ptr> bags;

};

bool contains_bag(tree_type::node_type_ptr& node, const std::string& bag_name)
{
    // Iterative implementation.
    using stack_item = std::pair<tree_type::node_children_type::iterator, tree_type::node_children_type::iterator> ;

    std::stack<stack_item> stack;
    stack.push({ node->children.begin(), node->children.end() });
    
    while(!stack.empty())
    {
        auto& item = stack.top();
        if(item.first == item.second)
        {
            stack.pop();
            continue;
        }

        auto& child = item.first->child;
        if (child->name == bag_name)
        {
            return true;
        }

        if(child->children.size())
        {
            stack.push({ child->children.begin(), child->children.end() });
        }

        ++item.first;
    }

    return false;
}

size_t count_children_bags(tree_type::node_type_ptr& node)
{
    // Recursive implementation.
    size_t sum = 0;

    for(auto& child: node->children)
    {
        sum += child.count * (count_children_bags(child.child) + 1);
    }

    return sum;
}

size_t solve_part_1(const std::vector<std::string>& lines)
{
    tree_type tree;
    tree.parse(lines);

    size_t count = 0;
    for(auto& bag_node : tree.bags)
    {
        if(contains_bag(bag_node.second, "shiny gold"))
        {
            count++;
        }
    }

    return count;
}

size_t solve_part_2(const std::vector<std::string>& lines)
{
    tree_type tree;
    tree.parse(lines);

    auto shiny_golden_bag = tree.bags.find("shiny gold")->second;
    return count_children_bags(shiny_golden_bag);
}

int main()
{
    utils::solve_puzzles(
        "day_7_input.txt",
        solve_part_1,
        solve_part_2
    );
}