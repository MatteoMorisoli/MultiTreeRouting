//
//  Printer.hpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 15.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#ifndef Printer_hpp
#define Printer_hpp

#include <stdio.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "TreeWorker.hpp"
#include <iostream>
#include <fstream>

//using EdgeWeight = boost::property<boost::edge_weight_t, int>;
//using VertexName = boost::property<boost::vertex_name_t, std::string> ;
//using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexName, EdgeWeight>;
//using DistanceMatrix = boost::exterior_vertex_property<Graph, int>::matrix_type;



class Printer{
    using TreeDistanceMatrix = boost::numeric::ublas::matrix<int>;
    using StretchMatrix = boost::numeric::ublas::matrix<double>;
    using EdgeWeight = boost::property<boost::edge_weight_t, int>;
    using VertexName = boost::property<boost::vertex_name_t, std::string> ;
    using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexName, EdgeWeight>;
    using DistanceMatrix = boost::exterior_vertex_property<Graph, int>::matrix_type;
    
public:
    void printUblasTreeMatrix(const TreeDistanceMatrix& m, const std::vector<std::string>& names, const int root) const;
    void printUblasStretchMatrix(const StretchMatrix& m, const std::vector<std::string>& names, const int root) const;
    void printUblasStretchStarMatrix(const StretchMatrix& m, const std::vector<std::string>& names) const;
    void printTreeCongestion(const Congestion &c, const std::vector<std::string> &names, std::string filePath, int sourceNode) const;
    void printDistanceMatrix(const DistanceMatrix& m, const std::vector<std::string>& names) const;
    };
    
#endif /* Printer_hpp */
