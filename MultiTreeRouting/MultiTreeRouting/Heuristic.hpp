//
//  Heuristic.hpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 13.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#ifndef Heuristic_hpp
#define Heuristic_hpp

#include <stdio.h>
#include <vector>
#include <boost/graph/adjacency_list.hpp>

using EdgeWeight = boost::property<boost::edge_weight_t, int>;
using VertexName = boost::property<boost::vertex_name_t, std::string> ;
using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexName, EdgeWeight>;

//abstract interface for heuristic implementations
class Heuristic{
protected:
    std::vector<int> usedRoots;
    int numVertices;
public:
    Heuristic(int numV);
    virtual int selectStartingNode(const Graph& graph) const = 0;
};

#endif /* Heuristic_hpp */
