#include "Network.h"


void Network::test(std::vector<Image> testing_set) {

    float classification_rate = 0;
    double total_cost = 0;

    int num_correct = 0;
    double cost_out = 0;

    for (Image image : testing_set) {
        std::vector<float> classification = classify(image);
        if (max_index(classification) == image.LABEL) {
            //print_classification(image, classification);
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

void Network::print_classification(Image image, std::vector<float> classification) {
    image.print();

    int selection = 0;
    for (int index = 0; index < classification.size(); index++) {
        if (classification[index] > classification[selection]) selection = index;
        printf("<%d> : %.2f | <%d> : %.2f \n", index, classification[index], index, (index == image.LABEL) ? 1.0f : 0.0f);
    }

    std::cout << std::endl << "I THINK ITS A " + std::to_string(selection) << std::endl;
    std::cout << "COST: " << cost(image.LABEL, classification) << std::endl << std::endl;
}

float Network::cost(int label, std::vector<float> classification) {
    float cost = 0;
    std::vector<float> y;
    for (int i = 0; i < 10; i++) y.push_back((i == label) ? 1 : 0);

    for (int i = 0; i < classification.size(); i++)
        cost += pow(classification[i] - y[i], 2);

    return cost;
}