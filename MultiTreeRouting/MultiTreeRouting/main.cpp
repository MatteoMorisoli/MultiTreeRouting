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
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <iostream>
#include <fstream>
#include "GraphLoader.hpp"
#include "SimpleHeuristic.hpp"
#include "CenterHeuristic.hpp"
#include "TreeWorker.hpp"
#include "Printer.hpp"
#include "MatrixLoader.hpp"
#include <chrono>
#include <thread>
#include "Writer.hpp"

using namespace boost;

using EdgeWeight = property<edge_weight_t, int>;
using VertexName = property<vertex_name_t, std::string> ;
using Graph = adjacency_list<setS, vecS, undirectedS, VertexName, EdgeWeight>;
using vertexDescriptor = graph_traits<Graph>::vertex_descriptor;
using edgeDescriptor = graph_traits<Graph>::edge_descriptor;
using DistanceMatrix = boost::exterior_vertex_property<Graph, int>::matrix_type;
using TreeDistanceMatrix = boost::numeric::ublas::matrix<int>;
using StretchMatrix = boost::numeric::ublas::matrix<double>;

//prototype
void computeTree(const int root, const Graph& g, const DistanceMatrix &dm, StretchMatrix &sm, std::vector<Congestion> &c);


/* Parameters for the main are the path of the file containing the graph, the number of trees to create on that graph,
 the heuristic to use, identified with a integer, and the number of repetitions (use 1 if the selected heuristic is completely deterministic)
 */
int main(int argc, const char * argv[]) {
    
    
    //d\Definitions of the parameters for ease of use
    const char * startingGraphPath = argv[1];
    const int treeNum = atoi(argv[2]);
    const int heuristicType = atoi(argv[3]);
    const int repetitions = atoi(argv[4]);
    
    
    //Loading the graph from file and file creation as an adjacency list
    GraphLoader g(startingGraphPath);
    Graph graph;
    int index = 0;
    //creating the mapping from name to int used in the graph
    std::map<std::string, int> mapping;
    for(auto it = g.getVerticesLabels().begin(); it != g.getVerticesLabels().end(); ++it){
        add_vertex(*it, graph);
        mapping.insert(std::pair<std::string, int>(*it, index));
        index++;
    }
    //setting edge weight at 1 for every edge in our case
    EdgeWeight weight(1);
    for(auto it = g.getEdgeList().begin(); it != g.getEdgeList().end(); ++it){
        add_edge(mapping.at(it->first), mapping.at(it->second), weight, graph);
    }
    //storing the number of vertices in variable for readability
    const int vertexNum = (int) num_vertices(graph);
    
    
    //condition for number of trees less than number of nodes, necessary for certain heuristics
    if(num_vertices(graph) < treeNum){
        return 0;
    }
    std::cout << "graph loaded!" << std::endl;
    
    
    //Distance matrix creation and population using Johnson
    std::string matrixFile(argv[1]);
    matrixFile.replace(matrixFile.size()-4, 4, ".dmf");
    DistanceMatrix distanceMatrix(vertexNum);
    MatrixLoader ml(matrixFile);
    //to save computations, if matrix already computed and in file, load it, otherwise compute it and save it to file
    if(!ml.fileExists()){
        johnson_all_pairs_shortest_paths(graph, distanceMatrix);
        ml.writeMatrix(distanceMatrix, vertexNum);
    }else{
        std::cout << "reading..." << std::endl;
        ml.readMatrix(distanceMatrix, vertexNum);
    }
    std::cout << "johnson done!" << std::endl;
    
    
    //heuristic for starting node selection
    
    Writer w;
    //tree creation for each starting node, selected using the chosen heuristic
    //std::vector<StretchMatrix> stretchStars;
    StretchMatrix stretchStarStar(vertexNum, vertexNum, 0);
    for(int i = 0; i < repetitions; ++i){
        std::cout << "starting repetition " << i+1 << " of " << repetitions << "..." << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        //creation of the trees and of the relative distance matrices and stretch matrices
        //std::vector<TreeDistanceMatrix> treeMatrices; //storage for distance between each couple of nodes on a tree
        //std::vector<StretchMatrix> stretchMatrices; //storage for the stretch matrices for each tree
        std::vector<Congestion> congestions; //storage of congestion informations for each tree
        std::vector<int> starterNodes;
        if(heuristicType == 1){
            CenterHeuristic h;
            std::set<int> simpleStarters = h.CenterHeuristic::selectStartingNodes(vertexNum, graph, treeNum);
            starterNodes.insert(starterNodes.end(), simpleStarters.begin(), simpleStarters.end());
            std::cout << "Heuristic chosen: center" << std::endl;
        }else if(heuristicType == 0){
            SimpleHeuristic h;
            std::set<int> simpleStarters = h.SimpleHeuristic::selectStartingNodes(vertexNum, graph, treeNum);
            starterNodes.insert(starterNodes.end(), simpleStarters.begin(), simpleStarters.end());
            std::cout << "Heuristic chosen: random" << std::endl;
        }
        StretchMatrix stretchStar(vertexNum, vertexNum, 0);
        int max_threads = std::thread::hardware_concurrency();
        if(std::thread::hardware_concurrency() > treeNum){
            max_threads = treeNum;
        }
        int repetitions = starterNodes.size() / max_threads;
        std::cout << "doing " << repetitions << " repetitions" << std::endl;
        for(int k = 0; k < repetitions; ++k){
            std::thread threads[max_threads];
            std::vector<StretchMatrix> stretchMatrices(max_threads, StretchMatrix(vertexNum, vertexNum, 0));
            for(int j = 0; j < max_threads; ++j){ //it = starterNodes.begin(); it != starterNodes.end(); ++it){
                threads[j] = std::thread(computeTree, starterNodes[j + 8 * k], std::cref(graph), std::cref(distanceMatrix), std::ref(stretchMatrices[j]), std::ref(congestions));
            //asio::post(pool, bind(computeTree, starterNodes[j], std::cref(graph), std::cref(distanceMatrix), std::ref(stretchMatrices[j]), std::ref(congestions)));
            //std::cout << "tree computing..." << std::endl;
            }
        //pool.join();
            for(int j = 0; j < max_threads; ++j){ //it = starterNodes.begin(); it != starterNodes.end(); ++it){
                threads[j].join();
            }
            for(int j = 0; j < max_threads; ++j){ //it = starterNodes.begin(); it != starterNodes.end(); ++it){
                stretchStar += stretchMatrices[j];
            }
        }
        
        stretchStar = stretchStar / treeNum;
        w.writeStretchStars(stretchStar, std::string(startingGraphPath), treeNum);
        stretchStarStar += stretchStar;
        //stretchStars.emplace_back(stretchStar);
        std::cout << "repetition " << i << " done!" << std::endl;
    }
//    for(std::vector<StretchMatrix>::iterator it = stretchStars.begin(); it != stretchStars.end(); ++it){
//        stretchStarStar += *it;
//    }
    stretchStarStar = stretchStarStar / repetitions;
    
    //saving data on file
    std::cout << "Saving data on file..." << std::endl;
    //w.writeStretchStars(stretchStars, std::string(startingGraphPath), treeNum);
    w.writeStretchStarStar(stretchStarStar, std::string(startingGraphPath), treeNum);
    std::cout << "Saving done!" << std::endl;
    
    //Statistics
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();
    double sum = 0;
    std::vector<double> sorted_elements;
    for(int i = 0; i < stretchStarStar.size1(); ++i){
        for(int j = i+1; j < stretchStarStar.size2(); j++){
            sum += stretchStarStar(i, j);
            sorted_elements.emplace_back(stretchStarStar(i, j));
            if(stretchStarStar(i, j) >= max){
                max = stretchStarStar(i, j);
            }
            if(stretchStarStar(i, j) <= min && stretchStarStar(i, j) >= 1.0){
                min = stretchStarStar(i, j);
            }
        }
    }
    double mean = sum / sorted_elements.size();
    std::sort(sorted_elements.begin(), sorted_elements.end());
    double median = sorted_elements[std::floor(sorted_elements.size()/2)];
    std::cout << "The maximum value of the matrix is: " << max << std::endl;
    std::cout << "The minimum value of the matrix is: " << min << std::endl;
    std::cout << "The mean value of the matrix is: " << mean << std::endl;
    std::cout << "The median of the matrix is: " << median << std::endl;
    
    //PRINTER SECTION
    //this are all various prints to visualize the graph/matrices and help debug
    
    //Print for checking that all the nodes and edges are included in adjacency list
//    std::cout << "Number of vertices in Adj_List: " << vertexNum << " and in file: " << g.getVertexNum() << std::endl;
//    std::cout << "Number of edges in Adj_List: " << num_edges(graph) << " and in file: " << g.getEdgeNum() << std::endl;
    
    //Print for connectivity informations
//        std::cout << std::endl << "connectivity informations: " << std::endl;
//        Graph::vertex_iterator vertexIt, vertexEnd;
//        Graph::adjacency_iterator neighbourIt, neighbourEnd;
//        tie(vertexIt, vertexEnd) = vertices(graph);
//        for(; vertexIt != vertexEnd; ++vertexIt){
//            std::cout << verticesNames[*vertexIt] << " is connected with ";
//            tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt, graph);
//            for(; neighbourIt != neighbourEnd; ++neighbourIt){
//                std::cout << verticesNames[*neighbourIt] << " ";
//            }
//            std::cout << std::endl;
//        }
    
    //Print for distance matrices, tree matrices and stretch matrices
//    auto verticesNames = get(vertex_name, graph);
//    std::vector<std::string> printableStrings;
//    for(int a = 0; a < num_vertices(graph); ++a){
//        std::string str = verticesNames[a];
//        str.insert(str.size(), 7-str.size(), ' ');
//        printableStrings.push_back(str);
//    }
//    Printer p;
//    p.printDistanceMatrix(distanceMatrix, printableStrings);
//    for(int i = 0; i < starterNodes.size(); ++i){
//        p.printUblasTreeMatrix(treeMatrices[i], printableStrings, starterNodes[i]);
//    }
//    for(int i = 0; i < starterNodes.size(); ++i){
//        p.printUblasStretchMatrix(stretchMatrices[i], printableStrings, starterNodes[i]);
//    }
//    for(int i = 0; i < starterNodes.size(); ++i){
//        p.printTreeCongestion(congestions[i], printableStrings, argv[1], starterNodes[i]);
//    }
//    for(int i = 0 ; i < repetitions; ++i){
//        p.printUblasStretchStarMatrix(stretchStars[i], printableStrings);
//    }
//    p.printUblasStretchStarMatrix(stretchStarStar, printableStrings);
    return 0;
}

void computeTree(const int root, const Graph& g, const DistanceMatrix &dm, StretchMatrix &sm, std::vector<Congestion> &c){
    std::cout << "starting tree..." << std::endl;
    int vertexNum = num_vertices(g);
    std::vector<vertexDescriptor> parents(vertexNum); //necessary for dijkstra
    std::vector<int> distances(vertexNum); //necessary for dijkstra
    dijkstra_shortest_paths(g, root, predecessor_map(&parents[0]).distance_map(&distances[0]));
    std::vector<int> predecessors(parents.begin(), parents.end());
    TreeWorker t(distances, predecessors, root, g);
    //c.emplace_back(t.getCongestion());
    TreeDistanceMatrix treeMatrix(vertexNum, vertexNum, 0);
    //StretchMatrix stretchMatrix(vertexNum, vertexNum, 0);
    for(std::size_t i = 0; i < vertexNum; ++i) {
                if(i % 1000 == 0){
                    std::cout << i << " columns of the tree computed" << std::endl;
                }
        for(std::size_t j = i+1; j < vertexNum; ++j) {
            treeMatrix(i, j) = distances[i] + distances[j] - 2 * distances[t.lca(i, j)];
            if(dm[i][j] == 0){
                sm(i, j) = treeMatrix(i, j);
            }else{
                sm(i, j) = treeMatrix(i, j) / (double) dm[i][j];
            }
        }
    }
    std::cout << "finished tree!" << std::endl;
    //tdm.emplace_back(treeMatrix);
}

