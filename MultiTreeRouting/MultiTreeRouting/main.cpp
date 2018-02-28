//
//  main.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 19.02.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <iostream>
#include <fstream>
#include "GraphLoader.hpp"

using namespace boost;
const char* test1 = "/Users/met/Documents/MultiTreeRouting/MultiTreeRouting/MultiTreeRouting/testGraph1.txt";
const char* test2 = "/Users/met/Documents/MultiTreeRouting/MultiTreeRouting/MultiTreeRouting/testGraph2.txt";
const char* test3 = "/Users/met/Documents/MultiTreeRouting/MultiTreeRouting/MultiTreeRouting/testGraph3.txt";
const char* links = "/Users/met/Documents/MultiTreeRouting/topologies/map.20120301";

typedef adjacency_list<setS, vecS, undirectedS> Graph;

int main(int argc, const char * argv[]) {
    
    graphInfo *g = loadEdges(test2);
    
    Graph graph1;
    for(int i = 0; i < g->edges->first.size(); i++){
        add_edge(g->edges->first.at(i),g->edges->second.at(i), graph1);
    }
    //remove_vertex(0, graph1);
    std::cout << "Number of edges: " << num_edges(graph1) << std::endl;
    Graph::vertex_iterator vertexIt, vertexEnd;
    Graph::adjacency_iterator neighbourIt, neighbourEnd;
    tie(vertexIt, vertexEnd) = vertices(graph1);
    for (; vertexIt != vertexEnd; ++vertexIt)
    {
        std::cout << *vertexIt << " is connected with ";
        tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt, graph1);
        for (; neighbourIt != neighbourEnd; ++neighbourIt)
            std::cout << *neighbourIt << " ";
        std::cout << std::endl;
    }
    dij;
    return 0;
}
