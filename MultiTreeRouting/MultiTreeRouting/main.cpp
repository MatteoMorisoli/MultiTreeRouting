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

typedef property<edge_weight_t, int> edgeWeight;
typedef adjacency_list<setS, vecS, undirectedS, no_property, edgeWeight> Graph;
typedef graph_traits<Graph>::vertex_descriptor vertexDescriptor;
typedef graph_traits<Graph>::edge_descriptor edgeDescriptor;

int main(int argc, const char * argv[]) {
    
    //parameters to select, graph and starting node for dijsktra
    graphInfo *g = loadEdges(test2);
    int startNode = 4;
    
    Graph graph1;
    edgeWeight weight(1);
    for(int i = 0; i < g->edges->first.size(); i++){
        add_edge(g->edges->first.at(i),g->edges->second.at(i), weight, graph1);
    }
    //remove_vertex(0, graph1);
    std::cout << "Number of vertices: " << num_vertices(graph1) << std::endl;
    std::cout << "Number of edges: " << num_edges(graph1) << std::endl;
    Graph::vertex_iterator vertexIt, vertexEnd;
    Graph::adjacency_iterator neighbourIt, neighbourEnd;
    tie(vertexIt, vertexEnd) = vertices(graph1);
    ++vertexIt;
    for (; vertexIt != vertexEnd; ++vertexIt){
        std::cout << *vertexIt << " is connected with ";
        tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt, graph1);
        for (; neighbourIt != neighbourEnd; ++neighbourIt){
            std::cout << *neighbourIt << " ";
        }
        std::cout << std::endl;
    }
    std::vector<vertexDescriptor> parents(num_vertices(graph1));
    std::vector<int> distances(num_vertices(graph1));
    
    dijkstra_shortest_paths(graph1, startNode, predecessor_map(&parents[0]).distance_map(&distances[0]));
    tie(vertexIt, vertexEnd) = vertices(graph1);
    ++vertexIt;
    for(; vertexIt != vertexEnd; ++vertexIt){
        std::cout << "distance(" << *vertexIt << ") = " << distances[*vertexIt] << ", ";
        std::cout << "parent(" << *vertexIt << ") = " << parents[*vertexIt] << std::endl;
    }
    return 0;
}
