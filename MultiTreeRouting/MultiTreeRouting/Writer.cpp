//
//  Writer.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 29.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "Writer.hpp"


void Writer::writeStretchStars(std::vector<StretchMatrix> &sm, std::string filePath, const int numTrees){
    filePath.replace(filePath.size()-4, 8, "all" + std::to_string(numTrees) + ".csv");
    std::ofstream output(filePath);
    for(std::vector<StretchMatrix>::iterator it = sm.begin(); it != sm.end(); ++it){
        for(int i = 0; i < it->size1(); ++i){
            for(int j = i+1; j < it->size2(); j++){
                output << (*it)(i, j) << ",";
            }
        }
    }
    output.close();
}

void Writer::writeStretchStarStar(StretchMatrix &sm, std::string filePath, const int numTrees){
    filePath.replace(filePath.size()-4, 9, "star" + std::to_string(numTrees) + ".csv");
    std::ofstream output(filePath);
    for(int i = 0; i < sm.size1(); ++i){
        for(int j = i+1; j < sm.size2(); j++){
            output << sm(i, j) << ",";
        }
    }
    output.close();
}
