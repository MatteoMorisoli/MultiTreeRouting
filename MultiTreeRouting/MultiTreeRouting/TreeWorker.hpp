//
//  TreeWorker.hpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 15.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#ifndef TreeWorker_hpp
#define TreeWorker_hpp

#include <stdio.h>
#include <vector>
#include <set>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>

//struct to store congestion informations
struct Congestion {
    std::vector<std::pair<int, int>> edges;
    std::vector<int> congestionValues;
};

//class that works on a tree given by djikstra shortest path to compute additional informations
class TreeWorker{
    using EdgeWeight = boost::property<boost::edge_weight_t, int>;
    using VertexName = boost::property<boost::vertex_name_t, std::string> ;
    using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexName, EdgeWeight>;
public:
    TreeWorker(const std::vector<int>& dV, const std::vector<int>& pV, const int rootNode, const Graph& graph);
    int lca(int node1, int node2);
    Congestion getCongestion();
    
private:
    void addSubTrees(const int root, std::vector<std::set<int>>& trees);
    const Graph& graph;
    const int rootNode;
    const std::vector<int>& distanceVector;
    const std::vector<int>& predecessorVector;
};

#endif /* TreeWorker_hpp */
