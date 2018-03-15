//
//  Printer.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 15.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "Printer.hpp"

void Printer::printUblasTreeMatrix(TreeDistanceMatrix m, std::vector<std::string> names, int root) const{
        std::cout << "Distance matrix for Tree rooted at: " << names[root] << std::endl;
        std::cout << "       ";
        for(int i = 0; i < names.size(); ++i){
            std::cout << names[i];
        }
        std::cout << std::endl;
        for(std::size_t i = 0; i < names.size(); ++i) {
            std::cout << names[i];
            for(std::size_t j = 0; j < names.size(); ++j) {
                std::string numStr = std::to_string(m(i, j));
                std::cout << numStr.insert(numStr.size(), 7 - numStr.size(), ' '); ;
            }
            std::cout << std::endl;
        }
}

void Printer::printUblasStretchMatrix(StretchMatrix m, std::vector<std::string> names, int root) const{
    std::cout << "Stretch matrix for Tree rooted at: " << names[root] << std::endl;
    std::cout << "       ";
    for(int i = 0; i < names.size(); ++i){
        std::cout << names[i];
    }
    std::cout << std::endl;
    for(std::size_t i = 0; i < names.size(); ++i) {
        std::cout << names[i];
        for(std::size_t j = 0; j < names.size(); ++j) {
            std::string numStr = std::to_string(m(i, j));
            numStr.resize(5);
            std::cout << numStr.insert(numStr.size(), 7 - numStr.size(), ' '); ;
        }
        std::cout << std::endl;
    }
}

void Printer::printUblasStretchStarMatrix(StretchMatrix m, std::vector<std::string> names) const{
    std::cout << "Average stretch matrix : " << std::endl;
    std::cout << "       ";
    for(int i = 0; i < names.size(); ++i){
        std::cout << names[i];
    }
    std::cout << std::endl;
    for(std::size_t i = 0; i < names.size(); ++i) {
        std::cout << names[i];
        for(std::size_t j = 0; j < names.size(); ++j) {
            std::string numStr = std::to_string(m(i, j));
            numStr.resize(5);
            std::cout << numStr.insert(numStr.size(), 7 - numStr.size(), ' '); ;
        }
        std::cout << std::endl;
    }
}
