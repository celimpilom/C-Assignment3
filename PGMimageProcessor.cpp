#include "PGMimageProcessor.h"

// constructor
PGMimageProcessor::PGMimageProcessor(std::string nm = "input/chess-thresh.pgm"):
    fileName(nm) {}
// copy
PGMimageProcessor::PGMimageProcessor(const PGMimageProcessor & other):
    fileName(other.fileName), inputImage(other.inputImage), height(other.height), width(other.width),
    components(other.components), count_comp(other.count_comp), largest_size(other.largest_size),
    small_size(other.small_size) {

}
// move
PGMimageProcessor::PGMimageProcessor(PGMimageProcessor && other):
    fileName(other.fileName), inputImage(other.inputImage), height(other.height), width(other.width),
    count_comp(other.count_comp), largest_size(other.largest_size),
    small_size(other.small_size) 
{
    components = std::move(other.components);
}
//destructor
PGMimageProcessor::~PGMimageProcessor(){
    delete [] inputImage;
}

// --------------------------------------------------------------
// Extract Components
int PGMimageProcessor::extractComponents(unsigned char threshold, int minValidSize){

    int id = 0;
    int found = 0;

    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j){

            if (inputImage[i][j] >= threshold){

                std::shared_ptr<ConnectedComponent> comp(new ConnectedComponent(id++) ); // increment id
                std::queue< std::pair<int,int> > queue;
                queue.push( std::make_pair(i,j) );

                while(!queue.empty()){
                    std::pair<int,int> pixel (queue.front());
                    queue.pop();

                    if (inputImage[pixel.first][pixel.second] >= threshold){
                        comp->add(pixel);
                        inputImage[pixel.first][pixel.second] = 0;

                        if (pixel.first > 0) {
                            queue.push(std::make_pair( pixel.first-1, pixel.second));
                        }
                        if (pixel.second < width - 1){
                            queue.push(std::make_pair( pixel.first, pixel.second + 1));
                        }
                        if (pixel.first < height - 1){
                            queue.push(std::make_pair( pixel.first + 1, pixel.second));
                        }
                        if (pixel.second > 0){
                            queue.push(std::make_pair( pixel.first, pixel.second - 1));
                        }

                    }

                }

                // check if component size meets threshold
                if (comp->getSize() >= minValidSize ){
                    // update smallest and largest size
                    if (comp->getSize() > largest_size) largest_size = comp->getSize();
                    if (comp->getSize() < small_size) small_size = comp->getSize();

                    components.push_back(std::move(comp));
                    ++found;
                    
                 }
                
            }

        }
    }
    count_comp = found;
    return found;
}

// filter by component size
int PGMimageProcessor::filterComponentsBySize(int minSize, int maxSize){

    small_size = height * width; 
    largest_size = 0;

    for (std::vector< std::shared_ptr<ConnectedComponent> >::iterator it = components.begin(); it < components.end(); ){
        if ((*it)->getSize() < minSize || (*it)->getSize() > maxSize){
            components.erase(it);
            --count_comp; // update component count
        } else {
            // recalculate smallest and largest sizes
            if ((*it)->getSize() > largest_size) this->largest_size = (*it)->getSize();
            if ((*it)->getSize() < small_size) small_size = (*it)->getSize();

            ++it; // increment iterator if element not removed
        }
    }

    return count_comp; // return new count
}

// print component data

void PGMimageProcessor::printComponentData(const ConnectedComponent & theComponent) const{
    theComponent.display();
}

void PGMimageProcessor::display() const{
    std::cout << "There are " << count_comp << " components." << std::endl;
    for (int i = 0; i < components.size(); ++i){
        PGMimageProcessor::printComponentData(*(components[i]));
    }
    std::cout << "smallest size: " << getSmallestSize() << std::endl;
    std::cout << "largest size: " << getLargestSize() << std::endl;
}


// Get functions

int PGMimageProcessor::getComponentCount(void) const{
    return count_comp;
}

int PGMimageProcessor::getLargestSize(void) const{
    return largest_size;
}

int PGMimageProcessor::getSmallestSize(void) const{
    return small_size;
}


// Write out components
bool PGMimageProcessor::writeComponents(const std::string & outFilename){

    int output_size = height*width;

    std::ofstream ofs(outFilename, std::ios::out | std::ios::binary);
    ofs << "P5" << std::endl;
    ofs << width << " " << height << std::endl;
    ofs << 255 << std::endl;

    unsigned char ** outputImg = new unsigned char *[height];

    for (int i = 0; i < height; i++){
        outputImg[i]= new unsigned char[width];
        for (int j = 0; j < width; ++j){
            outputImg[i][j] = 0;
        }
    }

    // write components
    for (std::vector< std::shared_ptr<ConnectedComponent> >::iterator 
    it = components.begin(); it < components.end(); ++it){
        for (std::pair<int, int> pixel: (*it)->pixels){
            outputImg[pixel.first][pixel.second]=225;
        }
    }


    ofs.write(reinterpret_cast<char*>(outputImg), width*height);

    for (int i = 0; i < width; i++)
    {
        delete [] outputImg[i];
    }
    
    delete [] outputImg;

    return true;
}


// Read image into a 2D array
bool PGMimageProcessor::readImage(){
    int rows, cols;
    std::string header;

    // read image
    std::ifstream ifs(fileName, std::ios::in | std::ios::binary);

    if(!ifs){
        std::cout << "Cannot read image : " << fileName << std::endl;
        return false;
    }
    log("reading the image");
    std::cout << fileName << std::endl;

    std::getline(ifs, header);
    std::getline(ifs, header);

    while(header[0] == '#'){
        std::getline(ifs, header);
    }

    cols = std::stoi( header.substr( 0, header.find(" ") ) );
    rows = std::stoi( header.substr( header.find(" ") + 1 ) );

    height = rows;
    width = cols;

    small_size = rows * cols; // set smallest size to the image size

    std::getline(ifs, header); 

    unsigned char *data = new unsigned char[cols * rows];

    // read temporary as char
    char * temp = new char [cols * rows];
    ifs.read(temp, cols*rows);
    data = reinterpret_cast<unsigned char *>(temp);
  
    inputImage = new unsigned char * [rows];
    for (int i = 0; i < rows; ++i){
        inputImage[i] = new unsigned char [cols];
        for (int j = 0; j < cols; ++j){
            inputImage[i][j] = data[i*cols + j];
        }
    }

    delete [] data; // delete data image

    return true;
}

void PGMimageProcessor::log(std::string message){
    std::cout << message << std::endl;
 }