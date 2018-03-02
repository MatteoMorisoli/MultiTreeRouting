//
//  GraphLoader.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 27.02.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "GraphLoader.hpp"
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>

graphInfo * loadEdges(const char * filePath){
    
    std::vector<int> edge1;
    std::vector<int> edge2;
    std::cout << "loading graph from file..." << filePath << std::endl;
    std::ifstream readFile(filePath);
    std::string line;
    std::set<int> vertices;
    while(std::getline(readFile, line)){
        std::istringstream iss(line);
        int e1, e2;
        iss >> e1 >> e2;
        vertices.insert(e1);
        vertices.insert(e2);
        edge1.push_back(e1);
        edge2.push_back(e2);
    }
    /*for (auto i = edge1.begin(); i != edge1.end(); ++i)
        std::cout << *i << ' ';
    for (auto j = edge2.begin(); j != edge2.end(); ++j)
        std::cout << *j << ' ';
     */
    //std::cout << "Number of vertices: " << vertices.size() << std::endl;
    //std::cout << edge1.size() << " " << edge2.size() << std::endl;
    graphInfo * z = new graphInfo{new customEdges(edge1, edge2), new std::set<int>(vertices)};
    return z;
}
