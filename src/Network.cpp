#include"Network.h"


Network::Network(){
    //Input nodes:
    unsigned int num_input_pixel_values = 784;
    unsigned int num_output_values = 10;

    std::vector<unsigned int> network_structure = {num_input_pixel_values, 16, 16, num_output_values};

    unsigned int num_layers = network_structure.size();
    
    //input layer
    layers.push_back(Layer(0,network_structure[0],0));
    
    for(int i = 1; i < num_layers; i++){
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


void Network::train(std::vector<Image> training_set, int iterations){

    if(iterations <= 0) return;
    else std::cout << "TRAINING 1 OF " << iterations << " ITERATIONS..." << std::endl;

    std::clock_t start = clock();

    //layer//weight[to][from]
    std::vector<std::vector<std::vector<float> > > weight_changes;
    weight_changes.resize(layers.size());
    for(int i = 0; i < weight_changes.size(); i++){
        weight_changes[i].resize(layers[i].weights.size());
        for(int j = 0; j < weight_changes[i].size(); j++){
            weight_changes[i][j].resize(layers[i].weights[j].size());
        }
    }

    //layer//bias[node]
    std::vector<std::vector<float > > bias_changes;
    bias_changes.resize(layers.size());
    for(int i = 0; i < bias_changes.size(); i++) bias_changes[i].resize(layers[i].biases.size());


    //Learn on all images in training set;
    for(Image image : training_set){

        std::vector<std::vector<float> > activations;
        std::vector<std::vector<float> > z_values; 

        activations.resize(layers.size());
        z_values.resize(layers.size());
        for(int i = 0; i < layers.size(); i++){
            activations[i].resize(layers[i].biases.size());
            z_values[i].resize(layers[i].biases.size());
        } 

        for(int y = 0; y < image.HEIGHT; y++){
            for(int x = 0; x < image.WIDTH; x++){
                activations[0][y * image.HEIGHT + x] = image.get(x,y) / 255.0f;
            }
        }
        
        //calculate z-values and activations for all nodes
        for(int id = 1; id < layers.size(); id++){
            z_values[id] = layers[id].calc_z(activations[id-1]);
            activations[id] = apply_sigmoid(z_values[id]);
        }

        //init y-vec
        std::vector<float> y;
        for(int i = 0; i < 10; i++) y.push_back( (i == image.LABEL) ? 1 : 0 ); 

        //Start at final layer, stop upon reaching input layer
        for(int layer_id = layers.size() - 1; layer_id > 0; layer_id--){
            Layer* layer = &layers[layer_id];

            //calculate nudge for weights and biases of this layer
            for(int j = 0; j < layer->weights.size(); j++){
                for(int k = 0; k < layer->weights[j].size(); k++){

                    float d1 = activations[layer_id - 1][k];
                    float d2 = der_sigmoid(z_values[layer_id][j]);
                    float d3 = 2 * (activations[layer_id][j] - y[j]);

                    float d_biases = d2 * d3;
                    float d_weight = d1 * d_biases;

                    bias_changes[layer_id][j] -= d_biases;
                    weight_changes[layer_id][j][k] -= d_weight;

                }
            }
        }
    }

    float conv_param = 0.9f;
    int num_examples = (int)training_set.size();

    float C = conv_param / num_examples;

    for(int layer_id = 1; layer_id < layers.size(); layer_id++){
            for(int j = 0; j < layers[layer_id].weights.size(); j++){
                layers[layer_id].biases[j] += C * bias_changes[layer_id][j];
                for(int k = 0; k < layers[layer_id].weights[j].size(); k++){
                    layers[layer_id].weights[j][k] += C * weight_changes[layer_id][j][k];
                }
            }
    }


    std::cout << "TOOK " << ( std::clock() - start ) / (double) CLOCKS_PER_SEC << " SECONDS." << std::endl;
    std::cout << "NETWORK COST : " << total_cost(training_set) << std::endl << std::endl;


    train(training_set, iterations - 1);
}