#include "LoadMNIST.h"
#include "Network.h"

#define TRAINING_IMAGE_SET_PATH "MNIST/train-images.idx3-ubyte"
#define TRAINING_LABELS_SET_PATH "MNIST/train-labels.idx1-ubyte"
#define TEST_IMAGE_SET_PATH "MNIST/t10k-images.idx3-ubyte"
#define TEST_LABELS_SET_PATH "MNIST/t10k-labels.idx1-ubyte"

int main(){
    
    srand(time(0));

    std::vector<Image> training_images, testing_images;
    std::vector<unsigned int> training_labels, testing_labels;

    std::cout << std::endl << "LOADING DATA..." << std::endl << std::endl;

    testing_labels = read_mnist_labels(TEST_LABELS_SET_PATH);
    testing_images = read_mnist_images(TEST_IMAGE_SET_PATH, testing_labels);

    
    //training_labels = read_mnist_labels(TRAINING_LABELS_SET_PATH);
    //training_images = read_mnist_images(TRAINING_IMAGE_SET_PATH, training_labels);
    
    std::cout << "FINISHED LOADING DATA." << std::endl;

    Network network;

    network.print_structure();

    network.classify(testing_images[0]);

    return 0;
}