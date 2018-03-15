//
//  TreeWorker.hpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 15.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#ifndef TreeWorker_hpp
#define TreeWorker_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
class TreeWorker{
public:
    TreeWorker(std::vector<int>* dV, std::vector<int>* pV);
    int lca(int node1, int node2);
    
private:
    std::vector<int>* distanceVector;
    std::vector<int>* predecessorVector;
};

#endif /* TreeWorker_hpp */
