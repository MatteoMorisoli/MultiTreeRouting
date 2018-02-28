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

typedef std::pair<std::vector<int>, std::vector<int>> customEdges;
struct graphInfo {
    customEdges * edges;
    std::set<int> * vertices;
};

graphInfo * loadEdges(const char * filePath);

#endif /* GraphLoader_hpp */
