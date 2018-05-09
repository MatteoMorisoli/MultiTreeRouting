//
//  CentroidHeuristic.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 07.05.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "CentroidHeuristic.hpp"

CentroidHeuristic::CentroidHeuristic(int numV): Heuristic(numV), computed(false){}

void CentroidHeuristic::computeMaximumDistances(const DiagMatrixInt& dm){
    for(int i = 0; i < dm.size1(); ++i){
        int max = 0;
        for(int j = i; j < dm.size2(); ++j){
            if(dm(i, j) >= max){
                max = dm(i, j);
            }
        }
        if(i != 0){
            for(int k = i-1; k >= 0; --k){
                if(dm(k, i-1) >= max){
                    max = dm(k, i-1);
                }
            }
        }
        maximumDistances.push_back(max);
    }
    int maxLast = 0;
    for(int h = 0; h < dm.size1(); ++h){
        if(dm(h, dm.size2()-1) >= maxLast){
            maxLast = dm(h, dm.size2()-1);
        }
    }
    maximumDistances.push_back(maxLast);
}

int CentroidHeuristic::selectStartingNode(const DiagMatrixInt& dm){
    if(!computed){
        CentroidHeuristic::computeMaximumDistances(dm);
        computed = true;
    }
    int minimum = numVertices;
    for(int i = 0; i < maximumDistances.size(); ++i){
        if(minimum > maximumDistances[i]){
            minimum = maximumDistances[i];
        }
    }
    std::vector<int> centroids;
    for(int i = 0; i < maximumDistances.size(); ++i){
        if(maximumDistances[i] == minimum){
            centroids.push_back(i);
        }
    }
    boost::random::uniform_int_distribution<> dist(0, centroids.size()-1);
    boost::random::mt19937 gen(std::time(0));
    int selectedRoot = dist(gen);
    int root = centroids[selectedRoot];
    maximumDistances[root] = numVertices;
    return root;
}
