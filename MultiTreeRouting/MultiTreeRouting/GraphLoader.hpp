//
//  GraphLoader.hpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 27.02.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#ifndef GraphLoader_hpp
#define GraphLoader_hpp

#include <stdio.h>
#include <utility>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>

using Edge = std::pair<std::string, std::string>;
using VSet = std::set<std::string>;

//this class parses the file of the graph and loads it into the program
class GraphLoader{
private:
    int edgeNum;
    int VertexNum;
    std::vector<Edge> edges;
    VSet verticesLabels;
    void addPossibleVertices(const std::string& v1, const std::string& v2);
    bool checkIfCaidaFile(const std::string& filePath);
    
public:
    GraphLoader(const char * filePath);
    const int getEdgeNum();
    const int getVertexNum();
    const std::vector<Edge>& getEdgeList();
    const VSet& getVerticesLabels();
};
#endif /* GraphLoader_hpp */
