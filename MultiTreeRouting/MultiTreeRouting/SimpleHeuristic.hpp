//
//  SimpleHeuristic.hpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 13.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#ifndef SimpleHeuristic_hpp
#define SimpleHeuristic_hpp
#include <stdio.h>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include "Heuristic.hpp"
class SimpleHeuristic: public Heuristic{
public:
    std::set<int> selectStartingNodes(int numVertices, int numRoots) const;
};
#endif /* SimpleHeuristic_hpp */
