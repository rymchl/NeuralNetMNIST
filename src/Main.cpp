
#include "mnist_reader_less.hpp"

#include "Network.h"

int main() {
    srand(time(0));

    int TOTAL_EPOCHS = 10000;

    std::cout << std::endl << "LOADING DATA..." << std::endl << std::endl;

    auto dataset = mnist::read_dataset();

    std::vector<Image> training_images, testing_images;
    parse_as_images(training_images,
        testing_images,
        dataset.training_images,
        dataset.test_images,
        dataset.training_labels,
        dataset.test_labels);



    std::cout << "FINISHED LOADING DATA." << std::endl << std::endl;

    std::cout << "INITIALIZING NETWORK..." << std::endl << std::endl;

    Network network;

    //network.test(testing_images);
    //return 0;

    std::cout << "FINISHED INITIALIZING NETWORK." << std::endl << std::endl;

    std::cout << "TRAINING..." << std::endl << std::endl;

    int epochs = TOTAL_EPOCHS;

    while(epochs > 0){
        printf("%d/%d\n", TOTAL_EPOCHS - epochs + 1, TOTAL_EPOCHS);
        network.train(training_images, testing_images, 10000);
        epochs--;
    }
    
    network.test(training_images);

    network.test(testing_images);

    for (Image image : sample(10, testing_images)) {
        network.print_classification(image, network.classify(image));
    }

    network.save_weights_biases();

    return 0;
}