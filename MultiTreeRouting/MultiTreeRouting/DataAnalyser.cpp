//
//  DataAnalyser.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 26.04.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "DataAnalyser.hpp"
//
//  main.cpp
//  DataAnalyzer
//
//  Created by Matteo Morisoli on 12.04.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//
DataAnalyser::DataAnalyser(std::string f, std::string suf, bool isStar, int treeN){
    filePath = f;
    sizeCounter = 0;
    sum = 0;
    hex = {"0", "1", "2", "3", "4", "5", "6", "7", "8" ,"9", "A", "B", "C", "D", "E", "F"};
    suffix = suf;
    treeNum = treeN;
    if(isStar){
        suffix.append("star");
    }else{
        suffix.append("all");
    }
    suffix.append(std::to_string(treeNum));
    
}

void DataAnalyser::addData(diagMatrixFloat &sm){
    for(int i = 0; i < sm.size1(); ++i){
        for(int j = i; j < sm.size2(); j++){
            long double value = sm(i, j);
            sum += value;
            ++sizeCounter;
            if(valueMap.find(value) == valueMap.end()){
                valueMap.insert(std::pair<double, long>(value, 1));
            }else{
                valueMap[value] += 1;
            }
        }
    }
}

void DataAnalyser::addAndPrintCongestionData(diagMatrixFloat &sm, std::map<int, std::string> &nm){
    long double max = -1;
    for(int i = 0; i < sm.size1(); ++i){
        for(int j = i; j < sm.size2(); j++){
            long double value = sm(i, j);
            if(value > max){
                max = value;
            }
            sum += value;
            ++sizeCounter;
            if(valueMap.find(value) == valueMap.end()){
                valueMap.insert(std::pair<double, long>(value, 1));
            }else{
                valueMap[value] += 1;
            }
        }
    }
    std::size_t dotPos = filePath.find_last_of(".");
    std::string graphFilePath = filePath.substr(0, dotPos);
    graphFilePath.append(suffix);
    graphFilePath.append("graph.dot");
    std::ofstream graph(graphFilePath);
    graph << "graph D {\n";
    graph << "  graph[label=\"congestion average for  " << treeNum << " trees\"];\n";
    for(std::map<int, std::string>::iterator it = nm.begin(); it != nm.end(); ++it){
        graph << it->first << " [label=\"" << it->second << "\"];\n";
    }
    std::vector<std::string> nonZeroStrings;
    for(int i = 0; i < sm.size1(); ++i){
        for(int j = i; j < sm.size2(); j++){
            int num = ((int) sm(i, j) * 255 / (max));
            std::stringstream s;
            s << "#" << std::hex << hex[num /16] << hex[num % 16] << "0000";
            std::string h(s.str());
            if(sm(i, j) == 0){
                //graph << i << " -- " << j << "[label=\"" << sm(i,j) << "\"" << ", color=\"" << h << "\", penwidth=3];\n";
            }else{
                graph << i << " -- " << j+1 << "[label=\"" << sm(i,j) << "\"" << ", color=\"" << h << "\", penwidth=3];\n";
            }
        }
    }
    graph << "}\n";
    graph.close();
}

void DataAnalyser::generateStretchFiles(){
    long double mean = sum / (long double) sizeCounter;
    double min = DataAnalyser::valueOfIndex(valueMap, 0);
    double max = DataAnalyser::valueOfIndex(valueMap, sizeCounter);
    long double median;
    long double firstQuartile;
    long double thirdQuartile;
    if(sizeCounter % 2 == 0){
        median = (DataAnalyser::valueOfIndex(valueMap, sizeCounter/2) + valueOfIndex(valueMap, sizeCounter/2 + 1)) / 2.0;
        if(sizeCounter % 4 == 0){
            firstQuartile = (DataAnalyser::valueOfIndex(valueMap, sizeCounter/4) + DataAnalyser::valueOfIndex(valueMap, sizeCounter/4 + 1)) / 2.0;
            thirdQuartile = (DataAnalyser::valueOfIndex(valueMap, 3 * sizeCounter/4) + DataAnalyser::valueOfIndex(valueMap, 3 * sizeCounter/4 + 1)) / 2.0;
        }else{
            firstQuartile = valueOfIndex(valueMap, sizeCounter/4 + 1);
            thirdQuartile = valueOfIndex(valueMap, 3 * sizeCounter/4 + 1);
        }
    }else{
        median = valueOfIndex(valueMap, sizeCounter/2 + 1);
        if(sizeCounter % 4 == 0){
            firstQuartile = (valueOfIndex(valueMap, sizeCounter/4) + valueOfIndex(valueMap, sizeCounter/4 + 1)) / 2.0;
            thirdQuartile = (valueOfIndex(valueMap, 3 * sizeCounter/4) + valueOfIndex(valueMap, 3 * sizeCounter/4 + 1)) / 2.0;
        }else{
            firstQuartile = valueOfIndex(valueMap, sizeCounter/4 + 1);
            thirdQuartile = valueOfIndex(valueMap, 3 * sizeCounter/4 + 1);
        }
    }
    std::size_t dotPos = filePath.find_last_of(".");
    std::string refinedFilePath = filePath.substr(0, dotPos);
    refinedFilePath.append(suffix);
    refinedFilePath.append("refined.csv");
    std::string reviewFilePath = filePath.substr(0, dotPos);
    reviewFilePath.append(suffix);
    reviewFilePath.append("review.txt");
    std::string valueFilePath = filePath.substr(0, dotPos);
    valueFilePath.append(suffix);
    valueFilePath.append("value.txt");
    std::ofstream refined(refinedFilePath);
    refined << min << "," << firstQuartile << "," << median << "," << thirdQuartile << ","  << max;
    refined.close();
    std::ofstream review(reviewFilePath);
    review << "The number of datapoints is: " << sizeCounter << std::endl;
    review << "The minimum is: " << min << std::endl;
    review << "The first quartile is: " << firstQuartile << std::endl;
    review << "The median is: " << median << std::endl;
    review << "The third quartile is: " << thirdQuartile << std::endl;
    review << "The maximum is: " << max << std::endl;
    review << "The mean is: " << mean << std::endl;
    review << "The results are: " << std::endl;
    for(std::map<double, long>::iterator it = valueMap.begin(); it != valueMap.end(); ++it){
        review << "We have " << it->second << " instances of the value " << it->first << std::endl;
    }
    review.close();
    std::ofstream values(valueFilePath);
    for(std::map<double, long>::iterator it = valueMap.begin(); it != valueMap.end(); ++it){
        values << it->first << "," << it->second << std::endl;
    }
}

void DataAnalyser::generateCongestionFiles(){
    long double mean = sum / (long double) sizeCounter;
    double max = DataAnalyser::valueOfIndex(valueMap, sizeCounter);
    std::size_t dotPos = filePath.find_last_of(".");
    std::string reviewFilePath = filePath.substr(0, dotPos);
    reviewFilePath.append(suffix);
    reviewFilePath.append("review.txt");
    std::string valueFilePath = filePath.substr(0, dotPos);
    valueFilePath.append(suffix);
    valueFilePath.append("value.txt");
    std::ofstream review(reviewFilePath);
    review << "The number of datapoints is: " << sizeCounter << std::endl;
    review << "The maximum is: " << max << std::endl;
    review << "The mean is: " << mean << std::endl;
    review << "The results are: " << std::endl;
    for(std::map<double, long>::iterator it = valueMap.begin(); it != valueMap.end(); ++it){
        review << "We have " << it->second << " instances of the value " << it->first << std::endl;
    }
    review.close();
    std::ofstream values(valueFilePath);
    for(std::map<double, long>::iterator it = valueMap.begin(); it != valueMap.end(); ++it){
        values << it->first << "," << it->second << std::endl;
    }
}

double DataAnalyser::valueOfIndex(const std::map<double, long> & map, long index)const{
    long reachedIndex = 0;
    for( std::map<double, long>::const_iterator it = map.begin(); it != map.end(); ++it){
        reachedIndex += it->second;
        if( reachedIndex >= index){
            return it->first;
        }
    }
    return -1.0;
}
