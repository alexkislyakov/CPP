//
//  main.cpp
//  Prim's algorithm
//
//  Created by Александр Кисляков on 22.07.17.
//  Copyright © 2017 Александр Кисляков. All rights reserved.
//

/*
 Problem:
 Given a graph which consists of several edges connecting the
 N nodes in it.
 It is required to find a subgraph of the given graph with the
 following properties:
 
 The subgraph contains all the nodes present in the original graph.
 The subgraph is of minimum overall weight (sum of all edges) among
 all such subgraphs.
 It is also required that there is exactly one, exclusive path
 between any two nodes of the subgraph.
 One specific node S is fixed as the starting point of finding
 the subgraph.
 Find the total weight of such a subgraph (sum of all edges in
 the subgraph)
*/

#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <set>
#include <tuple>

class Graph {
 private:
    std::vector<std::pair<int, int>> *adj;
    int nodes;
    
    
 public:
    explicit Graph(int n) : nodes(n) {
        adj = new std::vector<std::pair<int, int>>[nodes];
    }
    void Add_edge(int u, int v, int w) {
        adj[u].push_back(std::make_pair(v, w));
        adj[v].push_back(std::make_pair(u, w));
    }
    int Prim(int start) {
        int sum = 0;
        std::priority_queue<std::tuple<int, int, int>> q;
        std::set<int> s;
        
        q.push(std::make_tuple(0, start, start));
        
        while (s.size() != nodes) {
            std::tuple<int, int, int> t = q.top(); q.pop();
            if (s.count(std::get<1>(t)) && s.count(std::get<2>(t))) continue;
            sum += -std::get<0>(t);
            s.insert(std::get<1>(t));
            s.insert(std::get<2>(t));
            for (auto u : adj[std::get<1>(t)]) {
                q.push(std::make_tuple(-u.second, u.first, std::get<1>(t)));
            }
        }
        return sum;
    }
};

int main() {
    int n = 0, m = 0;
    std::cin >> n >> m;
    Graph *graph = new Graph(n);
    
    for (int i = 0; i < m; i++) {
        int v = 0, u = 0, w = 0;
        std::cin >> v >> u >> w;
        v--; u--;
        graph->Add_edge(v, u, w);
    }
    int start = 0;
    std::cin >> start;
    start--;
    std::cout << graph->Prim(start);
    
    return EXIT_SUCCESS;
}
