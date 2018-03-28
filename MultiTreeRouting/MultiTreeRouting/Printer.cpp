//
//  Printer.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 15.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "Printer.hpp"

void Printer::printUblasTreeMatrix(const TreeDistanceMatrix &m, const std::vector<std::string>& names, const int root) const{
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
                std::cout << numStr.insert(numStr.size(), 7 - numStr.size(), ' ');
            }
            std::cout << std::endl;
        }
}

void Printer::printUblasStretchMatrix(const StretchMatrix &m, const std::vector<std::string>& names, const int root) const{
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
            std::cout << numStr.insert(numStr.size(), 7 - numStr.size(), ' ');
        }
        std::cout << std::endl;
    }
}


void Printer::printUblasStretchStarMatrix(const StretchMatrix& m, const std::vector<std::string>& names) const{
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
            std::cout << numStr.insert(numStr.size(), 7 - numStr.size(), ' ');
        }
        std::cout << std::endl;
    }
}

void Printer::printTreeCongestion(const Congestion &c, const std::vector<std::string> &names, std::string filePath, int sourceNode) const{
    std::vector<std::string> colors;
    colors.emplace_back("#000000");
    colors.emplace_back("#000000");
    colors.emplace_back("#550000");
    colors.emplace_back("#AA0000");
    colors.emplace_back("#FF0000");
    colors.emplace_back("#FF0000");
    
    std::ofstream dotFile(filePath.replace(filePath.size()-4, 5, std::to_string(sourceNode) + ".dot"));
    dotFile << "graph D {\n";
    dotFile << "  graph[label=\"congestion on tree with root " << names[sourceNode] << "\"];\n";
    for(int i = 0; i < names.size(); ++i){
        dotFile << i << " [label=\"" << names[i] << "\"];\n";
    }
    for(int j = 0; j < c.edges.size(); ++j){
        dotFile << c.edges[j].first << " -- " << c.edges[j].second << "[label=\"" << c.congestionValues[j] << "\"" << ", color=\"" << colors[c.congestionValues[j]] << "\", penwidth=3];\n";
    }
    dotFile << "}\n";
}

void Printer::printDistanceMatrix(const DistanceMatrix &m, const std::vector<std::string> &names) const{

        std::cout << "Distance matrix for Floyd-Warshall: " << std::endl;
        std::cout << "       ";
    for(int i = 0; i < names.size(); ++i){
            std::cout << names[i];
        }
        std::cout << std::endl;
        for(std::size_t i = 0; i < names.size(); ++i) {
            std::cout << names[i];
            for(std::size_t j = 0; j < names.size(); ++j) {
                std::string numStr = std::to_string(m[i][j]);
                std::cout << numStr.insert(numStr.size(), 7 - numStr.size(), ' '); ;
            }
            std::cout << std::endl;
        }
}


