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
    std::ifstream readFile(filePath);
    std::string line;
    while(std::getline(readFile, line)){
        std::istringstream iss(line);
        std::string e1, e2;
        iss >> e1 >> e2;
        addPossibleVertices(e1, e2);
        edges.push_back(Edge(e1, e2));
        edgeNum++;
    }    
}

void GraphLoader::addPossibleVertices(std::string v1, std::string v2){
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
