//
//  MatrixLoader.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 21.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

//TODO possibly merge with graphloader

#include "MatrixLoader.hpp"

MatrixLoader::MatrixLoader(std::string fileName){
    name = fileName;
}

//checks if file with given name exists or not
bool MatrixLoader::fileExists(){
    std::ifstream infile(name);
    return infile.good();
}

//writes already computed all-pairs distance matrix to file
void MatrixLoader::writeMatrix(const DistanceMatrix &d, const int size){
    std::ofstream outputFile(name);
    for(int i = 0; i < size; ++i){
        for(int j = i+1; j < size; ++j){
            outputFile << d[i][j] << " ";
        }
        if(i != size - 1){
            outputFile << std::endl;
        }
    }
    outputFile.close();
}

//reads a file with an already computed distance matrix and loads it
void MatrixLoader::readMatrix(SmartDistanceMatrix &d, const int size){
    std::ifstream matrixfile(name);
    std::string line;
    for(int i = 0; i < size; ++i){
        std::getline(matrixfile, line);
        std::istringstream iss(line);
        std::string token;
        for(int j = i; j < size; ++j){
            iss >> token;
            d(i, j) = std::atof(token.c_str());
        }
    }
    matrixfile.close();
}
