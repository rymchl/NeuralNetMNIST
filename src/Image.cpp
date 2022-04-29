#include"Image.h"
#include<iostream>
#include<math.h>

//ASCII sorted by brightness for printing images
static const char* ascii_lum = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
static const int ascii_lum_length = 73;

//Creates a new blank image
 Image::Image(unsigned int WIDTH, unsigned int HEIGHT, unsigned int LABEL) : 
    WIDTH(WIDTH), HEIGHT(HEIGHT), LABEL(LABEL) {
    for(unsigned int y = 0; y < HEIGHT; y++){
        pixel_values.push_back(std::vector<unsigned int>());
        for(unsigned int x = 0; x < WIDTH; x++){
            pixel_values[y].push_back(0);
        }
    }
}

void Image::put(int x, int y, int val){
    pixel_values[y][x] = val;
}

float Image::get(int x, int y){
    return (float)pixel_values[y][x];
}

//Prints the image as ascii with a border, spicifying the label, to stdout
void Image::print(){
    for (unsigned int i = 0; i <= (WIDTH / 2); i++) printf("+ + ");
    printf("\n+ LABEL : %d\n", LABEL);
    for (unsigned int i = 0; i <= (WIDTH / 2); i++) printf("+ + ");
    std::cout << std::endl;

    for(unsigned int y = 0; y < HEIGHT; y++){
        std::string line = "+"; 
        for(unsigned int x = 0; x < WIDTH; x++){
            int val = pixel_values[y][x];
            float brightness = fmax(fmin((float(val)/255.0f),1.0f),0.0f);            
            
            int max = ascii_lum_length-1;
            int index = int(brightness*max);

            if (index == 0 && brightness > 0) index = 1;
            
            char b_char = ascii_lum[index];
            line += b_char;
            line += b_char;
        }
        std::cout << line << std::endl;
    }

//border
    for(int i = 0; i <= (WIDTH / 2); i++) printf("+ + ");
    printf("\n");
}