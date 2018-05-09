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
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/io.hpp>

using EdgeWeight = boost::property<boost::edge_weight_t, int>;
using VertexName = boost::property<boost::vertex_name_t, std::string> ;
using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexName, EdgeWeight>;
using DiagMatrixInt = boost::numeric::ublas::triangular_matrix<int, boost::numeric::ublas::upper>;

//abstract interface for heuristic implementations
class Heuristic{
protected:
    std::vector<int> usedRoots;
    int numVertices;
public:
    Heuristic(int numV);
    virtual int selectStartingNode(const Graph& graph) = 0;
    virtual int selectStartingNode(const DiagMatrixInt & dm) = 0;
};

#endif /* Heuristic_hpp */
