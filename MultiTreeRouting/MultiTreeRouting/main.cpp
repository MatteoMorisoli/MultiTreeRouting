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

using EdgeWeight = property<edge_weight_t, int>;
using VertexName = property<vertex_name_t, std::string> ;
using Graph = adjacency_list<setS, vecS, undirectedS, VertexName, EdgeWeight>;
using vertexDescriptor = graph_traits<Graph>::vertex_descriptor;
using edgeDescriptor = graph_traits<Graph>::edge_descriptor;

int main(int argc, const char * argv[]) {
    
    //parameters to select, graph and starting node for dijsktra
    //graphInfo *g = loadEdges(test3);
    GraphLoader * g = new GraphLoader(test2);
    
    Graph graph;
    int index = 0;
    std::map<std::string, int> mapping;
    for(auto it = g->getVerticesLabels().begin(); it != g->getVerticesLabels().end(); it++){
        add_vertex(*it, graph);
        mapping.insert(std::pair<std::string, int>(*it, index));
        index++;
    }
    EdgeWeight weight(1);
    for(auto it = g->getEdgeList().begin(); it != g->getEdgeList().end(); it++){
        add_edge(mapping.at(it->first), mapping.at(it->second), weight, graph);
    }
    std::cout << "Number of vertices in Adj_List: " << num_vertices(graph) << " and in file: " << g->getVertexNum() << std::endl;
    std::cout << "Number of edges in Adj_List: " << num_edges(graph) << " and in file: " << g->getEdgeNum() << std::endl;
    
    Graph::vertex_iterator vertexIt, vertexEnd;
    Graph::adjacency_iterator neighbourIt, neighbourEnd;
    tie(vertexIt, vertexEnd) = vertices(graph);
    auto verticesNames = get(vertex_name, graph);
    for (; vertexIt != vertexEnd; ++vertexIt){
        std::cout << verticesNames[*vertexIt] << " is connected with ";
        tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt, graph);
        for (; neighbourIt != neighbourEnd; ++neighbourIt){
            std::cout << verticesNames[*neighbourIt] << " ";
        }
        std::cout << std::endl;
    }
    std::vector<vertexDescriptor> parents(num_vertices(graph));
    std::vector<int> distances(num_vertices(graph));
    
    dijkstra_shortest_paths(graph, 4, predecessor_map(&parents[0]).distance_map(&distances[0]));
    tie(vertexIt, vertexEnd) = vertices(graph);
    for(; vertexIt != vertexEnd; ++vertexIt){
        std::cout << "distance(" << verticesNames[*vertexIt] << ") = " << distances[*vertexIt] << ", ";
        std::cout << "parent(" << verticesNames[*vertexIt] << ") = " << verticesNames[parents[*vertexIt]] << std::endl;
    }
    return 0;
}
