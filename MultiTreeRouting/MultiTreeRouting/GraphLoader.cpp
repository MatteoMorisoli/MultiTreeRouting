//
//  GraphLoader.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 27.02.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

//TODO total refactor, add matrix loading here, make more flexible

#include "GraphLoader.hpp"

//constructor, opens the file and load the graph in the object
GraphLoader::GraphLoader(const std::string filePath){
    VertexNum = 0;
    edgeNum = 0;
    
    std::cout << "loading graph from file..." << filePath << std::endl;
    //check on windows if it works
    std::ifstream readFile(filePath);
    std::string line;
    bool isCaida = checkIfCaidaFile(filePath);
    while(std::getline(readFile, line)){
        std::istringstream iss(line);
        std::string e1, e2, e3;
        iss >> e1 >> e2 >> e3;
        if(isCaida){
            if((e1 == std::string("D")) || (e1 == std::string("I"))){
                addPossibleVertices(e2, e3);
                edges.emplace_back(Edge(e2, e3));
                ++edgeNum;
            }
        }else{
            addPossibleVertices(e1, e2);
            edges.emplace_back(Edge(e1, e2));
            ++edgeNum;
        }
    }    
}

//worker method to check if vertices are already in set and update the vertex number accordingly
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

//getter method for list of edges
const std::vector<Edge>&  GraphLoader::getEdgeList(){
    return edges;
}

//getter method for set of strings representing all the vertices names
const VSet& GraphLoader::getVerticesLabels(){
    return verticesLabels;
}

//getter for number of edges
const int GraphLoader::getEdgeNum(){
    return edgeNum;
}

//getter for number of vertices
const int GraphLoader::getVertexNum(){
    return VertexNum;
}

bool GraphLoader::checkIfCaidaFile(const std::string &filePath){
    if(filePath.substr(filePath.size()-3, 3) == "txt"){
        return true;
    }else{
        return false;
    }
}
