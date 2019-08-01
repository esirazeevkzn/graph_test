#include <logger.hpp>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "graph.hpp"

std::vector<std::string> GetTokens(const std::string& data)
{
    std::vector<std::string> tokens;

    size_t start{0};
    size_t position{0};

    while ((position = data.find(';', start)) != std::string::npos)
    {
        tokens.push_back(data.substr(start, position - start));
        start = position + 1;
    }

    if (start < data.size())
    {
        tokens.push_back(data.substr(start));
    }

    return tokens;
}

int main(int argc, const char** argv)
{
    std::ifstream file("data.txt", std::ios_base::binary);
    if (!file.is_open())
    {
        LOG_ERROR("data.txt does not exist");
        return 1;
    }

    file.seekg(std::ios_base::beg, std::ios_base::end);
    const size_t size = file.tellg();
    file.seekg(std::ios_base::beg);

    auto buffer = std::make_unique<char[]>(size + 1);
    file.read(buffer.get(), static_cast<std::streamsize>(size));
    buffer[size] = '\0';
    file.close();

    auto tokens = GetTokens(buffer.get());


    Graph graph(true);

    for (size_t i = 0; i < tokens.size(); i+=3)
    {
        graph.Insert(tokens[i], tokens[i + 1], std::stoul(tokens[i + 2]));
    }

    LOG_INFO(Graph::Cycle(graph));

    return 0;
}