//
//  main.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 19.02.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//
#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include "GraphLoader.hpp"

const char* test1 = "/Users/met/Documents/MultiTreeRouting/MultiTreeRouting/MultiTreeRouting/testGraph1.txt";
const char* test2 = "/Users/met/Documents/MultiTreeRouting/MultiTreeRouting/MultiTreeRouting/testGraph2.txt";
const char* links = "/Users/met/Documents/MultiTreeRouting/topologies/map.20120301";

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    edges *e = loadEdges(test1);
    return 0;
}
