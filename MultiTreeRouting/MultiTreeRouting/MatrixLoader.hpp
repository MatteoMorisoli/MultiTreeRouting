//
//  MatrixLoader.hpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 21.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#ifndef MatrixLoader_hpp
#define MatrixLoader_hpp

#include <stdio.h>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/io.hpp>

//class to write and read already computed all-pairs distance matrices from and to file
class MatrixLoader{
    using EdgeWeight = boost::property<boost::edge_weight_t, int>;
    using VertexName = boost::property<boost::vertex_name_t, std::string> ;
    using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexName, EdgeWeight>;
    using DistanceMatrix = boost::exterior_vertex_property<Graph, int>::matrix_type;
    using SmartDistanceMatrix = boost::numeric::ublas::triangular_matrix<int, boost::numeric::ublas::upper>;
private:
    std::string name;
public:
    MatrixLoader(const std::string fileName);
    bool fileExists();
    void writeMatrix(const DistanceMatrix& d, const int size);
    void readMatrix(SmartDistanceMatrix& d, const int size);
    
};
#endif /* MatrixLoader_hpp */
