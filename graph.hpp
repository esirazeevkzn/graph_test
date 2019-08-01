#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>

class Graph final
{
  public:
    Graph(bool directed = false);
    Graph(const Graph&) = delete;
    Graph(Graph&&) = delete;
    Graph& operator=(const Graph&) = delete;
    Graph& operator=(Graph&&) = delete;
    ~Graph() = default;

    void Insert(const std::string& vertex_name_from, const std::string& vertex_name_to, size_t distance);
    void Remove(const std::string& vertex_name);
    void Print() const;
    bool Empty() const;
    size_t Size() const;

    static size_t MST(const Graph& graph);
    static bool Cycle(const Graph& graph);

  private:
    static bool CycleInDirectedGraph(const Graph& graph, const std::string& vertex,
                                     std::unordered_set<std::string>& visited_vertex,
                                     std::unordered_map<std::string, bool>& inStack);
    static bool CycleInUnderectedGraph(const Graph& graph);
    static void PrintEdge(const std::string& vertex_name_from, const std::string& vertex_name_to,
                          size_t distance);
    static std::string Find(std::unordered_map<std::string, std::string>& paren_set,
                            const std::string& vertex_name);

    bool directed_;
    std::unordered_map<std::string, std::unordered_map<std::string, size_t>> graph_;
};

#endif // GRAPH_HPP
