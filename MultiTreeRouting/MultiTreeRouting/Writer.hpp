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
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <fstream>

class Writer{
    using StretchMatrix = boost::numeric::ublas::matrix<double>;

    
public:
    void writeStretchStars(StretchMatrix &sm, std::string filePath, const int numTrees);
    void writeStretchStarStar(StretchMatrix &sm, std::string filePath, const int numTrees);
};
#endif /* Writer_hpp */
