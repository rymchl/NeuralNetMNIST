#include"Network.h"


Network::Network(){
    //Input nodes:
    unsigned int num_input_pixel_values = 784;
    unsigned int num_output_values = 10;

    std::vector<unsigned int> network_structure = {num_input_pixel_values, 16, 16, num_output_values};

    NUM_LAYERS = network_structure.size();
    
    //input layer
    layers.push_back(Layer(0,network_structure[0],0));
    
    for(int i = 1; i < NUM_LAYERS; i++){
        layers.push_back(Layer(i,network_structure[i],network_structure[i-1]));
    }    
}

std::vector<float> Network::classify(Image image){
    
    std::vector<float> activations;

    for(int y = 0; y < image.HEIGHT; y++){
        for(int x = 0; x < image.WIDTH; x++){
            activations.push_back(float(image.get(x,y)) / 255.0f);
        }
    }
    
    for(int id = 0; id < layers.size(); id++){
        activations = layers[id].activate(activations);
    }
    
    

    return activations;
}

void Network::print_classification(Image image, std::vector<float> classification){
    image.print();
    
    int selection = 0;
    for(int index = 0; index < classification.size(); index++){
        if(classification[index] > classification[selection]) selection = index;
        printf("<%d> : %.2f | <%d> : %.2f \n", index, classification[index], index, (index == image.LABEL) ? 1.0f : 0.0f);
    }
    
    std::cout << std::endl << "I THINK ITS A " + std::to_string(selection) << std::endl;
    std::cout << "COST: " << cost(image.LABEL, classification) << std::endl << std::endl;
}

float Network::cost(int label, std::vector<float> classification){
        float cost = 0;
        for(int i = 0; i < classification.size(); i++) 
            cost += (i == label) ? pow(classification[i]-1,2) : pow(classification[i],2);
            
        return cost;
}

float Network::total_cost(std::vector<Image> image_set){
    float cost_out = 0;
    for(Image image : image_set){
        cost_out += cost(image.LABEL, classify(image));
    }
    return cost_out / image_set.size();
}


float Network::get_classification_rate(std::vector<Image> image_set) {
    int num_correct = 0;
    for (Image image : image_set) {
        std::vector<float> classification = classify(image);
        if (max_index(classification) == image.LABEL) num_correct++;
    }
    return (float)num_correct / image_set.size();
}

void Network::save_weights_biases() {
    std::ofstream weights_out("weights.txt");
    std::ofstream biases_out("biases.txt");

    for (int l = 0; l < layers.size(); l++) {
        for (int j = 0; j < layers[l].SIZE; j++) {
            for (int k = 0; k < layers[l].weights[j].size(); k++) {
                weights_out << std::to_string(l) + ' ' + std::to_string(j) + ' ' + std::to_string(k) + ' ' + std::to_string(layers[l].weights[j][k]) << std::endl;
            }
            biases_out << std::to_string(l) + ' ' + std::to_string(j) + ' ' + std::to_string(layers[l].biases[j]) << std::endl;
        }
    }

    weights_out.close();
    biases_out.close();
}