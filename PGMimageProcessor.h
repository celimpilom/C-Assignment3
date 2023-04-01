#ifndef PGMimageProcessor_H
#define PGMimageProcessor_H

#include "ConnectedComponent.h"
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

class PGMimageProcessor {

    private:
    // container to store components
    std::vector< std::shared_ptr<ConnectedComponent> > components; 
    //input image
    unsigned char ** inputImage; 
    std::string filepath; 
    int height, width; 

    int component_count = 0;
    int largest_size = 0;
    int smallest_size;

    public:
   
    PGMimageProcessor();
    PGMimageProcessor(std::string nm);
    PGMimageProcessor(const PGMimageProcessor & other); 
    PGMimageProcessor(PGMimageProcessor && other); 
    ~PGMimageProcessor(); 

    // methods
    bool readImage();
    int extractComponents(unsigned char threshold, int minValidSize);
    int filterComponentsBySize(int minSize, int maxSize);
    bool writeComponents(const std::string & outFilename);
    int getComponentCount(void) const;
    int getLargestSize(void) const;
    int getSmallestSize(void) const;
    void printComponentData(const ConnectedComponent & theComponent) const;
    void display() const;



};



#endif