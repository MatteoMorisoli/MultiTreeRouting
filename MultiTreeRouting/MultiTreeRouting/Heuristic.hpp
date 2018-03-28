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
#include <set>
#include <boost/graph/adjacency_list.hpp>

using EdgeWeight = boost::property<boost::edge_weight_t, int>;
using VertexName = boost::property<boost::vertex_name_t, std::string> ;
using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexName, EdgeWeight>;

//abstract interface for heuristic implementations
class Heuristic{
public:
    virtual std::set<int> selectStartingNodes(const int numVertices, const Graph& graph, const int numRoots) const = 0;
};

#endif /* Heuristic_hpp */
