#ifndef PGMimageProcessor_H
#define PGMimageProcessor_H

#include "ConnectedComponents.h"
#include <queue>
#include <string>
#include <utility>
#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <math.h>
#include <cstring>
#include <cstdlib>


// class defination

struct Pixel {
    int x;
    int y;
};

class PGMimageProcessor {

    private:
    // container to store components
    std::vector< std::shared_ptr<ConnectedComponent> > components; 
    //input image
    unsigned char ** input; 
    std::string filepath; 
    int height, width; 

    int component_count = 0;
    int largest_size = 0;
    int smallest_size;

    public:
    PGMimageProcessor();
    PGMimageProcessor(std::string name);
    PGMimageProcessor(const PGMimageProcessor & other); 
    PGMimageProcessor(PGMimageProcessor && other); 
    ~PGMimageProcessor(); 

    // methods
    int extractComponents(unsigned char threshold, int minValidSize);
    int filterComponentsBySize(int minSize, int maxSize);
    bool writeComponents(const std::string & outFilename);
    void readImage(std::string file);
    int getComponentCount(void) const;
    int getLargestSize(void) const;
    int getSmallestSize(void) const;
    void printComponentData(const ConnectedComponent & theComponent) const;
    void display() const;
    void log(std::string message);
    
    void processConnectedComponets(
        std::queue<std::pair<int, int>> &myque,
        std::pair<int, int> coordinate);




};

#endif