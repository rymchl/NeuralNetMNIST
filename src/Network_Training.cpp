#include "Network.h"

//Trains the neural net 
void Network::train(std::vector<Image> training_set, int batch_size, float conv_param) {
       
    std::clock_t start = clock();
    //layer//weight[to][from]
    std::vector<std::vector<std::vector<float> > > weight_changes;

    //layer//bias[node]
    std::vector<std::vector<float > > bias_changes;

    //Randomly shuffle the training set contents
    training_set = sample<Image>((int)training_set.size(), training_set);
    std::vector<std::vector<Image> > batches;
    std::vector<Image>::iterator training_set_iterator = training_set.begin();
    while (training_set_iterator != training_set.end()) {
        std::vector<Image> batch;
        for (int i = 0; i < batch_size; i++) {
            batch.push_back(*training_set_iterator);
            training_set_iterator++;
            if (training_set_iterator == training_set.end()) break;
        }
        batches.push_back(batch);
    }

    int batch_count = 0;

    //Learn on all images in training set;
    for (std::vector<Image> batch : batches) {
        //Backprop for each image in the batch
        for (Image image : batch) {
            std::vector<std::vector<float> > activations;
            std::vector<std::vector<float> > z_values;
            known_dCdA_values.clear(); //Clear the known partial derivatives, as they no longer apply.

            //Setup weight changes (represents component of gradient resp to each weight)
            weight_changes.clear();
            weight_changes.resize(layers.size());
            for (int i = 0; i < weight_changes.size(); i++) {
                weight_changes[i].resize(layers[i].weights.size());
                for (int j = 0; j < weight_changes[i].size(); j++)
                    weight_changes[i][j].resize(layers[i].weights[j].size());
            }

            //Setup bias changes (represents component of gradient resp to each bias)
            bias_changes.clear();
            bias_changes.resize(layers.size());
            for (int i = 0; i < bias_changes.size(); i++) 
                bias_changes[i].resize(layers[i].biases.size());

            //Setup activation and zvalue vectors
            activations.resize(layers.size());
            z_values.resize(layers.size());
            for (int i = 0; i < layers.size(); i++) {
                activations[i].resize(layers[i].biases.size());
                z_values[i].resize(layers[i].biases.size());
            }

            //Init network input (from image)
            for (int y = 0; y < image.HEIGHT; y++) 
                for (int x = 0; x < image.WIDTH; x++) 
                    activations[0][y * image.HEIGHT + x] = image.get(x, y) / 255.0f;
            //calculate z-values and activations for all nodes
            for (unsigned int id = 1; id < layers.size(); id++) {
                z_values[id] = layers[id].calc_z(activations[id - 1]);
                activations[id] = apply_sigmoid(z_values[id]);
            }

            //init y-vec (expected/correct output layer)
            std::vector<float> y;
            for (int i = 0; i < 10; i++) {
                y.push_back((i == image.LABEL) ? 1 : 0);
            }

            //Backprop
            for (unsigned int id = layers.size() - 1; id > 0; id--) {
                for (int j = 0; j < layers[id].SIZE; j++) {
                    float dSig = der_sigmoid(z_values[id][j]);
                    float dCdA = calc_dCdA(id, j, z_values, activations, y);

                    bias_changes[id][j] += dSig * dCdA;
                    for (int k = 0; k < layers[id - 1].SIZE; k++) {
                        weight_changes[id][j][k] +=
                            activations[id - 1][k]
                            * dSig
                            * dCdA;
                    }
                }
            }
        }

        //Update weights and biases by conv_param * -Gradient
        float C = conv_param / batch_size;
        for (int layer_id = 1; layer_id < layers.size(); layer_id++) {
            for (int j = 0; j < layers[layer_id].weights.size(); j++) {
                layers[layer_id].biases[j] -= C * bias_changes[layer_id][j];
                for (int k = 0; k < layers[layer_id].weights[j].size(); k++) {
                    layers[layer_id].weights[j][k] -= C * weight_changes[layer_id][j][k];
                }
            }
        }    
    }
       
    print_as_time((std::clock() - start) / (double)CLOCKS_PER_SEC);
}




//Calculate the partial derivative of the cost function with respect to a node specified by layer[id]-node[j]
//This is done recursively (backprop) so the calculated values are saved to be used by later iterations for
//better performance.
float Network::calc_dCdA(unsigned int id, unsigned int j, std::vector<std::vector<float> > &z_values, std::vector<std::vector<float> > &activations, std::vector<float> &y) {

    //To compare with the map of known values
    std::pair<unsigned int, unsigned int> pair(id, j);

    if (known_dCdA_values.find(pair) == known_dCdA_values.end()) {
        if (id == NUM_LAYERS - 1) {
            //Base case (dCdA of a node in output layer
            known_dCdA_values[pair] = 2 * (activations[id][j] - y[j]);
        }
        else {
            //Iterative case
            float val = 0;
            for (int x = 0; x < layers[id + 1].SIZE; x++) {
                val +=
                    layers[id + 1].weights[x][j]
                    * der_sigmoid(z_values[id][x])
                    * calc_dCdA(id + 1, x, z_values, activations, y);
            }
            known_dCdA_values[pair] = val;
        }
    }

    return known_dCdA_values[pair];

}