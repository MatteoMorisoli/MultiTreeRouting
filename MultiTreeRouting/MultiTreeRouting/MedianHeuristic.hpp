//
//  MedianHeuristic.hpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 07.05.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#ifndef MedianHeuristic_hpp
#define MedianHeuristic_hpp

#include <stdio.h>
#include <iostream>
#include "Heuristic.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <ctime>
#include <limits>

class MedianHeuristic: public Heuristic {
private:
    bool computed;
    std::vector<long> distanceSums;
    void computeDistanceSums(const DiagMatrixInt& dm);
public:
    MedianHeuristic(int numV);
    int selectStartingNode(const Graph& graph) override{ return -1;};
    int selectStartingNode(const DiagMatrixInt& dm) override;
};
#endif /* MedianHeuristic_hpp */
