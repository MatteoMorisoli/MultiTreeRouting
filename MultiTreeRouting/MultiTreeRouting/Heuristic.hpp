//
//  Heuristic.hpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 13.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#ifndef Heuristic_hpp
#define Heuristic_hpp

#include <stdio.h>
#include <set>

class Heuristic{
public:
    virtual std::set<int> selectStartingNodes(int numVertices, int numRoots) = 0;
};

#endif /* Heuristic_hpp */
