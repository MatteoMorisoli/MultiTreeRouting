//
//  MatrixLoader.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 21.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "MatrixLoader.hpp"

MatrixLoader::MatrixLoader(std::string fileName){
    name = fileName;
}

bool MatrixLoader::fileExists(){
    std::ifstream infile(name);
    return infile.good();
}

void MatrixLoader::writeMatrix(const DistanceMatrix& d, const int size){
    std::ofstream outputFile(name);
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            outputFile << d[i][j] << " ";
        }
        if(i != size - 1){
            outputFile << std::endl;
        }
    }
}

void MatrixLoader::readMatrix(DistanceMatrix &d, const int size){
    std::ifstream matrixfile(name);
    std::string line;
    for(int i = 0; i < size; ++i){
        std::getline(matrixfile, line);
        std::istringstream iss(line);
        std::string token;
        for(int j = 0; j < size; ++j){
            iss >> token;
            d[i][j] = std::atof(token.c_str());
        }
    }
}
