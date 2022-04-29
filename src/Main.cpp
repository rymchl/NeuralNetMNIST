
#include "mnist_reader_less.hpp"
#include "Network.h"

int main(int argc, char** argv) {
    srand(time(0));

    //MODIFY THESE TO ALTER PROGRAM
    //------------------------------------------------------------------------------------------------------------------

    std::string filepath_in = "saved_networks/network_85P.nn"; //Path to an input network (Leave blank if you wish to create a new one)
    std::string filepath_out = ""; //Path to save the finished network (Leave blank if you do not wish to save)
    unsigned int num_epochs = 0; //Number of training epochs
    float convergance_parameter = 0.5f; //Multiplier to the gradient (Usually between 0,1)
    unsigned int batch_size = 5000; //Number of images to consider from the training set 
    TRAINING_MODE mode = TRAINING_MODE::COST_ANALYSIS; //COST_ANALYSIS will calculate and display the cost of the network after each epoch.


    Network network("saved_networks/network_85P.nn"); //THIS WILL CREATE A NETWORK BASED ON INPUT FILE
    //Network network; //THIS WILL CREATE A RANDOMIZED NETWORK
    //Network network(std::vector<unsigned int>{784, 16, 16, 10}); //THIS WILL CREATE A NETWORK WITH THE INPUT STRUCTURE

    //------------------------------------------------------------------------------------------------------------------

    std::cout << "LOADING DATA..." << std::endl << std::endl;

    auto dataset = mnist::read_dataset();
    std::vector<Image> training_images, testing_images;
    parse_as_images(training_images,
        testing_images,
        dataset.training_images,
        dataset.test_images,
        dataset.training_labels,
        dataset.test_labels);

    std::cout << "INITIALIZING NETWORK..." << std::endl << std::endl;

   

    std::cout << "TRAINING..." << std::endl << std::endl;

    int epochs = num_epochs;

    while(epochs > 0){
        printf("%d/%d\n", num_epochs - epochs + 1, num_epochs);
        if (mode == TRAINING_MODE::COST_ANALYSIS) network.test(testing_images);
        network.train(training_images, batch_size, convergance_parameter);  
        epochs--;
    }

    
    network.save_network_to_file(filepath_out);

    std::cout << "5 Random test cases:" << std::endl << std::endl;

    for (Image image : sample(5, testing_images)) {
        network.print_classification(image, network.classify(image));
    }

    network.test(testing_images);


    return 0;
}