//
//  main.cpp
//  DataAnalyzer
//
//  Created by Matteo Morisoli on 12.04.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>

double valueOfIndex(const std::map<long double, long> & map, long index);

int main(int argc, const char * argv[]) {
    std::string filePath = std::string(argv[1]);
    std::ifstream datafile(filePath);
    std::string value;
    std::map<long double, long> valueMap;
    long size = 0;
    long double mean = 0;
    long start = 1;
    std::string substring = filePath.substr(filePath.size()-3, 3);
    if(substring == "dmf"){
        for(int i = 0; i < size; ++i){
            std::getline(datafile, value);
            std::istringstream iss(value);
            std::string token;
            for(int j = 0; j < size; ++j){
                iss >> token;
                if( j >= start){
                    long double numValue = std::stod(value);
                    mean += numValue;
                    ++size;
                    if(valueMap.find(numValue) == valueMap.end()){
                        valueMap.insert(std::pair<long double, long>(numValue, 1));
                    }else{
                        valueMap[numValue] = valueMap[numValue] + 1;
                    }
                }
            }
            start++;
        }
    }else{
        while(!datafile.eof()){
            std::getline(datafile, value, ',');
            try{
                //std::cout << value << std::endl;
                long double numValue = std::stod(value);
                mean += numValue;
                ++size;
                if(valueMap.find(numValue) == valueMap.end()){
                    valueMap.insert(std::pair<long double, long>(numValue, 1));
                }else{
                    valueMap[numValue] = valueMap[numValue] + 1;
                }
            }catch(...){
                std::cout << "end reached" << std::endl;
            }
        }
    }
    mean = mean / (long double) size;
    std::cout << "size is " << size << std::endl;
    long double min = valueOfIndex(valueMap, 0);
    long double max = valueOfIndex(valueMap, size);
    long double median;
    long double firstQuartile;
    long double thirdQuartile;
    if(size % 2 == 0){
        median = (valueOfIndex(valueMap, size/2) + valueOfIndex(valueMap, size/2 + 1)) / 2.0;
        if(size % 4 == 0){
            firstQuartile = (valueOfIndex(valueMap, size/4) + valueOfIndex(valueMap, size/4 + 1)) / 2.0;
            thirdQuartile = (valueOfIndex(valueMap, 3 * size/4) + valueOfIndex(valueMap, 3 * size/4 + 1)) / 2.0;
        }else{
            firstQuartile = valueOfIndex(valueMap, size/4 + 1);
            thirdQuartile = valueOfIndex(valueMap, 3 * size/4 + 1);
        }
    }else{
        median = valueOfIndex(valueMap, size/2 + 1);
        if(size % 4 == 0){
            firstQuartile = (valueOfIndex(valueMap, size/4) + valueOfIndex(valueMap, size/4 + 1)) / 2.0;
            thirdQuartile = (valueOfIndex(valueMap, 3 * size/4) + valueOfIndex(valueMap, 3 * size/4 + 1)) / 2.0;
        }else{
            firstQuartile = valueOfIndex(valueMap, size/4 + 1);
            thirdQuartile = valueOfIndex(valueMap, 3 * size/4 + 1);
        }
    }
    std::cout << "The minimum is: " << min << std::endl;
    std::cout << "The first quartile is: " << firstQuartile << std::endl;
    std::cout << "The median is: " << median << std::endl;
    std::cout << "The third quartile is: " << thirdQuartile << std::endl;
    std::cout << "The maximum is: " << max << std::endl;
    std::cout << "The mean is: " << mean << std::endl;
    std::string refinedFilePath = filePath;
    refinedFilePath.replace(refinedFilePath.size()-4, 11, "refined.csv");
    std::string reviewFilePath = filePath;
    reviewFilePath.replace(reviewFilePath.size()-4, 10, "review.txt");
    std::string valueFilePath = filePath;
    valueFilePath.replace(valueFilePath.size()-4, 9, "value.txt");
    std::ofstream refined(refinedFilePath);
    refined << min << "," << firstQuartile << "," << median << "," << thirdQuartile << ","  << max;
    refined.close();
    std::ofstream review(reviewFilePath);
    review << "The minimum is: " << min << std::endl;
    review << "The first quartile is: " << firstQuartile << std::endl;
    review << "The median is: " << median << std::endl;
    review << "The third quartile is: " << thirdQuartile << std::endl;
    review << "The maximum is: " << max << std::endl;
    review << "The mean is: " << mean << std::endl;
    review << "The results are: " << std::endl;
    for(std::map<long double, long>::iterator it = valueMap.begin(); it != valueMap.end(); ++it){
        review << "We have " << it->second << " instances of the value " << it->first << std::endl;
    }
    review.close();
    std::ofstream values(valueFilePath);
    for(std::map<long double, long>::iterator it = valueMap.begin(); it != valueMap.end(); ++it){
        values << it->first << "," << it->second << std::endl;
    }
    return 0;
}

double valueOfIndex(const std::map<long double, long> & map, long index){
    long reachedIndex = 0;
    for( std::map<long double, long>::const_iterator it = map.begin(); it != map.end(); ++it){
        reachedIndex += it->second;
        if( reachedIndex >= index){
            return it->first;
        }
    }
    return -1.0;
}
