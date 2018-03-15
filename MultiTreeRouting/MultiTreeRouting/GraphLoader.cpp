//
//  GraphLoader.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 27.02.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "GraphLoader.hpp"

GraphLoader::GraphLoader(const char * filePath){
    VertexNum = 0;
    edgeNum = 0;
    
    std::cout << "loading graph from file..." << filePath << std::endl;
    //check on windows if it works
    std::ifstream readFile(filePath);
    std::string line;
    while(std::getline(readFile, line)){
        std::istringstream iss(line);
        std::string e1, e2, e3;
        iss >> e1 >> e2 >> e3;
        if((e1 == std::string("D")) || (e1 == std::string("I"))){
            addPossibleVertices(e2, e3);
            edges.emplace_back(Edge(e2, e3));
            edgeNum++;
        }
    }    
}

void GraphLoader::addPossibleVertices(const std::string& v1, const std::string& v2){
    std::pair<std::set<std::string>::iterator, bool> verification1 = verticesLabels.insert(v1);
    if(verification1.second){
        VertexNum++;
    }
    std::pair<std::set<std::string>::iterator, bool> verification2 = verticesLabels.insert(v2);
    if(verification2.second){
        VertexNum++;
    }
}

const std::vector<Edge>&  GraphLoader::getEdgeList(){
    return edges;
}

const VSet& GraphLoader::getVerticesLabels(){
    return verticesLabels;
}

const int GraphLoader::getEdgeNum(){
    return edgeNum;
}

const int GraphLoader::getVertexNum(){
    return VertexNum;
}
