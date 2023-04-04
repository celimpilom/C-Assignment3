#include "PGMimageProcessor.h"

// constructor
PGMimageProcessor::PGMimageProcessor(std::string name = "input/test1.pgm"):
    filepath(name) {}
// constructor
PGMimageProcessor::PGMimageProcessor(std::string name = "input/test1.pgm"):
    filepath(name) {}
// copy
PGMimageProcessor::PGMimageProcessor(const PGMimageProcessor & other):
    filepath(other.filepath), input(other.input), height(other.height), width(other.width),
    components(other.components), component_count(other.component_count), largest_size(other.largest_size),
    smallest_size(other.smallest_size) {

}
// move
PGMimageProcessor::PGMimageProcessor(PGMimageProcessor && other):
    filepath(other.filepath), input(other.input), height(other.height), width(other.width),
    component_count(other.component_count), largest_size(other.largest_size),
    smallest_size(other.smallest_size) 
{
    components = std::move(other.components);
}
//destructor
PGMimageProcessor::~PGMimageProcessor(){
    delete [] input;
}

// --------------------------------------------------------------
// Extract Components
int PGMimageProcessor::extractComponents(unsigned char threshold, int minValidSize){

    int id = 0;
    int found = 0;

    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j){

            if (input[i][j] >= threshold){

                std::shared_ptr<ConnectedComponent> comp(new ConnectedComponent(id++) ); // increment id
                std::queue< std::pair<int,int> > que;
                que.push( std::make_pair(i,j) );

                while(!que.empty()){
                    std::pair<int,int> pixel (que.front());
                    que.pop();
                    if (input[pixel.first][pixel.second] >= threshold){
                    comp->add(pixel);
                    input[pixel.first][pixel.second] = 0;
                    processConnectedComponets(que, pixel) ;
                    }

                }

                // check if component size meets threshold
                if (comp->getSize() >= minValidSize ){
                    // update smallest and largest size
                    if (comp->getSize() > largest_size) largest_size = comp->getSize();
                    if (comp->getSize() < smallest_size) smallest_size = comp->getSize();

                    components.push_back(std::move(comp));
                    ++found;
                    
                 }
                
            }

        }
    }
    component_count = found;
    return components.size();
}

void PGMimageProcessor::processConnectedComponets(std::queue<std::pair<int, int>> &que, std::pair<int, int> coordinate)
 {   
    if (coordinate.first > 0) {
        que.push(std::make_pair( coordinate.first-1, coordinate.second));
    }
    if (coordinate.second < width - 1){
        que.push(std::make_pair( coordinate.first, coordinate.second + 1));
    }
    if (coordinate.first < height - 1){
        que.push(std::make_pair( coordinate.first + 1, coordinate.second));
    }
    if (coordinate.second > 0){
        que.push(std::make_pair( coordinate.first, coordinate.second - 1));
    }
}

//-------------------------------------------------------------
// Filtering

// filter by component size
int PGMimageProcessor::filterComponentsBySize(int minSize, int maxSize){

    smallest_size = height * width; 
    largest_size = 0;

    for (std::vector< std::shared_ptr<ConnectedComponent> >::iterator it = components.begin(); it < components.end(); ){
        if ((*it)->getSize() < minSize || (*it)->getSize() > maxSize){
            components.erase(it);
            --component_count; // update component count
        } else {
            // recalculate smallest and largest sizes
            if ((*it)->getSize() > largest_size) this->largest_size = (*it)->getSize();
            if ((*it)->getSize() < smallest_size) smallest_size = (*it)->getSize();

            ++it; // increment iterator if element not removed
        }
    }

    return component_count; // return new count
}

//--------------------------------------------------------------
// print component data

void PGMimageProcessor::printComponentData(const ConnectedComponent & theComponent) const{
    theComponent.display();
}

void PGMimageProcessor::display() const{
    std::cout << "There are " << component_count << " components." << std::endl;
    for (int i = 0; i < components.size(); ++i){
        PGMimageProcessor::printComponentData(*(components[i]));
    }
    std::cout << "smallest size: " << getSmallestSize() << std::endl;
    std::cout << "largest size: " << getLargestSize() << std::endl;
}

//--------------------------------------------------------------
// Get functions

int PGMimageProcessor::getComponentCount(void) const{
    return component_count;
}

int PGMimageProcessor::getLargestSize(void) const{
    return largest_size;
}

int PGMimageProcessor::getSmallestSize(void) const{
    return smallest_size;
}

//---------------------------------------------------------------
// Write out components
bool PGMimageProcessor::writeComponents(const std::string & outFilename){

    int output_size = height*width;

    std::ofstream ofs(outFilename, std::ios::out | std::ios::binary);
    ofs << "P5" << std::endl;
    ofs << width << " " << height << std::endl;
    ofs << 255 << std::endl;

    unsigned char * outImage = new unsigned char [width * height];

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; ++j){
            outImage[i * width + j] = 0;
        }
    }

    // write components
    for (std::vector< std::shared_ptr<ConnectedComponent> >::iterator it = components.begin(); it < components.end(); ++it){
        for (std::pair<int, int> pixel: (*it)->pixels){
            outImage[ pixel.first * width + pixel.second] = 255;
        }
    }


    ofs.write(reinterpret_cast<char*>(outImage), width*height);

    delete [] outImage;

    return true;
}

// copy
PGMimageProcessor::PGMimageProcessor(const PGMimageProcessor & other):
    filepath(other.filepath), input(other.input), height(other.height), width(other.width),
    components(other.components), component_count(other.component_count), largest_size(other.largest_size),
    smallest_size(other.smallest_size) {

}
// move
PGMimageProcessor::PGMimageProcessor(PGMimageProcessor && other):
    filepath(other.filepath), input(other.input), height(other.height), width(other.width),
    component_count(other.component_count), largest_size(other.largest_size),
    smallest_size(other.smallest_size) 
{
    components = std::move(other.components);
}
//destructor
PGMimageProcessor::~PGMimageProcessor(){
    delete [] input;
}

// --------------------------------------------------------------
// Extract Components
int PGMimageProcessor::extractComponents(unsigned char threshold, int minValidSize){

    int id = 0;
    int found = 0;

    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j){

            if (input[i][j] >= threshold){

                std::shared_ptr<ConnectedComponent> comp(new ConnectedComponent(id++) ); // increment id
                std::queue< std::pair<int,int> > n_queue;
                n_queue.push( std::make_pair(i,j) );

                while(!n_queue.empty()){
                    std::pair<int,int> pixel (n_queue.front());
                    n_queue.pop();

                    if (input[pixel.first][pixel.second] >= threshold){
                        comp->add(pixel);
                        input[pixel.first][pixel.second] = 0;

                        if (pixel.first > 0) {
                            n_queue.push(std::make_pair( pixel.first-1, pixel.second));
                        }
                        if (pixel.second < width - 1){
                            n_queue.push(std::make_pair( pixel.first, pixel.second + 1));
                        }
                        if (pixel.first < height - 1){
                            n_queue.push(std::make_pair( pixel.first + 1, pixel.second));
                        }
                        if (pixel.second > 0){
                            n_queue.push(std::make_pair( pixel.first, pixel.second - 1));
                        }

                    }

                }

                // check if component size meets threshold
                if (comp->getSize() >= minValidSize ){
                    // update smallest and largest size
                    if (comp->getSize() > largest_size) largest_size = comp->getSize();
                    if (comp->getSize() < smallest_size) smallest_size = comp->getSize();

                    components.push_back(std::move(comp));
                    ++found;
                    
                 }
                
            }

        }
    }
    component_count = found;
    return found;
}

//-------------------------------------------------------------
// Filtering

// filter by component size
int PGMimageProcessor::filterComponentsBySize(int minSize, int maxSize){

    smallest_size = height * width; 
    largest_size = 0;

    for (std::vector< std::shared_ptr<ConnectedComponent> >::iterator it = components.begin(); it < components.end(); ){
        if ((*it)->getSize() < minSize || (*it)->getSize() > maxSize){
            components.erase(it);
            --component_count; // update component count
        } else {
            // recalculate smallest and largest sizes
            if ((*it)->getSize() > largest_size) this->largest_size = (*it)->getSize();
            if ((*it)->getSize() < smallest_size) smallest_size = (*it)->getSize();

            ++it; // increment iterator if element not removed
        }
    }

    return component_count; // return new count
}

//--------------------------------------------------------------
// print component data

void PGMimageProcessor::printComponentData(const ConnectedComponent & theComponent) const{
    theComponent.display();
}

void PGMimageProcessor::display() const{
    std::cout << "There are " << component_count << " components." << std::endl;
    for (int i = 0; i < components.size(); ++i){
        PGMimageProcessor::printComponentData(*(components[i]));
    }
    std::cout << "smallest size: " << getSmallestSize() << std::endl;
    std::cout << "largest size: " << getLargestSize() << std::endl;
}

//--------------------------------------------------------------
// Get functions

int PGMimageProcessor::getComponentCount(void) const{
    return component_count;
}

int PGMimageProcessor::getLargestSize(void) const{
    return largest_size;
}

int PGMimageProcessor::getSmallestSize(void) const{
    return smallest_size;
}

//---------------------------------------------------------------
// Write out components
bool PGMimageProcessor::writeComponents(const std::string & outFilename){

    int output_size = height*width;

    std::ofstream ofs(outFilename, std::ios::out | std::ios::binary);
    ofs << "P5" << std::endl;
    ofs << width << " " << height << std::endl;
    ofs << 255 << std::endl;

    unsigned char * outImage = new unsigned char [width * height];

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; ++j){
            outImage[i * width + j] = 0;
        }
    }

    // write components
    for (std::vector< std::shared_ptr<ConnectedComponent> >::iterator it = components.begin(); it < components.end(); ++it){
        for (std::pair<int, int> pixel: (*it)->pixels){
            outImage[ pixel.first * width + pixel.second] = 255;
        }
    }


    ofs.write(reinterpret_cast<char*>(outImage), width*height);

    delete [] outImage;

    return true;
}

// --------------------------------------------------------------
// Read image into a 2D array
void PGMimageProcessor::readImage(std::string file)
{
		std::fstream fileName ;
		fileName.open(file) ;
		std::string line ;
		
		while(true)
		{
			getline(fileName, line) ;
		
			if (line.compare("P5") == 0)
            {continue ;}
			if(line.substr(0, 1).compare("#") == 0 )
            {continue ;}
		
		else
		{
				width = std::stoi(line.substr(0, line.find(" "))) ;
				height = std::stoi(line.substr(line.find(" ") + 1, sizeof(line)-line.find(" ")+1)) ;
				getline(fileName, line) ;
		        input  = new unsigned char*[height] ;
			    for (int i = 0 ; i < height; i++)
					{
						input[i] = new unsigned char[width] ;
					}
				int counter = 0 ;
				while(fileName.good())
				{
						fileName.read(reinterpret_cast<char*>(input[counter]), width) ;
						counter ++ ;
				}
				
				break ;
				fileName.close() ;
		}
	}
	
}