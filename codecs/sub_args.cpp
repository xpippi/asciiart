#include "sub_args.hpp"

Sub_args::Sub_args(const std::string & group_name):
    Options{""},
    group_name_{group_name}
{}

cxxopts::OptionAdder Sub_args::add_options()
{
    return cxxopts::Options::add_options(group_name_);
}

cxxopts::ParseResult Sub_args::parse(const std::vector<std::string> & args)
{
#if CXXOPTS__VERSION_MAJOR >= 3
    auto argv = std::vector<const char *>{};
    argv.emplace_back("");
    for(auto && i: args)
        argv.emplace_back(i.c_str());

    return cxxopts::Options::parse(std::size(argv), std::data(argv));
#else
    auto args_copy = args;
    auto argv = std::vector<char *>{};
    std::string arg0 = "";
    argv.emplace_back(arg0.data());
    for(auto && i: args_copy)
        argv.emplace_back(i.data());

    int argc = std::size(argv);
    auto argv_p = std::data(argv);
    return cxxopts::Options::parse(argc, argv_p);
#endif
}

std::string Sub_args::help(const std::string & main_help) const
{
    auto sub_help = cxxopts::Options::help();
    constexpr auto newline_count = 4u;
    auto count = 0u, pos = 0u;
    for(; pos < std::size(sub_help) && count < newline_count; ++pos)
    {
        if(sub_help[pos] == '\n')
            ++count;
    }
    return main_help + "\n" + sub_help.substr(pos);
}
