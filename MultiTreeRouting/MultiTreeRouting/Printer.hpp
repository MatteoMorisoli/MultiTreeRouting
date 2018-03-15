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


using TreeDistanceMatrix = boost::numeric::ublas::matrix<int>;
using StretchMatrix = boost::numeric::ublas::matrix<double>;


class Printer{
public:
    void printUblasTreeMatrix(TreeDistanceMatrix m, std::vector<std::string> names, int root) const;
    void printUblasStretchMatrix(StretchMatrix m, std::vector<std::string> names, int root) const;
    void printUblasStretchStarMatrix(StretchMatrix m, std::vector<std::string> names) const;
    };
    
#endif /* Printer_hpp */
