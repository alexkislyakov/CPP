//
//  main.cpp
//  Dijkstra
//
//

/*
 Problem:
 
 Given a graph consisting N nodes (labelled 1 to N) where a
 specific given node S represents the starting position S and
 an edge between two nodes is of a given length, which may
 or may not be equal to other lengths in the graph.
 
 It is required to calculate the shortest distance from
 the start position (Node S) to all of the other nodes in
 the graph.
 
 Note: If a node is unreachable , the distance is assumed as -1 .
 
*/

#include <queue>
#include <vector>
#include <iostream>
#include <utility>

class Graph {
 private:
    std::vector<std::pair<int, int>> *adj;
    int n;
    
    
 public:
    explicit Graph(int nodes) : n(nodes) {
        adj = new std::vector<std::pair<int, int>>[n];
    }
    
    void Add_edge(int u, int v, int w) {
        adj[u].push_back(std::make_pair(v, w));
        adj[v].push_back(std::make_pair(u, w));
    }
    
    std::vector<int> Dijkstra(int start) {
        std::priority_queue<std::pair<int, int>> q;
        std::vector<bool> processed(n, false);
        std::vector<int> distance(n, static_cast<int>(10e6));
        distance[start] = 0;
        q.push(std::make_pair(0, start));
        
        while (!q.empty()) {
            int a = q.top().second; q.pop();
            if (processed[a]) {
                continue;
            }
            processed[a] = true;
            for (auto u : adj[a]) {
                int b = u.first, w = u.second;
                if (distance[a] + w < distance[b]) {
                    distance[b] = distance[a] + w;
                    q.push(std::make_pair(-distance[b], b));
                }
            }
        }
        return distance;
    }
};

int main(void) {
    int t;
    std::cin >> t;
    for (int a0 = 0; a0 < t; a0++) {
        int n;
        int m;
        std::cin >> n >> m;
        Graph *graph = new Graph(n);
        for (int a1 = 0; a1 < m; a1++) {
            int x;
            int y;
            int r;
            std::cin >> x >> y >> r;
            x--;
            y--;
            graph->Add_edge(x, y, r);
        }
        int s;
        std::cin >> s;
        s--;
        std::vector<int> d = graph->Dijkstra(s);
        for (int i = 0; i < n; i++) {
            if (i != s) {
                int p = d[i];
                if (p == static_cast<int>(10e6)) {
                    std::cout << -1 << " ";
                } else {
                    std::cout << p << " ";
                }
            }
        }
        std::cout << std::endl;
    }
    return EXIT_SUCCESS;
}
