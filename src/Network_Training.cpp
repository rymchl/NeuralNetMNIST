#include "Network.h"

float Network::calc_dCdA(unsigned int id, unsigned int j, std::vector<std::vector<float> > z_values, std::vector<std::vector<float> > activations, std::vector<float> y) {

    std::pair<unsigned int, unsigned int> pair(id,j);

    if (known_dCdA_values.find(pair) == known_dCdA_values.end()) {
        if (id == NUM_LAYERS - 1) {
            known_dCdA_values[pair] = 2 * (activations[id][j] - y[j]);
        }
        else {
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

void Network::train(std::vector<Image> training_set, std::vector<Image> testing_set, int sample_size) {
    std::clock_t start = clock();

    //layer//weight[to][from]
    std::vector<std::vector<std::vector<float> > > weight_changes;
    weight_changes.resize(layers.size());
    for (int i = 0; i < weight_changes.size(); i++) {
        weight_changes[i].resize(layers[i].weights.size());
        for (int j = 0; j < weight_changes[i].size(); j++) {
            weight_changes[i][j].resize(layers[i].weights[j].size());
        }
    }

    //layer//bias[node]
    std::vector<std::vector<float > > bias_changes;
    bias_changes.resize(layers.size());
    for (int i = 0; i < bias_changes.size(); i++) bias_changes[i].resize(layers[i].biases.size());

    std::vector<Image> sample_set = sample<Image>(sample_size, training_set);

    //Learn on all images in training set;
    for (Image image : sample_set) {

        //image.print();

        //printf("%.2f\n", 100 * float(image_index++) / training_set.size());

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

        //init y-vec and first cost-activation partial-derivative values
        std::vector<float> y;
        for (int i = 0; i < 10; i++) {
            y.push_back((i == image.LABEL) ? 1 : 0);
        }

        known_dCdA_values.clear();

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

    float conv_param = 0.4f;
    float C = conv_param / sample_size;


    for (int layer_id = 1; layer_id < layers.size(); layer_id++) {
        for (int j = 0; j < layers[layer_id].weights.size(); j++) {
            layers[layer_id].biases[j] -= C * bias_changes[layer_id][j];
            for (int k = 0; k < layers[layer_id].weights[j].size(); k++) {
                layers[layer_id].weights[j][k] -= C * weight_changes[layer_id][j][k];
            }
        }
    }


    print_as_time((std::clock() - start) / (double)CLOCKS_PER_SEC);
    printf("\0\n");

}