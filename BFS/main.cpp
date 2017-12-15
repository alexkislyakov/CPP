//
//  main.cpp
//  BFS
//
//

/*
 Problem:
 
 Consider an undirected graph consisting of n nodes
 where each node is labeled from 1 to n and the edge
 between any two nodes is always of length 6. We
 define node s to be the starting position for a BFS.
 
Given q queries in the form of a graph and some
 starting node,s, perform each query by calculating
 the shortest distance from starting node s to all
 the other nodes in the graph. Then print a single
 line of n - 1 space-separated integers listing node
 s's shortest distance to each of the n - 1 other
 nodes (ordered sequentially by node number); if
 s is disconnected from a node, print -1 as the
 distance to that node.
*/

#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>

class Graph {
private:
    std::vector<std::vector<int>> adj;
    size_t size;
    
public:
    explicit Graph(size_t size) : size(size) {
        adj.resize(size);
    }
    void Add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    std::vector<int> BFS(int start) {
        std::vector<int> distances(size, -1);
        std::vector<bool> visited(size, false);
        std::queue<int> q;
        
        visited[start] = true;
        q.push(start);
        distances[start] = 0;
        while (!q.empty()) {
            int s = q.front(); q.pop();
            
            for (auto u : adj[s]) {
                if (!visited[u]) {
                    visited[u] = true;
                    distances[u] = distances[s] + 6;
                    q.push(u);
                }
            }
        }
        return distances;
    }
};


int main(void) {
    int q = 0;
    std::cin >> q;
    for (int i = 0; i < q; i++) {
        int m = 0, n = 0, start = 0;
        std::cin >> n >> m;
        Graph *graph = new Graph(n);
        for (int j = 0; j < m; j++) {
            int u = 0, v = 0;
            std::cin >> u >> v;
            u--; v--;
            graph->Add_edge(u, v);
        }
        std::cin >> start;
        start--;
        std::vector<int> d = graph->BFS(start);
        for (int k = 0; k < n; k++) {
            if (k != start) {
                std::cout << d[k] << " ";
            }
        }
        std::cout << std::endl;
    }
    return EXIT_SUCCESS;
}
