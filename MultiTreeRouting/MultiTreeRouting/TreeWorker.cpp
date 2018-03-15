//
//  TreeWorker.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 15.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "TreeWorker.hpp"

TreeWorker::TreeWorker(std::vector<int>* dV, std::vector<int>* pV){
    distanceVector = dV;
    predecessorVector = pV;
}

int TreeWorker::lca(int node1, int node2){
    int startingDepth;
    int newNode1 = node1;
    int newNode2 = node2;
    if((*distanceVector)[node1] > (*distanceVector)[node2]){
        startingDepth = (*distanceVector)[newNode2];
        int depth1 = (*distanceVector)[newNode1];
        while(depth1 > startingDepth){
            //std::cout << "changing " << newNode1 << " with " << (*predecessorVector)[newNode1] << " while the other node is " << newNode2 << std::endl;
            newNode1 = (*predecessorVector)[newNode1];
            depth1--;
        }
    }else if((*distanceVector)[node1] < (*distanceVector)[node2]){
        startingDepth = (*distanceVector)[newNode1];
        int depth2 = (*distanceVector)[newNode2];
        while(depth2 > startingDepth){
            //std::cout << "changing " << newNode2 << " with " << (*predecessorVector)[newNode2] << " while the other node is " << newNode1 << std::endl;
            newNode2 = (*predecessorVector)[newNode2];
            //std::cout << newNode2;
            depth2--;
        }
    }else{
        startingDepth = (*distanceVector)[newNode1];
    }
    while(startingDepth >= 0){
        //std::cout << "comparing " << newNode1 << " with " << newNode2 << " at depth " << startingDepth << std::endl;
        if(newNode1 == newNode2){
            return newNode1;
        }else{
            newNode1 = (*predecessorVector)[newNode1];
            newNode2 = (*predecessorVector)[newNode2];
            //std::cout << "comparing " << newNode1 << " with " << newNode2 << " at depth " << startingDepth << std::endl;
            startingDepth--;
        }
    }
    return newNode1;
    
}
