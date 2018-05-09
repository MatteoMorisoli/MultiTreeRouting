//
//  MedianHeuristic.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 07.05.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "MedianHeuristic.hpp"

MedianHeuristic::MedianHeuristic(int numV): Heuristic(numV), computed(false){}

void MedianHeuristic::computeDistanceSums(const DiagMatrixInt& dm){
    for(int i = 0; i < dm.size1(); ++i){
        long totalDistances = 0;
        for(int j = i; j < dm.size2(); ++j){
            totalDistances += dm(i, j);
        }
        if(i != 0){
            for(int k = i-1; k >= 0; --k){
                totalDistances += dm(k, i-1);
            }
        }
        distanceSums.push_back(totalDistances);
    }
    long totalDistancesLast = 0;
    for(int h = 0; h < dm.size1(); ++h){
        totalDistancesLast += dm(h, dm.size2()-1);
    }
    distanceSums.push_back(totalDistancesLast);
}

int MedianHeuristic::selectStartingNode(const DiagMatrixInt& dm){
    if(!computed){
        MedianHeuristic::computeDistanceSums(dm);
        computed = true;
    }
    long minimum = std::numeric_limits<long>::max();
    for(int i = 0; i < distanceSums.size(); ++i){
        if(minimum > distanceSums[i]){
            minimum = distanceSums[i];
        }
    }
    std::vector<int> medians;
    for(int i = 0; i < distanceSums.size(); ++i){
        if(distanceSums[i] == minimum){
            medians.push_back(i);
        }
    }
    boost::random::uniform_int_distribution<> dist(0, medians.size()-1);
    boost::random::mt19937 gen(std::time(0));
    int selectedRoot = dist(gen);
    int root = medians[selectedRoot];
    distanceSums[root] = std::numeric_limits<long>::max();
    return root;
}
