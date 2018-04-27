//
//  Writer.hpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 29.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#ifndef Writer_hpp
#define Writer_hpp

#include <stdio.h>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <fstream>

class Writer{
    using SmartStretchMatrix = boost::numeric::ublas::triangular_matrix<float, boost::numeric::ublas::upper>;

    
public:
    void writeStretchStars(SmartStretchMatrix &sm, std::string filePath, const int numTrees);
    void writeStretchStarStar(SmartStretchMatrix &sm, std::string filePath, const int numTrees);
};
#endif /* Writer_hpp */
