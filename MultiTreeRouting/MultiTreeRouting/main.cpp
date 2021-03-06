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
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <iostream>
#include <fstream>
#include "GraphLoader.hpp"
#include "SimpleHeuristic.hpp"
#include "CenterHeuristic.hpp"
#include "CentroidHeuristic.hpp"
#include "MedianHeuristic.hpp"
#include "TreeWorker.hpp"
#include "MatrixLoader.hpp"
#include "DataAnalyser.hpp"
#include "ConfigLoader.hpp"

using namespace boost;

using EdgeWeight = property<edge_weight_t, int>;
using VertexName = property<vertex_name_t, std::string> ;
using Graph = adjacency_list<setS, vecS, undirectedS, VertexName, EdgeWeight>;
using vertexDescriptor = graph_traits<Graph>::vertex_descriptor;
using edgeDescriptor = graph_traits<Graph>::edge_descriptor;
using DistanceMatrix = boost::exterior_vertex_property<Graph, int>::matrix_type;
using DiagMatrixInt = boost::numeric::ublas::triangular_matrix<int, boost::numeric::ublas::upper>;
using DiagMatrixFloat = boost::numeric::ublas::triangular_matrix<float, boost::numeric::ublas::upper>;

//prototype
void computeTreeStretch(const int root, Graph& g, const DiagMatrixInt &dm, DiagMatrixFloat &sm, const bool evo);
void computeTreeCongestion(const int root, Graph& g, DiagMatrixFloat &cm, const bool evo);
void increaseEdgeWeights(Graph& g, const std::vector<int>& parents);
void resetEdgeWeights(Graph& g);


/* The Parameter for the main is the path of the file containing the configuration informations
 */
int main(int argc, const char * argv[]) {
    
    std::vector<std::string> suffixes = {"random", "center", "centroid", "median", "randomEvo", "centerEvo", "centroidEvo", "medianEvo", "kruskal", "kruskalEvo"};
    std::vector<std::string> metrics = {"stretch", "congestion"};
    //initialization of config object, and selection of evo boolean
    ConfigLoader config(argv[1]);
    bool evo = false;
    if(config.getHeuristicId() > 4 || config.getHeuristicId() == 9){
        evo = true;
    }
    //selecting correct suffix for file naming
    std::string suffix = suffixes[config.getHeuristicId()];
    suffix.append(metrics[config.getMetricId()]);
    
    //graph declaration
    Graph graph;
    bool computedGraph = false;
    std::map<int, std::string> nameMapping;
    //main loop for all experiments in configuration
    for(int a = 0; a < config.getExperiments(); ++a){
        //Loading the graph from file and file creation as an adjacency list
        if(!computedGraph){GraphLoader g(config.getGraphPath());

            int index = 0;
            std::map<std::string, int> mapping;
            //creating the mapping from name to int used in the graph
            for(auto it = g.getVerticesLabels().begin(); it != g.getVerticesLabels().end(); ++it){
                add_vertex(*it, graph);
                mapping.insert(std::pair<std::string, int>(*it, index));
                nameMapping.insert(std::pair<int, std::string>(index, *it));
                index++;
            }
            //setting edge weight at 1 for every edge in our case
            EdgeWeight weight(1);
            for(auto it = g.getEdgeList().begin(); it != g.getEdgeList().end(); ++it){
                add_edge(mapping.at(it->first), mapping.at(it->second), weight, graph);
            }
            computedGraph = true;
        }
        
        
        //storing the number of vertices in variable for readability
        const int vertexNum = (int) num_vertices(graph);
        //condition for number of trees less than number of nodes, necessary for certain heuristics
        std::cout << "graph loaded!" << std::endl;
        std::cout << "Number of vertices: " << vertexNum << std::endl;
        std::cout << "Number of edges: " << num_edges(graph) << std::endl;
        
        
        //Distance matrix creation and population using Johnson
        DiagMatrixInt diagDistanceMatrix(vertexNum-1, vertexNum-1);
        {DistanceMatrix distanceMatrix(vertexNum);
            std::size_t dotPos = config.getGraphPath().find_last_of(".");
            std::string matrixFileName = config.getGraphPath().substr(0, dotPos);
            matrixFileName.append(".dmf");
            MatrixLoader ml(matrixFileName);
            //to save computations, if matrix already computed and in file, load it, otherwise compute it and save it to file
            if(!ml.fileExists()){
                std::cout << "computing..." << std::endl;
                johnson_all_pairs_shortest_paths(graph, distanceMatrix);
                ml.writeMatrix(distanceMatrix, vertexNum);
                for(int i = 0; i < vertexNum - 1; ++i){
                    for(int j = i+1; j < vertexNum; ++j){
                        diagDistanceMatrix(i, j-1) = distanceMatrix[i][j];
                    }
                }
            }else{
                std::cout << "reading..." << std::endl;
                ml.readMatrix(diagDistanceMatrix, vertexNum-1);
            }
        }
        std::cout << "all-pairs smart matrix ready!" << std::endl;
        
        
        //setup of the matrices to store results
        DiagMatrixFloat resultStarStar(vertexNum-1, vertexNum-1);
        DiagMatrixFloat resultStar(vertexNum-1, vertexNum-1);
        for(int i = 0; i < vertexNum - 1; ++i){
            for(int j = i; j < vertexNum -1; ++j){
                resultStarStar(i, j) = 0;
                resultStar(i, j) = 0;
            }
        }
        //for all repetitions in configuration
        DataAnalyser dAll(config.getGraphPath(), suffix, false, config.getTreeNumbers()[a]);
        for(int i = 0; i < config.getRepetitions(); ++i){
            for(int k = 0; k < vertexNum - 1; ++k){
                for(int j = k; j < vertexNum -1; ++j){
                    resultStar(k, j) = 0;
                }
            }
            std::cout << "starting repetition " << i+1 << " of " << config.getRepetitions() << "..." << std::endl;
            //heuristic selection
            Heuristic * h;
            bool graphNeeded = true;
            if((config.getHeuristicId() == 0) || (config.getHeuristicId() == 4)){
                h = new SimpleHeuristic(vertexNum);
                std::cout << "Heuristic chosen: random" << std::endl;
            }else if((config.getHeuristicId() == 1) || (config.getHeuristicId() == 5)){
                h = new CenterHeuristic(vertexNum);
                std::cout << "Heuristic chosen: center" << std::endl;
            }else if((config.getHeuristicId() == 2) || (config.getHeuristicId() == 6)){
                h = new CentroidHeuristic(vertexNum);
                graphNeeded = false;
                std::cout << "Heuristic chosen: centroid" << std::endl;
            }else if((config.getHeuristicId() == 3) || (config.getHeuristicId() == 7)){
                h = new MedianHeuristic(vertexNum);
                graphNeeded = false;
                std::cout << "Heuristic chosen: median" << std::endl;
            }
            //StretchMatrix stretchStar(vertexNum, vertexNum, 0);
            DiagMatrixFloat sm(vertexNum-1, vertexNum-1);
            //for all trees in selected experiment for this repetition in configuration
            for(int x=0; x < config.getTreeNumbers()[a]; ++x){
                if(config.getMetricId() == 0){
                    int root;
                    if(graphNeeded){
                        root = h->selectStartingNode(graph);
                    }else{
                        root = h->selectStartingNode(diagDistanceMatrix);
                    }
                    computeTreeStretch(root, graph, diagDistanceMatrix, sm, evo);
                }else if(config.getMetricId() == 1){
                    for(int k = 0; k < vertexNum - 1; ++k){
                        for(int j = k; j < vertexNum -1; ++j){
                            sm(k, j) = 0;
                        }
                    }
                    int root;
                    if(graphNeeded){
                        root = h->selectStartingNode(graph);
                    }else{
                        root = h->selectStartingNode(diagDistanceMatrix);
                    }
                    computeTreeCongestion(root, graph, sm, evo);
                }
                std::cout << "starting sum" << std::endl;
                for(int y = 0; y < sm.size1(); ++y){
                    for(int z = y; z < sm.size2();++z){
                        resultStar(y, z) += sm(y,z);
                    }
                }
                std::cout << "ended sum" << std::endl;
            }
            //reset edge weights at the end of current repetition
            if(evo){
                resetEdgeWeights(graph);
            }
            std::cout << "starting division" << std::endl;
            resultStar /= config.getTreeNumbers()[a];
            std::cout << "ended division" << std::endl;
            if(config.getMetricId() == 0){
                dAll.addData(resultStar);
            }else if(config.getMetricId() == 1){
                if(config.getRepetitions() == 1){
                    dAll.addAndPrintCongestionData(resultStar, nameMapping);
                }else{
                    dAll.addData(resultStar);
                }
            }
            std::cout << "starting sum" << std::endl;
            for(int y = 0; y < sm.size1(); ++y){
                for(int z = y; z < sm.size2();++z){
                    resultStarStar(y, z) += resultStar(y,z);
                }
            }
            std::cout << "ended sum" << std::endl;
            std::cout << "repetition " << i+1 << " done!" << std::endl;
        }
        if(config.getMetricId() == 0){
            dAll.generateStretchFiles();
        }else if(config.getMetricId() == 1){
            dAll.generateCongestionFiles();
        }
        if(config.getRepetitions() != 1){
            resultStarStar /= config.getRepetitions();
            DataAnalyser dStar(config.getGraphPath(), suffix, true, config.getTreeNumbers()[a]);
            if(config.getMetricId() == 0){
                dStar.addData(resultStarStar);
                dStar.generateStretchFiles();
            }else if(config.getMetricId() == 1){
                dStar.addAndPrintCongestionData(resultStarStar, nameMapping);
                dStar.generateCongestionFiles();
            }
            std::cout << "Files generated" << std::endl;
        }
    }
    return 0;
}

//void computeTreeStretch(const int root, Graph& g, const DiagMatrixInt &dm, DiagMatrixFloat &sm, const bool evo){
//    std::cout << "starting tree with root " << root << "..." << std::endl;
//    int vertexNum = num_vertices(g);
//    std::vector<vertexDescriptor> parents(vertexNum); //necessary for dijkstra
//    std::vector<int> distances(vertexNum); //necessary for dijkstra
//    dijkstra_shortest_paths(g, root, predecessor_map(&parents[0]).distance_map(&distances[0]));
//    std::vector<int> predecessors(parents.begin(), parents.end());
//    TreeWorker t(distances, predecessors, root, g);
//    if(evo){
//        std::vector<int> realDistances = t.computeRealDistances();
//        for(int i = 0; i < vertexNum-1; ++i) {
//            if(i % 1000 == 0){
//                std::cout << ((double) i)/vertexNum*100.0 << "% tree with root " << root << std::endl;
//            }
//            for(int j = i; j < vertexNum-1; ++j) {
//                sm(i, j) = (realDistances[i] + realDistances[j+1] - 2 * realDistances[t.lca(i, j+1)]) / (float) dm(i, j);
//            }
//        }
//        increaseEdgeWeights(g, predecessors);
//    }else{
//        for(int i = 0; i < vertexNum-1; ++i) {
//                    if(i % 1000 == 0){
//                        std::cout << ((double) i)/vertexNum*100.0 << "% tree with root " << root << std::endl;
//                    }
//            for(int j = i; j < vertexNum-1; ++j) {
//                    sm(i, j) = (distances[i] + distances[j+1] - 2 * distances[t.lca(i, j+1)]) / (float) dm(i, j);
//            }
//        }
//    }
//    std::cout << "finished tree with root " << root << std::endl;
//}

void computeTreeStretch(const int root, Graph& g, const DiagMatrixInt &dm, DiagMatrixFloat &sm, const bool evo){
    std::cout << "starting tree with root " << root << "..." << std::endl;
    int vertexNum = num_vertices(g);
    std::vector<vertexDescriptor> parents(vertexNum); //necessary for dijkstra
    std::vector<int> distances(vertexNum); //necessary for dijkstra
    dijkstra_shortest_paths(g, root, predecessor_map(&parents[0]).distance_map(&distances[0]));
    std::vector<int> predecessors(parents.begin(), parents.end());
    TreeWorker t(distances, predecessors, root, g);
    if(evo){
        std::vector<int> realDistances = t.computeRealDistances();
        for(int i = 0; i < vertexNum-1; ++i) {
            if(i % 1000 == 0){
                std::cout << ((double) i)/vertexNum*100.0 << "% tree with root " << root << std::endl;
            }
            for(int j = i; j < vertexNum-1; ++j) {
                sm(i, j) = (realDistances[i] + realDistances[j+1] - 2 * realDistances[t.optimizedLca(i, j+1)]) / (float) dm(i, j);
            }
        }
        increaseEdgeWeights(g, predecessors);
    }else{
        for(int i = 0; i < vertexNum-1; ++i) {
            if(i % 1000 == 0){
                std::cout << ((double) i)/vertexNum*100.0 << "% tree with root " << root << std::endl;
            }
            for(int j = i; j < vertexNum-1; ++j) {
                sm(i, j) = (distances[i] + distances[j+1] - 2 * distances[t.optimizedLca(i, j+1)]) / (float) dm(i, j);
            }
        }
    }
    std::cout << "finished tree with root " << root << std::endl;
}

void computeTreeCongestion(const int root, Graph& g, DiagMatrixFloat &cm, const bool evo){
    int vertexNum = num_vertices(g);
    std::vector<vertexDescriptor> parents(vertexNum); //necessary for dijkstra
    std::vector<int> distances(vertexNum); //necessary for dijkstra
    dijkstra_shortest_paths(g, root, predecessor_map(&parents[0]).distance_map(&distances[0]));
    std::vector<int> predecessors(parents.begin(), parents.end());
    TreeWorker t(distances, predecessors, root, g);
    Congestion congestion = t.getCongestion();
    for(int i = 0; i < congestion.edges.size(); ++i){
        cm(congestion.edges[i].first, congestion.edges[i].second -1) = congestion.congestionValues[i];
    }
    if(evo){
        increaseEdgeWeights(g, predecessors);
    }
}

void increaseEdgeWeights(Graph& g, const std::vector<int>& parents){
    std::cout << "increasing weights" << std::endl;
    using edgeD = Graph::edge_descriptor;
    for(int i = 0; i < parents.size(); ++i){
        if(i != parents[i]){
            std::pair<edgeD, bool> e = edge(i, parents[i], g);
            int w = get(edge_weight_t(), g, e.first);
            boost::put(boost::edge_weight_t(), g, e.first, w + 1);
        }
    }
}

void resetEdgeWeights(Graph& g){
    std::cout << "resetting weights" << std::endl;
    graph_traits<Graph>::edge_iterator eIt, eEnd;
    for(tie(eIt, eEnd) = edges(g); eIt != eEnd; ++eIt){
        boost::put(boost::edge_weight_t(), g, *eIt, 1);
    }
}

