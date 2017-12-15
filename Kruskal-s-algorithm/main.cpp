//
//  main.cpp
//  kruskal's algorithm
//
//

/*
 Problem:
 
 Given an undirected weighted connected graph, it is required
 to find the Really Special SubTree in it. The Really Special
 SubTree is defined as a subgraph consisting of all the nodes
 in the graph and:
 
 -There is only one exclusive path from a node to every other
 node.
 -The subgraph is of minimum overall weight (sum of all edges)
 among all such subgraphs.
 -While creating the Really Special SubTree, start by picking
 the edge with smallest weight. If there are edges of equal
 weight available at an instant, then the edge to be chosen
 first among them is the one with minimum value of sum of
 the following expression :
    u + wt + v , where u and v are the node numbers of the
 corresponding edge and wt is the weight.
 -Even then if there is a collision, choose any one of them.
 -While doing the above, ensure that no cycle is formed while
 picking up edges.
 Finally, you need to print the overall weight of the Tree so
 formed using above rules.
 
*/

#include <vector>
#include <iostream>
#include <algorithm>
#include <tuple>

class Graph {
private:
    std::vector<std::tuple<int, int, int>> adj;
    int *link;
    int *size;
    int nEdges;
    int nNodes;
    
public:
    Graph(int nEdges, int nNodes) : nEdges(nEdges), nNodes(nNodes) {
        link = new int[nNodes];
        size = new int[nNodes];
        for (int i = 1; i <= nNodes; i++) {
            link[i] = i;
            size[i] = 1;
        }
        
    }
    void Add_edge(int u, int v, int w) {
        adj.push_back(std::make_tuple(u, v, w));
    }
    void Sort_edges() {
        std::sort(adj.begin(), adj.end(), [] (const std::tuple<int, int, int> &a, const std::tuple<int, int, int> &b) {
            return std::get<2>(a) < std::get<2>(b);
        });
    }
    int Kruskal() {
        int sum = 0;
        this->Sort_edges();
        for (int i = 0; i < nEdges; i++) {
            if (!same(std::get<0>(adj[i]), std::get<1>(adj[i]))) {
                unite(std::get<0>(adj[i]), std::get<1>(adj[i]));
                sum += std::get<2>(adj[i]);
            }
        }
        return sum;
    }
    int find(int x) {
        while (x != link[x]) {
            x = link[x];
        }
        return x;
    }
    bool same(int a, int b) {
        return find(a) == find(b);
    }
    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (size[a] < size[b]) {
            std::swap(a, b);
        }
        size[a] += size[b];
        link[b] = a;
    }
    
    ~Graph() {
        delete[] link;
        delete[] size;
    }
};

int main(void) {
    int n = 0, m = 0;
    std::cin >> n >> m;
    Graph *graph = new Graph(m, n);
    
    for (int i = 0; i < m; i++) {
        int v = 0, u = 0, w = 0;
        std::cin >> v >> u >> w;
        graph->Add_edge(v, u, w);
    }
    
    std::cout << graph->Kruskal();
    
    return EXIT_SUCCESS;
}
