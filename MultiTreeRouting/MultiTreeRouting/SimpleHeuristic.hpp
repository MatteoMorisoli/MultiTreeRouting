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
#include <ctime>

//simple heuristic, will return numRoots distinct random vertices
class SimpleHeuristic: public Heuristic{
private:
public:
    SimpleHeuristic(int numV);
    int selectStartingNode(const Graph& graph) override;
    int selectStartingNode(const DiagMatrixInt& dm) override {return -1;};
};
#endif /* SimpleHeuristic_hpp */
