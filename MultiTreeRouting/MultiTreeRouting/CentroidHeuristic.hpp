//
//  CentroidHeuristic.hpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 07.05.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#ifndef CentroidHeuristic_hpp
#define CentroidHeuristic_hpp

#include <stdio.h>
#include <iostream>
#include "Heuristic.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <ctime>

class CentroidHeuristic: public Heuristic {
private:
    bool computed;
    std::vector<int> maximumDistances;
    void computeMaximumDistances(const DiagMatrixInt& dm);
public:
    CentroidHeuristic(int numV);
    int selectStartingNode(const Graph& graph) override{ return -1;};
    int selectStartingNode(const DiagMatrixInt& dm) override;
};

#endif /* CentroidHeuristic_hpp */
