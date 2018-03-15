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
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <iostream>
#include <fstream>
#include "GraphLoader.hpp"
#include "SimpleHeuristic.hpp"
#include "TreeWorker.hpp"
#include "Printer.hpp"

using namespace boost;

using EdgeWeight = property<edge_weight_t, int>;
using VertexName = property<vertex_name_t, std::string> ;
using Graph = adjacency_list<setS, vecS, undirectedS, VertexName, EdgeWeight>;
using vertexDescriptor = graph_traits<Graph>::vertex_descriptor;
using edgeDescriptor = graph_traits<Graph>::edge_descriptor;
using DistanceMatrix = boost::exterior_vertex_property<Graph, int>::matrix_type;
using TreeDistanceMatrix = boost::numeric::ublas::matrix<int>;
using StretchMatrix = boost::numeric::ublas::matrix<double>;

int main(int argc, const char * argv[]) {
    
    
    //Loading the graph from file and file creation as an adjacency list
    GraphLoader g(argv[1]);
    Graph graph;
    int index = 0;
    std::map<std::string, int> mapping;
    for(auto it = g.getVerticesLabels().begin(); it != g.getVerticesLabels().end(); ++it){
        add_vertex(*it, graph);
        mapping.insert(std::pair<std::string, int>(*it, index));
        index++;
    }
    EdgeWeight weight(1);
    for(auto it = g.getEdgeList().begin(); it != g.getEdgeList().end(); ++it){
        add_edge(mapping.at(it->first), mapping.at(it->second), weight, graph);
    }
    

    //condition for number of trees less than number of nodes
    if(num_vertices(graph) < atoi(argv[2])){
        return 0;
    }
  
    
    //Print for checking that all the nodes and edges are included in adjacency list
//    std::cout << "Number of vertices in Adj_List: " << num_vertices(graph) << " and in file: " << g->getVertexNum() << std::endl;
//    std::cout << "Number of edges in Adj_List: " << num_edges(graph) << " and in file: " << g->getEdgeNum() << std::endl;
    

    //retrieval of node names from adjacency list structure
    //TODO streamline retrieval of node names with adjacency list creation
    auto verticesNames = get(vertex_name, graph);
    std::vector<std::string> printableStrings;
    for(int a = 0; a < num_vertices(graph); ++a){
        std::string str = verticesNames[a];
        str.insert(str.size(), 7-str.size(), ' ');
        printableStrings.push_back(str);
    }
    
    
    //Print for connectivity informations
//    std::cout << std::endl << "connectivity informations: " << std::endl;
//    Graph::vertex_iterator vertexIt, vertexEnd;
//    Graph::adjacency_iterator neighbourIt, neighbourEnd;
//    tie(vertexIt, vertexEnd) = vertices(graph);
//    for(; vertexIt != vertexEnd; ++vertexIt){
//        std::cout << verticesNames[*vertexIt] << " is connected with ";
//        tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt, graph);
//        for(; neighbourIt != neighbourEnd; ++neighbourIt){
//            std::cout << verticesNames[*neighbourIt] << " ";
//        }
//        std::cout << std::endl;
//    }
    
    
    //Distance matrix creation and population using Floyd-Warshall
    DistanceMatrix distanceMatrix(num_vertices(graph));
    floyd_warshall_all_pairs_shortest_paths(graph, distanceMatrix);
    
    
    //Floyd-Warshall print of distance matrix
//    std::cout << std::endl;
//    std::cout << "Distance matrix for Floyd-Warshall: " << std::endl;
//    std::cout << "       ";
//    for(int a = 0; a < num_vertices(graph); ++a){
//        std::string str = verticesNames[a];
//        std::cout << str.insert(str.size(), 7-str.size(), ' ');
//    }
//    std::cout << std::endl;
//    for(std::size_t i = 0; i < num_vertices(graph); ++i) {
//        std::string nameStr = verticesNames[i];
//        std::cout << nameStr.insert(nameStr.size(), 7 - nameStr.size(), ' ');
//        for(std::size_t j = 0; j < num_vertices(graph); ++j) {
//            std::string numStr = std::to_string(distanceMatrix[i][j]);
//            std::cout << numStr.insert(numStr.size(), 7 - numStr.size(), ' '); ;
//        }
//        std::cout << std::endl;
//    }
    
    //creation of the trees and of the relative distance matrices and stretch matrices
    std::vector<vertexDescriptor> parents(num_vertices(graph));
    std::vector<int> distances(num_vertices(graph));
    std::vector<TreeDistanceMatrix> treeMatrices;
    std::vector<StretchMatrix> stretchMatrices;
    //heuristic for starting node selection
    SimpleHeuristic e;
    std::set<int> starters = e.SimpleHeuristic::selectStartingNodes(num_vertices(graph), atoi(argv[2]));
    std::vector<int> starterNodes(starters.begin(), starters.end());
    //tree creation for each starting node
    for(std::vector<int>::iterator it = starterNodes.begin(); it != starterNodes.end(); ++it){
        dijkstra_shortest_paths(graph, *it, predecessor_map(&parents[0]).distance_map(&distances[0]));
        std::vector<int> predecessors(parents.begin(), parents.end());
        //distance matrix and stretch matrix creation for each tree
        TreeWorker t(&distances, &predecessors);
        TreeDistanceMatrix treeMatrix(num_vertices(graph), num_vertices(graph));
        StretchMatrix stretchMatrix(num_vertices(graph), num_vertices(graph));
        for(std::size_t i = 0; i < num_vertices(graph); ++i) {
            for(std::size_t j = 0; j < num_vertices(graph); ++j) {
                treeMatrix(i, j) = distances[i] + distances[j] - 2 * distances[t.lca(i, j)];
                if(distanceMatrix[i][j] == 0){
                    stretchMatrix(i, j) = treeMatrix(i, j);
                }else{
                    stretchMatrix(i, j) = treeMatrix(i, j) / (double) distanceMatrix[i][j];
                }
            }
        }
        treeMatrices.emplace_back(treeMatrix);
        stretchMatrices.emplace_back(stretchMatrix);
//        Graph::vertex_iterator vertexIt, vertexEnd;
//        tie(vertexIt, vertexEnd) = vertices(graph);
//        for(; vertexIt != vertexEnd; ++vertexIt){
//            std::cout << "distance(" << verticesNames[*vertexIt] << ") = " << distances[*vertexIt] << ", ";
//            std::cout << "parent(" << verticesNames[*vertexIt] << ") = " << verticesNames[parents[*vertexIt]] << std::endl;
//        }
//        std::cout << std::endl;
    }
    StretchMatrix stretchStar(num_vertices(graph), num_vertices(graph));
    for(std::vector<StretchMatrix>::iterator it = stretchMatrices.begin(); it != stretchMatrices.end(); ++it){
        stretchStar += *it;
        
    }
    stretchStar = stretchStar / atof(argv[2]);
    
    
    //prints nicely distance matrices, stretch matrices and the S* stretch matrix
    Printer p;
//    for(int i = 0; i < starterNodes.size(); ++i){
//        p.printUblasTreeMatrix(treeMatrices[i], printableStrings, starterNodes[i]);
//    }
//    for(int i = 0; i < starterNodes.size(); ++i){
//        p.printUblasStretchMatrix(stretchMatrices[i], printableStrings, starterNodes[i]);
//    }
    p.printUblasStretchStarMatrix(stretchStar, printableStrings);
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();
    double sum = 0;
    std::vector<double> sorted_elements;
    for(int i = 0; i < stretchStar.size1(); ++i){
        for(int j = 0; j < stretchStar.size2(); j++){
            sum += stretchStar(i, j);
            sorted_elements.emplace_back(stretchStar(i, j));
            if(stretchStar(i, j) >= max){
                max = stretchStar(i, j);
            }
            if(stretchStar(i, j) <= min && stretchStar(i, j) >= 1.0){
                min = stretchStar(i, j);
            }
        }
    }
    int elementNum = stretchStar.size1() * stretchStar.size2();
    double mean = sum / elementNum;
    std::sort(sorted_elements.begin(), sorted_elements.end());
    double median = sorted_elements[std::floor(elementNum/2)];
    std::cout << "The maximum value of the matrix is: " << max << std::endl;
    std::cout << "The minimum value of the matrix is: " << min << std::endl;
    std::cout << "The mean value of the matrix is: " << mean << std::endl;
    std::cout << "The median of the matrix is: " << median << std::endl;
    return 0;
}
