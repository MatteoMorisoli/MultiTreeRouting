//
//  TreeWorker.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 15.03.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "TreeWorker.hpp"

//TODO rename, specify better the function of the class
//constructor
TreeWorker::TreeWorker(const std::vector<int>& dV, const std::vector<int>& pV, const int rootNode, const Graph& graph): distanceVector(dV), predecessorVector(pV), rootNode(rootNode), graph(graph), sparseTable(predecessorVector.size(), std::vector<int>(((int) log2(pV.size())) + 1, -1)){
    preprocessingLca();
    int max_distance = *std::max_element(dV.begin(), dV.end());
    for(int i = 0; i < max_distance + 1 ; i++){
        computedLogs.push_back(log2_fast(i));
    }
}

//finds the lowest common ancestor between the two given nodes node1 and node2, returns the int of the ancestor
int TreeWorker::lca(int node1, int node2){
    int startingDepth;
    int newNode1 = node1;
    int newNode2 = node2;
    //start from the lowest of the 2 nodes in the tree and go up until the level of the second node is reached
    if(distanceVector[node1] > distanceVector[node2]){
        startingDepth = distanceVector[newNode2];
        int depth1 = distanceVector[newNode1];
        while(depth1 > startingDepth){
            newNode1 = predecessorVector[newNode1];
            depth1--;
        }
    }else if(distanceVector[node1] < distanceVector[node2]){
        startingDepth = distanceVector[newNode1];
        int depth2 = distanceVector[newNode2];
        while(depth2 > startingDepth){
            newNode2 = predecessorVector[newNode2];
            depth2--;
        }
    }else{
        startingDepth = distanceVector[newNode1];
    }
    //having now 2 nodes at the same level, starts comparing them, if equal, return as it is the LCA, otherwise, take parents and go on
    while(startingDepth >= 0){
        if(newNode1 == newNode2){
            return newNode1;
        }else{
            newNode1 = predecessorVector[newNode1];
            newNode2 = predecessorVector[newNode2];
            startingDepth--;
        }
    }
    return newNode1;
    
}

void TreeWorker::preprocessingLca(){
    for(int i = 0; i < sparseTable.size(); i++){
        if( predecessorVector[i] != i){
            sparseTable[i][0] = predecessorVector[i];
        }
    }
    
    for(int j = 1; (1 << j) < sparseTable.size(); j++){
        for(int i = 0; i < sparseTable.size(); i++){
            if(sparseTable[i][j-1] != -1){
                sparseTable[i][j] = sparseTable[sparseTable[i][j-1]][j-1];
            }
        }
    }
    
}

int TreeWorker::log2_fast(int d) {
    int result;
    std::frexp(d, &result);
    return result-1;
}

int TreeWorker::optimizedLca(int node1, int node2){
    int distance;
    int distanceLog;
    if(distanceVector[node1] > distanceVector[node2]){
        distance = distanceVector[node1] - distanceVector[node2];
        while(distance > 0){
            distanceLog = computedLogs[distance];
            node1 = sparseTable[node1][distanceLog];
            int distanceLogSquared = 1 << distanceLog;
            distance -= distanceLogSquared;
        }
    }else if(distanceVector[node2] > distanceVector[node1]){
        distance = distanceVector[node2] - distanceVector[node1];
        while(distance > 0){
            distanceLog = computedLogs[distance];
            node2 = sparseTable[node2][distanceLog];
            int distanceLogSquared = 1 << distanceLog;
            distance -= distanceLogSquared;
        }
    }
    
    if(node1 == node2){
        return node1;
    }
    int maxLog = (int) log2_fast(sparseTable.size());
    for(int i = maxLog; i >= 0; i--){
        if((sparseTable[node1][i] != -1) && (sparseTable[node1][i] != sparseTable[node2][i])){
            node1 = sparseTable[node1][i];
            node2 = sparseTable[node2][i];
        }
    }
    return predecessorVector[node1];
}

//compute the congestion value for each edge of the tree
Congestion TreeWorker::getCongestion(){
    std::vector<std::pair<int, int>> treeEdges;
    std::vector<std::set<int>> trees(predecessorVector.size());
    for(int i = 0; i < predecessorVector.size(); ++i){
        trees[predecessorVector[i]].insert(i);
        trees[i].insert(i);
        if(i != predecessorVector[i]){
            if(predecessorVector[i] >= i){
                treeEdges.emplace_back(std::make_pair(i, predecessorVector[i]));
            }else{
                treeEdges.emplace_back(std::make_pair(predecessorVector[i], i));
            }
        }
    }
    addSubTrees(rootNode, trees);
    std::vector<int> congestionValues;
    for(std::vector<std::pair<int, int>>::iterator edgeIt = treeEdges.begin(); edgeIt != treeEdges.end(); ++edgeIt){
        int root;
        if(distanceVector[edgeIt->first] > distanceVector[edgeIt->second]){
            root = edgeIt->first;
        }else{
            root = edgeIt->second;
        }
        int counter = 0;
        for(std::set<int>::iterator treeNodesIt = trees[root].begin(); treeNodesIt != trees[root].end(); ++treeNodesIt){
            std::pair<Graph::adjacency_iterator, Graph::adjacency_iterator> range = adjacent_vertices(*treeNodesIt, graph);
            for(Graph::adjacency_iterator neighbourIt = range.first; neighbourIt != range.second; ++neighbourIt){
                if(trees[root].find(*neighbourIt) == trees[root].end()){
                    ++counter;
                }
            }
        }
        congestionValues.emplace_back(counter);
    }
    Congestion c = { treeEdges, congestionValues};
    return c;
}

//modified DFS that computes for each vertex all the vertices that are part of its subtree
void TreeWorker::addSubTrees(const int root, std::vector<std::set<int>>& trees){
    for(std::set<int>::iterator it = trees[root].begin(); it != trees[root].end(); ++it){
        if(*it != root){
            addSubTrees(*it, trees);
        }
    }
    for(std::set<int>::iterator it = trees[root].begin(); it != trees[root].end(); ++it){
        if(*it != root){
            trees[root].insert(trees[*it].begin(), trees[*it].end());
        }
    }
}

std::vector<int> TreeWorker::computeRealDistances(){
    std::vector<int> distances(predecessorVector.size());
    std::vector<std::set<int>> children(predecessorVector.size());
    for(int i = 0; i < predecessorVector.size(); ++i){
        if(i != predecessorVector[i]){
            children[predecessorVector[i]].insert(i);
        }
    }
    doRealDistances(children, distances, rootNode, 0);
    return distances;
}

void TreeWorker::doRealDistances(const std::vector<std::set<int>>& children,  std::vector<int> &distances, const int root, int levelcounter){
    distances[root] = levelcounter;
    ++levelcounter;
    for(auto it = children[root].begin(); it != children[root].end(); ++it){
        doRealDistances(children, distances, *it, levelcounter);
    }
}
