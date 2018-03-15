//
//  main.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 19.02.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <iostream>
#include <fstream>
#include "GraphLoader.hpp"
#include "SimpleHeuristic.hpp"

using namespace boost;

using EdgeWeight = property<edge_weight_t, int>;
using VertexName = property<vertex_name_t, std::string> ;
using Graph = adjacency_list<setS, vecS, undirectedS, VertexName, EdgeWeight>;
using vertexDescriptor = graph_traits<Graph>::vertex_descriptor;
using edgeDescriptor = graph_traits<Graph>::edge_descriptor;
using DistanceMatrix = boost::exterior_vertex_property<Graph, int>::matrix_type;
using DistanceMatrixMap = boost::exterior_vertex_property<Graph, int>::matrix_map_type;

int main(int argc, const char * argv[]) {
    
    //parameters to select, graph and starting node for dijsktra
    GraphLoader * g = new GraphLoader(argv[1]);
    
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
    //std::cout << "Number of vertices in Adj_List: " << num_vertices(graph) << " and in file: " << g->getVertexNum() << std::endl;
    //std::cout << "Number of edges in Adj_List: " << num_edges(graph) << " and in file: " << g->getEdgeNum() << std::endl;
    
    std::cout << std::endl << "connectivity informations: " << std::endl;
    Graph::vertex_iterator vertexIt, vertexEnd;
    Graph::adjacency_iterator neighbourIt, neighbourEnd;
    tie(vertexIt, vertexEnd) = vertices(graph);
    auto verticesNames = get(vertex_name, graph);
    for(; vertexIt != vertexEnd; ++vertexIt){
        std::cout << verticesNames[*vertexIt] << " is connected with ";
        tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt, graph);
        for(; neighbourIt != neighbourEnd; ++neighbourIt){
            std::cout << verticesNames[*neighbourIt] << " ";
        }
        std::cout << std::endl;
    }
    
    DistanceMatrix dG(num_vertices(graph));
    
    floyd_warshall_all_pairs_shortest_paths(graph, dG);
    std::cout << std::endl;
    std::cout << "Distance matrix for Floyd-Warshall: " << std::endl;
    std::cout << "       ";
    for(int a = 0; a < num_vertices(graph); a++){
        std::string str = verticesNames[a];
        std::cout << str.insert(str.size(), 7-str.size(), ' ');
    }
    std::cout << std::endl;
    for(std::size_t i = 0; i < num_vertices(graph); ++i) {
        std::string nameStr = verticesNames[i];
        std::cout << nameStr.insert(nameStr.size(), 7 - nameStr.size(), ' ');
        for(std::size_t j = 0; j < num_vertices(graph); ++j) {
            std::string numStr = std::to_string(dG[i][j]);
            std::cout << numStr.insert(numStr.size(), 7 - numStr.size(), ' '); ;
        }
        std::cout << std::endl;
    }
    
    
    std::cout << std::endl << "Dijkstra distances: " << std::endl;
    std::vector<vertexDescriptor> parents(num_vertices(graph));
    std::vector<int> distances(num_vertices(graph));
    SimpleHeuristic * e = new SimpleHeuristic();
    std::set<int> starterNodes = e->SimpleHeuristic::selectStartingNodes(num_vertices(graph), atoi(argv[2]));
    for(std::set<int>::iterator it = starterNodes.begin(); it != starterNodes.end(); ++it){
        std::cout << "starting node : " << verticesNames[*it] << std::endl;
        dijkstra_shortest_paths(graph, *it, predecessor_map(&parents[0]).distance_map(&distances[0]));
        tie(vertexIt, vertexEnd) = vertices(graph);
        for(; vertexIt != vertexEnd; ++vertexIt){
            std::cout << "distance(" << verticesNames[*vertexIt] << ") = " << distances[*vertexIt] << ", ";
            std::cout << "parent(" << verticesNames[*vertexIt] << ") = " << verticesNames[parents[*vertexIt]] << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}
