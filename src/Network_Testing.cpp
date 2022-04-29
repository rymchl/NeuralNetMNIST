#include "Network.h"

//Tests the network against an input testing set.
//This will print to stdout the accuracy of the network:
//  Accuracy = Number of correctly classified images / Total number of images
//And the average cost of a classification over the testing set.
void Network::test(std::vector<Image> testing_set) {
    double classification_rate = 0;
    double total_cost = 0;
    double num_correct = 0;
    double cost_out = 0;

    for (Image image : testing_set) {
        std::vector<float> classification = classify(image);
        if (max_index(classification) == image.LABEL) {
            num_correct++;
        }
        cost_out += cost(image.LABEL, classification);
    }
    classification_rate = (float)num_correct / testing_set.size();
    total_cost = cost_out / testing_set.size();

    std::cout << 100 * classification_rate  << "% ACCURACY " << 
        "|" << num_correct << "\\" << testing_set.size() << "|" << std::endl;

    std::cout << "COST: " << total_cost << std::endl;
    
}


std::vector<float> Network::classify(Image image) {

    std::vector<std::vector<float> > activations;
    std::vector<std::vector<float> > z_values;

    activations.resize(layers.size());
    z_values.resize(layers.size());
    for (int i = 0; i < layers.size(); i++) {
        activations[i].resize(layers[i].biases.size());
        z_values[i].resize(layers[i].biases.size());
    }

    for (unsigned int y = 0; y < image.HEIGHT; y++) {
        for (unsigned int x = 0; x < image.WIDTH; x++) {
            activations[0][y * image.HEIGHT + x] = image.get(x, y) / 255.0f;
        }
    }

    //calculate z-values and activations for all nodes
    for (unsigned int id = 1; id < layers.size(); id++) {
        z_values[id] = layers[id].calc_z(activations[id - 1]);
        activations[id] = apply_sigmoid(z_values[id]);
    }

    return activations[NUM_LAYERS - 1];
}

void Network::print_classification(Image image, std::vector<float> classification) {
    image.print();
    int selection = 0;
    for (int index = 0; index < classification.size(); index++) {
        if (classification[index] > classification[selection]) selection = index;
        printf("+ <%d> : %.2f | <%d> : %.2f \n", index, classification[index], index, (index == image.LABEL) ? 1.0f : 0.0f);
    }
    
    for (unsigned int i = 0; i <= (image.WIDTH / 4); i++) printf("+ + ");
    std::cout << std::endl;

    std::cout << "+ CLASSIFIED AS A " + std::to_string(selection) << std::endl;
    
    for (unsigned int i = 0; i <= (image.WIDTH / 4); i++) printf("+ + ");
    std::cout << std::endl;

    std::cout << "+ COST: " << cost(image.LABEL, classification) << std::endl;
    
    for (unsigned int i = 0; i <= (image.WIDTH / 2); i++) printf("+ + ");
    std::cout << std::endl << std::endl;
}

float Network::cost(int label, std::vector<float> classification) {
    float cost = 0;
    std::vector<float> y;
    for (int i = 0; i < 10; i++) y.push_back((i == label) ? 1 : 0);
    for (int i = 0; i < classification.size(); i++)
        cost += pow(classification[i] - y[i], 2);

    return cost;
}