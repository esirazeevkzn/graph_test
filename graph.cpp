#include <logger.hpp>
#include <map>
#include <unordered_set>

#include "graph.hpp"

Graph::Graph(bool directed) : directed_(directed) {}

void Graph::Insert(const std::string& vertex_name_from, const std::string& vertex_name_to, size_t distance)
{
    graph_[vertex_name_from][vertex_name_to] = distance;
    graph_[vertex_name_to];

    if (!directed_)
    {
        graph_[vertex_name_to][vertex_name_from] = distance;
    }
}

void Graph::Remove(const std::string& vertex_name)
{
    for (auto& edges : graph_)
    {
        edges.second.erase(vertex_name);
    }

    graph_.erase(vertex_name);
}

void Graph::Print() const
{
    for (const auto& vertex_from : graph_)
    {
        const auto& vertex_name_from = vertex_from.first;

        for (const auto& vertex_to : vertex_from.second)
        {
            PrintEdge(vertex_name_from, vertex_to.first, vertex_to.second);
        }
    }
}

bool Graph::Empty() const { return graph_.empty(); }

size_t Graph::Size() const { return graph_.size(); }

void Graph::PrintEdge(const std::string& vertex_name_from, const std::string& vertex_name_to, size_t distance)
{
    LOG_INFO(vertex_name_from, "->", vertex_name_to, ":", distance);
}

size_t Graph::MST(const Graph& graph)
{
    size_t mst = 0;
    size_t edge_amount = 0;

    std::multimap<size_t, std::pair<std::string, std::string>> edges;

    for (const auto& vertex_from : graph.graph_)
    {
        for (const auto& vertex_to : vertex_from.second)
        {
            edges.emplace(std::piecewise_construct, std::forward_as_tuple(vertex_to.second),
                          std::forward_as_tuple(std::piecewise_construct,
                                                std::forward_as_tuple(vertex_from.first),
                                                std::forward_as_tuple(vertex_to.first)));
        }
    }

    std::unordered_map<std::string, std::string> parent_set;

    for (const auto& edge : edges)
    {
        if (edge_amount == graph.Size())
        {
            break;
        }

        auto parent_1 = Find(parent_set, edge.second.first);
        auto parent_2 = Find(parent_set, edge.second.second);

        if (parent_1 == parent_2)
        {
            continue;
        }

        parent_set[parent_1] = parent_2;

        PrintEdge(edge.second.first, edge.second.second, edge.first);

        mst += edge.first;
        ++edge_amount;
    }

    return mst;
}

bool Graph::Cycle(const Graph& graph)
{
    if (graph.directed_)
    {
        std::unordered_set<std::string> visited_vertex;
        std::unordered_map<std::string, bool> inStack;

        for (const auto& vertex_from : graph.graph_)
        {
            if (CycleInDirectedGraph(graph, vertex_from.first, visited_vertex, inStack))
            {
                return true;
            }
        }

        return false;
    }

    return CycleInUnderectedGraph(graph);
}

bool Graph::CycleInDirectedGraph(const Graph& graph, const std::string& vertex,
                                 std::unordered_set<std::string>& visited_vertex,
                                 std::unordered_map<std::string, bool>& inStack)
{
    if (!visited_vertex.count(vertex))
    {
        visited_vertex.insert(vertex);
        inStack[vertex] = true;

        const auto vertex_edges = graph.graph_.find(vertex);
        for (const auto& vertex_from : vertex_edges->second)
        {
            if (!visited_vertex.count(vertex_from.first) &&
                CycleInDirectedGraph(graph, vertex_from.first, visited_vertex, inStack))
            {
                return true;
            }

            else if (inStack[vertex_from.first])
            {
                return true;
            }
        }
    }

    inStack[vertex] = false;
    return false;
}

bool Graph::CycleInUnderectedGraph(const Graph& graph)
{
    std::unordered_map<std::string, std::string> parent_set;

    for (const auto& vertex_from : graph.graph_)
    {
        for (const auto& vertex_to : vertex_from.second)
        {
            const auto parent_1 = Find(parent_set, vertex_from.first);
            const auto parent_2 = Find(parent_set, vertex_to.first);

            if (parent_1 == parent_2)
            {
                return true;
            }

            parent_set[parent_1] = parent_2;
        }
    }

    return false;
}

std::string Graph::Find(std::unordered_map<std::string, std::string>& parent_set,
                        const std::string& vertex_name)
{
    if (parent_set[vertex_name].empty())
    {
        return vertex_name;
    }

    return Find(parent_set, parent_set[vertex_name]);
}
