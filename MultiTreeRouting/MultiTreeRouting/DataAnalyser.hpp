//
//  DataAnalyser.hpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 26.04.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#ifndef DataAnalyser_hpp
#define DataAnalyser_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/format.hpp>

class DataAnalyser{
    using diagMatrixFloat = boost::numeric::ublas::triangular_matrix<float, boost::numeric::ublas::upper>;
public:
    DataAnalyser(std::string f, std::string suf, bool isStar, int treeNum);
    void addData(diagMatrixFloat& sm);
    void addAndPrintCongestionData(diagMatrixFloat& sm,  std::map<int, std::string> &nm);
    void generateStretchFiles();
    void generateCongestionFiles();
private:
    std::string filePath;
    std::string suffix;
    std::vector<std::string> hex;
    int treeNum;
    long sizeCounter;
    long double sum;
    std::map<double, long> valueMap;
    double valueOfIndex(const std::map<double, long> & map, long index) const;
};
#endif /* DataAnalyser_hpp */
