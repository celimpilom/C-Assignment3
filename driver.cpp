#include "PGMimageProcessor.h"

int main(int argc, char * argv[]){

    std::cout << std::endl; // nice printing

    // control variables
    int min = 3; // minimum component size
    int max = 0; // no need to set a sensible default value since it's only used with -s option
    int threshold = 128; // default threshold

    // flags
    bool print_components = false;
    bool write_output = false;
    bool filter = false; // if max is set to a value
    
    // files
    std::string in_filepath = "input/chess-thresh.pgm";
    std::string out_filepath = "output/output.pgm";

    // check if at least the input file is specified
    if ( argc < 3){
        std::cout << "Invalid argument" << std::endl;
        return 0;
    }

    int index = 1;

    while (index < argc - 1){
        // valid component sizes
        if (std::strcmp(argv[index], "-s") == 0){
            min = std::stoi(argv[++index]);
            max = std::stoi(argv[++index]);
            filter = true;
            ++index;
        }

        // set threshold
        if (std::strcmp(argv[index], "-t") == 0){
            threshold = std::stoi(argv[++index]);
            ++index;
        }

        // print components information or not
        if (std::strcmp(argv[index], "-p") == 0){
            print_components = true;
            ++index;
        }

        // output file name
        if (std::strcmp(argv[index], "-w") == 0){
            out_filepath = argv[++index];
            write_output = true;
            ++index;
        }
    }
    // finally declare the input file
    in_filepath = argv[index];

    // Create the object
    PGMimageProcessor processor(in_filepath);

    if (!processor.readImage()){
        std::cout << "Could not read image" << std::endl;
    }

    std::cout << "Image Read succefully\n" << std::endl;

    int found = processor.extractComponents(threshold, min);

    std::cout << "Found " << processor.getComponentCount() << " components.\n" << std::endl;

    // Optional parameters
    if (filter) found = processor.filterComponentsBySize(min, max);
    if (print_components) processor.display();
    std::cout << std::endl;
    if (write_output){
        processor.writeComponents(out_filepath);
        std::cout << "Saved file: " << out_filepath << std::endl;
    } 

    return 0;
}